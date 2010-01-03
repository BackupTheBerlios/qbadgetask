#ifndef DIALOGCALENDAR_H
#define DIALOGCALENDAR_H

#include <QDialog>
#include <QDate>

namespace Ui {
    class DialogCalendar;
}

class DialogCalendar : public QDialog {
    Q_OBJECT
public:
    DialogCalendar(QWidget *parent = 0);
    ~DialogCalendar();
    QDate getDate(QDate date);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogCalendar *ui;
};

#endif // DIALOGCALENDAR_H
