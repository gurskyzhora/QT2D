#include <QPainter>
#include <stdio.h>
#include <cmath>

#include "grafika.h"
#include "22.h"
#include "42.h"

#define A -20
#define B 20
#define N 10

static double f0 (double x) {
  x += 0.0;
  return 1;
}

static double f1 (double x) {
  return x;
}

static double f2 (double x) {
    return x * x;
}

static double f3 (double x) {
    return x * x * x;
}

static double f4 (double x) {
    return x * x * x *x;
}

static double f5 (double x) {
    return exp(x);
}

static double f6 (double x) {
    return 1./(25 * x * x + 1);
}

Window::Window (QWidget *parent)
  : QWidget (parent)
{
  a = A;
  b = B;
  n = N;
  func_id = 0;
  view_id = 0;
  change_func ();
}

QSize Window::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize Window::sizeHint () const
{
  return QSize (1000, 1000);
}

int Window::parse_command_line (int argc, char *argv[])
{
  if (argc == 1)
    return 0;

  if (argc == 2)
    return -1;

  if (   sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-6
      || (argc > 3 && sscanf (argv[3], "%d", &n) != 1)
      || n <= 0 )
    return -2;

  return 0;
}

/// change current function for drawing
void Window::change_func ()
{
  func_id = (func_id + 1) % 7;

  switch (func_id)
    {
      case 0:
        f_name = "f(x) = 1";
        f = f0;
        break;
      case 1:
        f_name = "f(x) = x";
        f = f1;
        break;
      case 2:
        f_name = "f(x) = x * x";
        f = f2;
        break;
      case 3:
        f_name = "f(x) = x * x * x";
        f = f3;
        break;
      case 4:
        f_name = "f(x) = x * x * x * x";
        f = f4;
        break;
      case 5:
        f_name = "f(x) = exp(x)";
        f = f5;
        break;
      case 6:
        f_name = "f(x) = 1/(25x*x + 1)";
        f = f6;
        break;
    }
  update ();
}

void Window::change_view()
{
    view_id = (view_id + 1) % 4;

    update();
}

void Window::scale_up()
{
    double t = (b - a) / 4;
    a = a + t;
    b = b - t;
    s++;
    update();
}
void Window::scale_down()
{
    double t = (b - a) / 2;
    a = a - t;
    b = b + t;
    s--;
    update();
}

void Window::points_up(){
    n *= 2;
    if (n <= 10e+7) update();
}

void Window::points_down(){
    n = n / 2;
    if (n >= 3) update();
    else{
        n = n * 2;
    }
}

void Window::delta_up()
{
    delta++;
    update();
}

void Window::delta_down()
{
    delta--;
    update();
}

