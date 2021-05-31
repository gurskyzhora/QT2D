#include <QtWidgets/QtWidgets>

class Window : public QWidget
{
  Q_OBJECT

private:
  int func_id;
  int view_id;
  const char *f_name;
  double a;
  double b;
  int n;
  int s = 0;
  int delta = 0;
  double (*f) (double);

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);

public slots:
  void change_func ();
  void change_view ();
  void scale_up ();
  void scale_down ();
  void points_up ();
  void points_down ();
  void delta_up ();
  void delta_down ();

protected:
  void paintEvent (QPaintEvent *event);
};
