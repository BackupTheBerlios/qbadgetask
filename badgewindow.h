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
    unsigned int days; // 1 mon 2 Tue 4 wed 8 Thu 16 Fri 32 Sat 64 Sun
    QTime workingTime;
public slots:
    void selectedDay(QDate);
    void search();
    void info();
    void configure();
    void timer();
};

#endif // BADGEWINDOW_H