/// render graph
void Window::paintEvent (QPaintEvent * /* event */)
{  
  QPainter painter (this);
  double x_1, x_2, x_3, y1, y2, y3;
  double max_y, min_y;
  double delta_y, delta_x = (b - a) / n;
  QPen pen_darkRed(Qt::darkRed, 0, Qt::SolidLine);
  QPen pen_darkMagenta(Qt::darkMagenta, 0, Qt::SolidLine);
  QPen pen_cyan(Qt::cyan, 0, Qt::SolidLine);
  QPen pen_blue(Qt::darkBlue, 0, Qt::SolidLine);
  QPen pen_darkGreen(Qt::darkGreen, 0, Qt::SolidLine);
  int i;

  max_y = min_y = 0;
  for (x_1 = a; x_1 - b < 1.e-6; x_1 += delta_x)
    {
      y1 = f (x_1);
      if (y1 < min_y)
        min_y = y1;
      if (y1 > max_y)
        max_y = y1;
    }

  delta_y = 0.01 * (max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;

  // save current Coordinate System
  painter.save ();

  // make Coordinate Transformations
  painter.translate (0.5 * width (), 0.5 * height ());
  painter.scale (width () / (b - a), -height () / (max_y - min_y));
  painter.translate (-0.5 * (a + b), -0.5 * (min_y + max_y));

  // draw approximated line for graph
  if (view_id != 3){
      painter.setPen (pen_darkRed);
      x_1 = a;
      y1 = f (x_1);
      for (x_2 = x_1 + delta_x, i = 1; x_2 - b < 1e-6; x_2 += delta_x, i++){
          for (x_3 = x_1 + delta_x * 1e-4; x_3 - x_2 < 1e-6; x_3 += delta_x * 0.0001){
              y3 = f(x_3);
              painter.drawLine (QPointF (x_1, y1), QPointF (x_3, y3));
              x_1 = x_3; y1 = y3;
          }
          y2 = f(x_2);
          if (i == n/2)
              y2 += delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y));
          x_1 = x_2; y1 = y2;
      }
      x_2 = b;
      y2 = f(x_2);
      painter.drawLine (QPointF (x_1, y1), QPointF (x_2, y2));
      free1();
      free2();
  }
  //график первого метода
 if ((view_id == 0 || view_id == 2) && n <= 50){
     painter.setPen (pen_blue);
     method_1(n, a, b, f);
     delta_method_1(n/2 - 1, delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y)));
     coefficients_method_1();
     x_1 = a;
     y1 = f_1(x_1);
         for (x_2 = x_1 + delta_x; x_2 - b < 1e-6; x_2 += delta_x)
         {
             for (x_3 = x_1 + delta_x * 1e-4; x_3 - x_2 < 1e-6; x_3 += delta_x * 0.0001)
             {
                 y3 = f_1(x_3);
                 painter.drawLine (QPointF (x_1, y1), QPointF (x_3, y3));
                 x_1 = x_3; y1 = y3;
             }

             y2 = f_1(x_2);

             x_1 = x_2; y1 = y2;
         }
         x_2 = b;
         y2 = f_1(x_2);
         painter.drawLine (QPointF (x_1, y1), QPointF (x_2, y2));
         free1();
     }

     //график второго метода
     if (view_id == 1 || view_id == 2){
         painter.setPen (pen_cyan);
         method_2(n, a, b, f);
         delta_method_2(n/2 - 1, delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y)));
         coefficients_method_2();
         x_1 = a;
         y1 = f_2(x_1);
         for (x_2 = x_1 + delta_x; x_2 - b < 1e-6; x_2 += delta_x){
             for (x_3 = x_1 + delta_x * 1e-4; x_3 - x_2 < 1e-6; x_3 += delta_x * 0.0001){
                 y3 = f_2(x_3);
                 painter.drawLine (QPointF (x_1, y1), QPointF (x_3, y3));
                 x_1 = x_3; y1 = y3;
             }
             y2 = f_2(x_2);
             x_1 = x_2; y1 = y2;
         }
         x_2 = b;
         y2 = f_2(x_2);
         painter.drawLine (QPointF (x_1, y1), QPointF (x_2, y2));
         free2();
     }

     if (view_id == 3 && n <= 50){
         if (n <= 50){ //разность фукнций для первого метода
             painter.setPen (pen_blue);
             method_1(n, a, b, f);
             delta_method_1(n/2 - 1, delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y)));
             coefficients_method_1();
             x_1 = a;
             y1 = f(x_1) - f_1(x_1);
             for (x_2 = x_1 + delta_x, i = 1; x_2 - b < 1e-6; x_2 += delta_x, i++){
                 for (x_3 = x_1 + delta_x * 1e-4; x_3 - x_2 < 1e-6; x_3 += delta_x * 0.0001){
                     y3 = f(x_1) - f_1(x_3);
                     painter.drawLine (QPointF (x_1, y1), QPointF (x_3, y3));
                     x_1 = x_3; y1 = y3;
                 }
                 y2 = f(x_2) - f_1(x_2);
                 if (i == n/2)
                     y2 += delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y));
                 x_1 = x_2; y1 = y2;
             }
             x_2 = b;
             y2 = f(x_2) - f_1(x_2);
             painter.drawLine (QPointF (x_1, y1), QPointF (x_2, y2));
             free1();
        }
        //разность функций для второго метода
         painter.setPen (pen_darkGreen);
         method_2(n, a, b, f);
         delta_method_2(n/2 - 1, delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y)));
         coefficients_method_2();
         x_1 = a;
         y1 = f(x_1) - f_2(x_1);
         for (x_2 = x_1 + delta_x, i = 1; x_2 - b < 1e-6; x_2 += delta_x, i++){
             for (x_3 = x_1 + delta_x * 1e-4; x_3 - x_2 < 1e-6; x_3 += delta_x * 0.0001){
                 y3 = f(x_1) - f_2(x_3);
                 painter.drawLine (QPointF (x_1, y1), QPointF (x_3, y3));
                 x_1 = x_3; y1 = y3;
             }
             y2 = f(x_2) - f_2(x_2);
             if (i == n/2)
                 y2 += delta * 0.1 * (abs(max_y) > abs(min_y) ? abs(max_y) : abs(min_y));
             x_1 = x_2; y1 = y2;
         }
         x_2 = b;
         y2 = f(x_2) - f_2(x_2);
         painter.drawLine (QPointF (x_1, y1), QPointF (x_2, y2));
         free2();

     }
     // draw axis
     painter.setPen (pen_darkMagenta);
     painter.drawLine (a, 0, b, 0);
     painter.drawLine (0, max_y, 0, min_y);

     // restore previously saved Coordinate System
     painter.restore ();

     // render function name
     painter.setPen (pen_darkRed);
     painter.drawText (0, 15, f_name);
     painter.drawText (0, 30, QString("ГРАФИК = %1").arg(view_id + 1));
     painter.drawText (0, 45, QString("МАСТШТАБ = %1 %2 %3").arg(s).arg(a).arg(b));
     painter.drawText (0, 75, QString("p = %1").arg(delta));
     painter.drawText (0, 60, QString("n = %1").arg(n));
 }

