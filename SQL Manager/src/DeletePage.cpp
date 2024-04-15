#include "DeletePage.h"

DeletePage::DeletePage(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.scrollArea->widget()->setLayout(ui.ConditionsVLayout);
    ui.ConditionsVLayout->setAlignment(Qt::AlignTop);
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