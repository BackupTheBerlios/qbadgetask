#ifndef DIALOGSTATISTICS_H
#define DIALOGSTATISTICS_H

#include <QDialog>
#include <QDate>
#include <QSqlQueryModel>

namespace Ui {
    class DialogStatistics;
}

class DialogStatistics : public QDialog {
    Q_OBJECT
public:
    DialogStatistics(QWidget *parent = 0);
    void showStatistics(QDate begin, QDate end, int total, int overTime, QSqlQueryModel *model);
    ~DialogStatistics();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogStatistics *ui;
    QString _begin;
    QString _end;

public slots:
    void details(QModelIndex);

};

#endif // DIALOGSTATISTICS_H
