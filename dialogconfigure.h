#ifndef DIALOGCONFIGURE_H
#define DIALOGCONFIGURE_H

#include <QDialog>

namespace Ui {
    class DialogConfigure;
}

class DialogConfigure : public QDialog {
    Q_OBJECT
public:
    DialogConfigure(QWidget *parent = 0);
    ~DialogConfigure();
    bool openConfigure(unsigned int &days, QTime &time);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogConfigure *ui;
};

#endif // DIALOGCONFIGURE_H
