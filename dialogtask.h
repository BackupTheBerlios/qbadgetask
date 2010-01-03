#ifndef DIALOGTASK_H
#define DIALOGTASK_H

#include <QDate>
#include <QDialog>
#include <QtSql>


namespace Ui {
    class DialogTask;
}

class DialogTask : public QDialog {
    Q_OBJECT
public:
    DialogTask(QWidget *parent = 0);
    ~DialogTask();
    void openTask(QString title, QString labelTime, QSqlRelationalTableModel *model, QDate data, QString dbtable);

protected:
    void changeEvent(QEvent *e);
    int findAttivitaId(const QString &attivita);
    int addNewAttivita(const QString &attivita);
    int generateAttivitaId();
    int generateNoteId();
    QSqlRecord findAttivitaInDay(QString, QDate);
    void addNote(int id);
    QSqlRecord findNote(int id, QString table, QDate day, int &row);
private:
    Ui::DialogTask *ui;
    QSqlRelationalTableModel *model;
    QDate day;
    int nextId();
    QString table;
    bool textNoteChanged;
    int currentRow;
public slots:
    void insert();
    void selectedRow(QModelIndex);
    void noteChanged();
};

#endif // DIALOGTASK_H
