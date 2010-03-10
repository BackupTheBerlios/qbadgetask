#include "dialogtask.h"
#include "ui_dialogtask.h"
#include "badgedata.h"
#include <QMessageBox>
#include <QCompleter>

DialogTask::DialogTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTask)
{
    ui->setupUi(this);


    /* Model usato esclusivamente per il completer */

    QSqlTableModel *model = new QSqlTableModel;
    QStringList listAttivita;
    QSqlRecord record;

    model->setTable("attivita");

    model->select();
    model->removeColumn(0);
    model->select();

    int row = 0;

    while (row < model->rowCount()) {
        record = model->record(row);
        listAttivita << record.value("attivita").toString();

        row++;
    }


    QCompleter *completer = new QCompleter(listAttivita);



    ui->lineEdit->setCompleter(completer);
}


void DialogTask::insert()
{
    BadgeData data;
    QTime total;
    QTime remain;


    if (ui->lineEdit->text().size() == 0) {
        QMessageBox::warning(this, "Insert", "Activity is empty");
        return;
    }


    int idAttivita = findAttivitaId(ui->lineEdit->text().toUpper());



    if (idAttivita > 0) {
        QSqlRecord record = findAttivitaInDay(ui->lineEdit->text().toUpper(), day);
        QSqlTableModel m;
        int id;
        QSqlField f0("own", QVariant::String);
        QSqlField f1("id", QVariant::Int);
        QSqlField f2("day", QVariant::Date);

        QSqlField f4("how", QVariant::Int);
        QSqlField f3("time", QVariant::Time);

        f0.setValue(QVariant(ui->lineEditOwn->text()));
        f3.setValue(QVariant(ui->timeEdit->time()));
        f4.setValue(QVariant(idAttivita));
        f2.setValue(QVariant(day));

        m.setTable(table);
        m.setFilter("day='" + day.toString("yyyy-MM-dd") + "'");
        m.select();




        if (record.count() == 0) {

            id = this->nextId();


            f1.setValue(QVariant(id));       

            record.append(f0);
            record.append(f1);
            record.append(f2);
            record.append(f3);
            record.append(f4);
            m.insertRecord(-1, record);

            m.submitAll();
            model->select();
            currentRow = model->rowCount() - 1;

        }
        else {


            QSqlRecord recordTask;
            id = record.value(1).toInt();



            f1.setValue(QVariant(id));
            recordTask.append(f0);
            recordTask.append(f1);
            recordTask.append(f2);
            recordTask.append(f3);
            recordTask.append(f4);



            m.setRecord(currentRow , recordTask);

            m.submit();

        }


        // Add note
        addNote(id);
        model->submitAll();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->resizeColumnsToContents();

        // here to paint tableView in beatiful mode

        model->select();

    }
    textNoteChanged = false;
    ui->pushButtonInsert->setEnabled(false);

    data.dayRemain(day, total, remain);

    ui->timeEditRemain->setTime(remain);

    ui->pushButtonInsert->setEnabled(false);


}

void DialogTask::addNote(int id)
{


    if (textNoteChanged) {


        QSqlTableModel modelNote;
        QSqlRecord recordNote;
        QSqlRecord record;
        int row;
        QSqlField f1("id", QVariant::Int);
        QSqlField f2("note", QVariant::String);
        QSqlField f3((table+"id"), QVariant::Int);


        record = findNote(id, (table), day, row);


        f2.setValue(QVariant(ui->textEdit->toPlainText()));
        f3.setValue(QVariant(id));

        modelNote.setTable((table + "note"));



        if (record.count() == 0) {
            f1.setValue(QVariant(generateNoteId()));
            recordNote.append(f1);
            recordNote.append(f2);
            recordNote.append(f3);
            modelNote.insertRecord(-1, recordNote);

        }
        else {
            f1.setValue(QVariant(record.value(0).toInt()));
            recordNote.append(f1);
            recordNote.append(f2);
            recordNote.append(f3);
            modelNote.select();
            modelNote.setRecord(row , recordNote);

            modelNote.submit();
        }









    }
}


