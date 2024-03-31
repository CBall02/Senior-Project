#include "SelectPage.h"


SelectPage::SelectPage(QWidget *parent)
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

void SelectPage::on_loadTablesComboBox_currentIndexChanged(int index)
{
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    auto table = Database::instance()->getTable(tables.at(index));
    for (int i = 0; i < table.numFields(); i++)
    {

    }
}