#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>
#include <QDate>

namespace Ui {
    class DialogSearch;
}

class DialogSearch : public QDialog {
    Q_OBJECT
public:
    DialogSearch(QWidget *parent = 0);
    ~DialogSearch();
    //QTime totalHours(bool &ok);
    bool range(QDate &begin, QDate &end, QString &own);
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
