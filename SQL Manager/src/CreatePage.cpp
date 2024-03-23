#include "CreatePage.h"

CreatePage::CreatePage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

CreatePage::~CreatePage()
{}


//	add a new row to vertical layout above the spacer with two line edits
void CreatePage::on_plusButton_clicked()
{
	QHBoxLayout* newLine = new QHBoxLayout();
	newLine->addWidget(new QLineEdit());
	newLine->addWidget(new QLineEdit());
	ui.verticalLayout->insertLayout(ui.verticalLayout->count() - 2, newLine);
}



