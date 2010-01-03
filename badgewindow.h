#ifndef BADGEWINDOW_H
#define BADGEWINDOW_H

#include <QtGui/QMainWindow>
#include <QDate>

namespace Ui
{
    class BadgeWindow;
}

class BadgeWindow : public QMainWindow
{
    Q_OBJECT

public:
    BadgeWindow(QWidget *parent = 0);
    ~BadgeWindow();

private:
    Ui::BadgeWindow *ui;
public slots:
    void selectedDay(QDate);
    void search();
    void info();
    void configure();
};

#endif // BADGEWINDOW_H
