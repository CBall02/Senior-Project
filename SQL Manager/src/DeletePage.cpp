#include "DeletePage.h"
#include <sqlGenerator.h>

DeletePage::DeletePage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    QString cs1 = "QPushButton {"
        "background-color: rgb(210, 226, 231);"
        "color: rgb(0, 0, 0);"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(255, 255, 255);"
        "color: rgb(0, 0, 0);"
        "}";
    ui.deleteButton->setStyleSheet(cs1);
    ui.scrollArea->widget()->setLayout(ui.ConditionsVLayout);
    ui.ConditionsVLayout->setAlignment(Qt::AlignTop);
    loadTablesComboBox();
    addCondition();
}

DeletePage::~DeletePage()
{}

void DeletePage::loadTablesComboBox()
{
    ui.tablesComboBox->clear();
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    for (std::string name : tables)
    {
        ui.tablesComboBox->addItem(QString::fromStdString(name));
    }
}

void DeletePage::on_plusButton_clicked()
{
    addCondition();
}

void DeletePage::on_minusButton_clicked()
{
    if (numConditions > 0)
    {
        delete conditions.back();
        conditions.pop_back();
        delete labels.back();
        labels.pop_back();
        delete ui.scrollArea->widget()->layout()->takeAt(ui.scrollArea->widget()->layout()->count() - 1);
        numConditions--;
    }
}

void DeletePage::addCondition()
{
    QHBoxLayout* newLine = new QHBoxLayout();
    QLabel* WHERE = new QLabel();
    QLineEdit* condition = new QLineEdit();
    QWidget* attribute = new QWidget();

    WHERE->setText("WHERE");
    newLine->addWidget(WHERE);
    newLine->addWidget(condition);

    conditions.push_back(condition);
    labels.push_back(WHERE);

    attribute->setLayout(newLine);
    attribute->setFixedHeight(40);
    attribute->setFixedWidth(400);
    ui.scrollArea->widget()->layout()->addWidget(attribute);

    numConditions++;
}

void DeletePage::on_deleteButton_clicked()
{
    sqlGenerator::DeleteModel sqlCommand;
    sqlCommand.from(ui.tablesComboBox->currentText().toStdString());
    for (int i = 0; i < numConditions; i++)
    {
        sqlCommand.where(conditions.at(i)->text().toStdString());
    }
    if (Database::instance()->sqlExec(sqlCommand.str()))
    {
        emit tableDeleted(sqlCommand.str());
        close();
    }
}