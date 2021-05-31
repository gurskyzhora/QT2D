#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "grafika.h"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (window);
  QAction *action;

  if (graph_area->parse_command_line (argc, argv))
    {
      QMessageBox::warning (0, "Wrong input",
                            "Wrong input");
      return -1;
    }

  action = tool_bar->addAction ("Функция", graph_area, SLOT (change_func()));
     action->setShortcut (QString ("0"));

     action = tool_bar->addAction ("Вид", graph_area, SLOT (change_view()));
     action->setShortcut (QString ("1"));

     action = tool_bar->addAction ("Приблизить", graph_area, SLOT (scale_up()));
     action->setShortcut (QString ("2"));

     action = tool_bar->addAction ("Отдалить", graph_area, SLOT (scale_down()));
     action->setShortcut (QString ("3"));

     action = tool_bar->addAction ("Добавить точки", graph_area, SLOT (points_up()));
     action->setShortcut (QString ("4"));

     action = tool_bar->addAction ("Убрать точки", graph_area, SLOT (points_down()));
     action->setShortcut (QString ("5"));

     action = tool_bar->addAction ("Добавить max/10", graph_area, SLOT (delta_up()));
     action->setShortcut (QString ("6"));

     action = tool_bar->addAction ("Вычесть max/10", graph_area, SLOT (delta_down()));
     action->setShortcut (QString ("7"));

     action = tool_bar->addAction ("Выход", window, SLOT (close()));
     action->setShortcut (QString ("Ctrl+X"));

     tool_bar->setMaximumHeight (30);

     window->setMenuBar (tool_bar);
     window->setCentralWidget (graph_area);
     window->setWindowTitle ("Georgy_Gursky");

     window->show ();
     app.exec ();
     delete window;
     return 0;
 }
