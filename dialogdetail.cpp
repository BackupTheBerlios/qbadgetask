#include "dialogdetail.h"
#include "ui_dialogdetail.h"

DialogDetail::DialogDetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetail)
{
    ui->setupUi(this);

}


void DialogDetail::showDetails(QSqlQueryModel *model, QString task)
{
    setWindowTitle("Details of the " + task);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
    this->exec();
}

DialogDetail::~DialogDetail()
{
    delete ui;
}

void DialogDetail::changeEvent(QEvent *e)
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
