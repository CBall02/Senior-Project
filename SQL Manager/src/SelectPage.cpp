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
    for (int i = 0; i < selections.size(); i++)
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
        ui.verticalLayout->insertLayout(ui.verticalLayout->count() - 1, newLine);
    }
}