void DialogTask::openTask(QString title, QString labelTime, QSqlRelationalTableModel *modelTask, QDate daySelected, QString dbtable, QTime initialTime, QString name)
{
    BadgeData data;
    QTime total;
    QTime remain;

    day = daySelected;
    this->setWindowTitle(title);
    ui->label->setText(labelTime);
    ui->lineEdit->setText(name);
    ui->timeEdit->setTime(initialTime);
    day = daySelected;
    model = modelTask;
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
    table = dbtable;
    textNoteChanged = false;
    ui->pushButtonInsert->setEnabled(false);

    data.dayRemain(daySelected, total, remain);
    ui->timeEditTotal->setTime(total);
    ui->timeEditRemain->setTime(remain);


    if (name.size() > 0)
        ui->pushButtonInsert->setEnabled(true);

    if (this->exec() == QDialog::Accepted) {

    }
}

void DialogTask::noteChanged()
{
    textNoteChanged = true;
    ui->pushButtonInsert->setEnabled(true);
}

int DialogTask::nextId()
{
    QSqlQuery query;
    query.prepare(("SELECT max(id) FROM " + table));

    query.exec();
    query.next();
    return (query.value(0).toInt() + 1);

}



int DialogTask::findAttivitaId(const QString &attivita)
{
    QSqlTableModel *attivitaModel = model->relationModel(4);
    int row = 0;

    while (row < attivitaModel->rowCount()) {
        QSqlRecord record = attivitaModel->record(row);
        if (record.value("attivita") == attivita)
            return record.value("id").toInt();
        else
            row++;
    }
    return addNewAttivita(attivita);
}


QSqlRecord DialogTask::findNote(int id, QString tablenote, QDate day, int &row)
{
    QSqlRecord record;
    QSqlTableModel m;
    m.setTable(table + "note");
    m.select();
    row = 0;



    while (row < m.rowCount()) {
        record = m.record(row);
        if (record.value(tablenote + "id").toInt() == id) {

            return record;
        }
        row++;
    }
    record.clear();
    return record;
}

QSqlRecord DialogTask::findAttivitaInDay(QString attivita, QDate day)
{
    QSqlRecord record;
    QSqlTableModel m;
    //m.setTable(table);
    //m.setFilter("day='" + day.toString("yyyy-MM-dd") + "'");

    int row = 0;

    while (row < model->rowCount()) {
        record = model->record(row);
        if (record.value("attivita") == attivita && record.value("own") == ui->lineEditOwn->text()) {

            return record;
        }
        row++;
    }
    record.clear();
    return record;
}



int DialogTask::addNewAttivita(const QString &attivita)
{
    QSqlTableModel *attivitaModel = model->relationModel(4);
    QSqlRecord record;

    int id = generateAttivitaId();

    QSqlField f1("id", QVariant::Int);
    QSqlField f2("attivita", QVariant::String);


    f1.setValue(QVariant(id));
    f2.setValue(QVariant(attivita));
    record.append(f1);
    record.append(f2);

    attivitaModel->insertRecord(-1, record);
    attivitaModel->submitAll();
    return id;
}

int DialogTask::generateAttivitaId()
{
    QSqlQuery query;
    query.prepare("SELECT max(id) FROM attivita");

    query.exec();
    query.next();
    return (query.value(0).toInt() + 1);
}


int DialogTask::generateNoteId()
{
    QSqlQuery query;
    query.prepare(("SELECT max(id) FROM " + (table + "note")));

    query.exec();
    query.next();
    return (query.value(0).toInt() + 1);
}


void DialogTask::selectedRow(QModelIndex index)
{
    QSqlRecord record;
    QSqlTableModel modelNoteTask;

    modelNoteTask.setTable((table + "note"));


    record = model->record(index.row());
    modelNoteTask.setFilter(((table + "id=") + record.value("id").toString()));

    if (modelNoteTask.select()) {

        QSqlRecord recordNote = modelNoteTask.record(0);
        ui->textEdit->setText(recordNote.value("note").toString());
    }
    ui->lineEdit->setText(record.value("attivita").toString());
    ui->lineEditOwn->setText(record.value("own").toString());
    ui->timeEdit->setTime(record.value("time").toTime());
    textNoteChanged = false;

    currentRow = index.row();

}


void DialogTask::timeChanged()
{
    ui->pushButtonInsert->setEnabled(true);
}

void DialogTask::activityChanged()
{
    ui->pushButtonInsert->setEnabled(true);
}

void DialogTask::newTask()
{
    QTime zeroTime;
    zeroTime.setHMS(0, 0, 0);
    ui->lineEdit->clear();
    ui->lineEditOwn->clear();
    ui->textEdit->clear();
    ui->timeEdit->setTime(zeroTime);
    ui->pushButtonInsert->setEnabled(true);
}

DialogTask::~DialogTask()
{
    delete ui;
}

void DialogTask::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
