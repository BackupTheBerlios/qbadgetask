#include "dialogtimer.h"
#include "ui_dialogtimer.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QCompleter>
#include <QMessageBox>

DialogTimer::DialogTimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTimer)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);

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
    timer = new QTimer();
    timer->setInterval(60000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
}

void DialogTimer::updateClock()
{
    QTime time = ui->timeEdit->time();
    QTime t;
    t = time.addSecs(60);
    qDebug() << "t " << t.toString("hh:mm:ss");
    ui->timeEdit->setTime(t);
    qDebug() << "TIMEOUT\n";
}

void DialogTimer::start()
{
    timer->start();
    ui->pushButtonPause->setEnabled(true);
    ui->pushButtonStart->setEnabled(false);
}


void DialogTimer::pause()
{
    timer->stop();
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStart->setEnabled(true);
}


bool DialogTimer::getTaskInfo(QString &taskName, QTime &elapsedTime)
{
    if (this->exec() == QDialog::Accepted) {
        taskName = ui->lineEdit->text();
        elapsedTime = ui->timeEdit->time();
        return true;
    }
    return false;
}

void DialogTimer::accept()
{
    qDebug() << "accept";
    if (ui->lineEdit->text().size() == 0) {
        QMessageBox::warning(this, "Warning", "Task name is empty");
    }
    else
        QDialog::accept();
}

void DialogTimer::textChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
}

DialogTimer::~DialogTimer()
{
    delete ui;
    delete timer;
}

void DialogTimer::changeEvent(QEvent *e)
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
