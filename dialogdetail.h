#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
namespace Ui {
    class DialogDetail;
}

class DialogDetail : public QDialog {
    Q_OBJECT
public:
    DialogDetail(QWidget *parent = 0);
    ~DialogDetail();
    void showDetails(QSqlQueryModel *model, QString day);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDetail *ui;
};

#endif // DIALOGDETAIL_H
