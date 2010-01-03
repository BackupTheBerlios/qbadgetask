#include "dialogtask.h"
#include "ui_dialogtask.h"

DialogTask::DialogTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTask)
{
    ui->setupUi(this);
}


void DialogTask::insert()
{
    int idAttivita = findAttivitaId(ui->lineEdit->text());



    if (idAttivita > 0) {
        QSqlRecord record = findAttivitaInDay(ui->lineEdit->text(), day);
        QSqlTableModel m;
        int id;
        QSqlField f1("id", QVariant::Int);
        QSqlField f2("day", QVariant::Date);

        QSqlField f4("how", QVariant::Int);
        QSqlField f3("time", QVariant::Time);

        f3.setValue(QVariant(ui->timeEdit->time()));
        f4.setValue(QVariant(idAttivita));
        f2.setValue(QVariant(day));

        m.setTable(table);
        m.setFilter("day='" + day.toString("yyyy-MM-dd") + "'");
        m.select();

        if (record.count() == 0) {

            id = this->nextId();


            f1.setValue(QVariant(id));       


            record.append(f1);
            record.append(f2);
            record.append(f3);
            record.append(f4);
            if (m.insertRecord(-1, record))
                qDebug() << "INSERITO\n";
            else
                qDebug() << "Inserimento fallito\n";
            m.submitAll();
            model->select();
            currentRow = model->rowCount() - 1;
            //ui->tableView->selectRow(m.rowCount() - 1);
        }
        else {


            QSqlRecord recordTask;
            id = record.value(0).toInt();


            f1.setValue(QVariant(id));

            recordTask.append(f1);
            recordTask.append(f2);
            recordTask.append(f3);
            recordTask.append(f4);

            qDebug() << "CURRENT " << currentRow << endl;


            if (m.setRecord(currentRow , recordTask)) {
                qDebug() << "UPDATE OK\n";
            }
            else
                qDebug() << "UPDATE KO " << m.lastError() << endl;
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


        record = findNote(id, (table + "note"), day, row);


        f2.setValue(QVariant(ui->textEdit->toPlainText()));
        f3.setValue(QVariant(id));

        modelNote.setTable((table + "note"));



        if (record.count() == 0) {
            f1.setValue(QVariant(generateNoteId()));
            recordNote.append(f1);
            recordNote.append(f2);
            recordNote.append(f3);
            if (modelNote.insertRecord(-1, recordNote))
                qDebug() << "NOTE INSERITO\n";
            else
                qDebug() << "NOTE Inserimento fallito\n";
        }
        else {
            f1.setValue(QVariant(record.value(0).toInt()));
            recordNote.append(f1);
            recordNote.append(f2);
            recordNote.append(f3);
            modelNote.select();
            if (modelNote.setRecord(row , recordNote)) {
                qDebug() << "UPDATE OK\n";
            }
            else
                qDebug() << "UPDATE KO " << endl;
            modelNote.submit();
        }









    }
}


void DialogTask::openTask(QString title, QString labelTime, QSqlRelationalTableModel *modelTask, QDate daySelected, QString dbtable)
{
    this->setWindowTitle(title);
    ui->label->setText(labelTime);
    day = daySelected;
    model = modelTask;
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
    table = dbtable;
    textNoteChanged = false;
    if (this->exec() == QDialog::Accepted) {

    }
}

void DialogTask::noteChanged()
{
    textNoteChanged = true;
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
    QSqlTableModel *attivitaModel = model->relationModel(3);
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
    m.setTable(tablenote);
    m.select();
    row = 0;

    while (row < m.rowCount()) {
        record = m.record(row);
        if (record.value("id") == id) {

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
        if (record.value("attivita") == attivita) {

            return record;
        }
        row++;
    }
    record.clear();
    return record;
}



int DialogTask::addNewAttivita(const QString &attivita)
{
    QSqlTableModel *attivitaModel = model->relationModel(3);
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
    ui->timeEdit->setTime(record.value("time").toTime());
    textNoteChanged = false;

    currentRow = index.row();

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
