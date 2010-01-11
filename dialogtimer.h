#ifndef DIALOGTIMER_H
#define DIALOGTIMER_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class DialogTimer;
}

class DialogTimer : public QDialog {
    Q_OBJECT
public:
    DialogTimer(QWidget *parent = 0);
    ~DialogTimer();
    bool getTaskInfo(QString &taskName, QTime &elapsedTime);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogTimer *ui;
    QTimer *timer;
public slots:
    void updateClock();
    void start();
    void pause();
    void accept();
    void textChanged();
};

#endif // DIALOGTIMER_H
