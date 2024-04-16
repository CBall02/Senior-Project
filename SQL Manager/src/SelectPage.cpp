#include "SelectPage.h"
#include "SQLManagerGUI.h"
#include <QMessageBox>


SelectPage::SelectPage(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.attributesScrollArea->widget()->setLayout(ui.verticalAttributes);
    ui.verticalAttributes->setAlignment(Qt::AlignTop);
    ui.verticalAttributes->setContentsMargins(15, 15, 15, 15);
    ui.conditionsScrollArea->widget()->setLayout(ui.verticalConditions);
    ui.verticalConditions->setAlignment(Qt::AlignTop);
    ui.verticalConditions->setContentsMargins(15, 15, 15, 15);
    loadTablesComboBox();
    numConditions = 0;
    addCondition();
}

SelectPage::~SelectPage()
{}

void SelectPage::loadTablesComboBox()
{
    ui.tablesComboBox->clear();
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    for (std::string name : tables)
    {
        ui.tablesComboBox->addItem(QString::fromStdString(name));
    }
}

void SelectPage::addCondition()
{
    QHBoxLayout* newLine = new QHBoxLayout();
    QLabel* WHERE = new QLabel();
    QLineEdit* condition = new QLineEdit();

    WHERE->setText("WHERE");
    newLine->addWidget(WHERE);
    newLine->addWidget(condition);

    ui.verticalConditions->insertLayout(ui.verticalConditions->count(), newLine);
    conditions.push_back(condition);
    labels.push_back(WHERE);
    numConditions++;
}

void SelectPage::on_tablesComboBox_currentIndexChanged(int index)
{
    while (selections.size() > 0)
    {
        delete selections.back();
        selections.pop_back();
    }
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    std::vector<Database::Column> columns = Database::instance()->getTableSchema(tables.at(index));
    for (Database::Column column : columns)
    {
        QCheckBox* attribute = new QCheckBox();
        attribute->setText(QString::fromStdString(column.name));
        selections.push_back(attribute);
        ui.attributesScrollArea->widget()->layout()->addWidget(attribute);
    }
}

void SelectPage::on_plusButton_clicked()
{
    addCondition();
}

void SelectPage::on_minusButton_clicked()
{
    if (numConditions > 0)
    {
        delete conditions.back();
        conditions.pop_back();
        delete labels.back();
        labels.pop_back();
        delete ui.verticalConditions->takeAt(ui.verticalConditions->count() - 1);
        numConditions--;
    }
}

void SelectPage::on_selectButton_clicked() {
    sqlGenerator::SelectModel sqlCommand;
    std::string tableName = ui.tablesComboBox->currentText().toStdString();
    int numAttribute = Database::instance()->getTableSchema(tableName).size();
    
    for (int i = 0; i < selections.size(); i++) {
        QCheckBox* attribute = selections[i];
        if (attribute->isChecked()) {
            sqlCommand.select(attribute->text().toStdString());
        }
    }
    sqlCommand.from(tableName);
    for (int i = 0; i < conditions.size(); i++) {
        sqlCommand.where(conditions[i]->text().toStdString());
    }
    if (ui.groupByEdit->text() != "") {
        sqlCommand.group_by(ui.groupByEdit->text().toStdString());
    }
    if (ui.orderByEdit->text() != "") {
        sqlCommand.order_by(ui.orderByEdit->text().toStdString());
    }

    emit selectCommandRequested(sqlCommand.str());
}



