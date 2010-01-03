#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>

namespace Ui {
    class DialogSearch;
}

class DialogSearch : public QDialog {
    Q_OBJECT
public:
    DialogSearch(QWidget *parent = 0);
    ~DialogSearch();
    QTime totalHours(bool &ok);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogSearch *ui;
public slots:
    void start();
    void end();
    void accept();
};

#endif // DIALOGSEARCH_H
