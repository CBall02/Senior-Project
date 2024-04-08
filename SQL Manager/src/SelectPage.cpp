#include "SelectPage.h"


SelectPage::SelectPage(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    loadTablesComboBox();
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
        QHBoxLayout* newLine = new QHBoxLayout();
        QCheckBox* attribute = new QCheckBox();
        attribute->setText(QString::fromStdString(column.name));
        selections.push_back(attribute);
        newLine->addWidget(attribute);
        ui.verticalAttributes->insertLayout(ui.verticalAttributes->count() - 1, newLine);
    }
}

void SelectPage::on_plusButton_clicked()
{
    QHBoxLayout* newLine = new QHBoxLayout();
    QLabel* WHERE = new QLabel();
    QLineEdit* condition = new QLineEdit();
    WHERE->setText("WHERE");
    newLine->addWidget(WHERE);
    newLine->addWidget(condition);
    ui.verticalConditions->insertLayout(ui.verticalConditions->count() - 1, newLine);
    conditions.push_back(condition);
    labels.push_back(WHERE);
    numConditions++;
}

void SelectPage::on_minusButton_clicked()
{
    if (numConditions > 0)
    {
        delete conditions.back();
        conditions.pop_back();
        delete labels.back();
        labels.pop_back();
        delete ui.verticalConditions->takeAt(ui.verticalConditions->count() - 2);
    }
}