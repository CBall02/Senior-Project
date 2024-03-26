#include "CreatePage.h"
#include <sqlGenerator.h>

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
	QLineEdit* name = new QLineEdit();
	QLineEdit* type = new QLineEdit();
	newLine->addWidget(name);
	newLine->addWidget(type);
	table.push_back(name);
	table.push_back(type);
	ui.verticalLayout->insertLayout(ui.verticalLayout->count() - 1, newLine);
	numAttributes++;
}

// remove a row of line edits from vertical layout if numAttributes > 1
void CreatePage::on_minusButton_clicked()
{
	if (numAttributes > 1)
	{
		delete table.back();
		table.pop_back();
		delete table.back();
		table.pop_back();
		delete ui.verticalLayout->takeAt(ui.verticalLayout->count() - 2);
		numAttributes--;
	}
}

/*	
	read input from all line edits
	create table with given name and properties
	close window
*/
void CreatePage::on_createButton_clicked()
{
	sqlGenerator::CreateModel sqlCommand;
	sqlCommand.tableName(ui.nameEdit->text().toStdString());
	sqlCommand.columns(ui.att1name->text().toStdString(), ui.att1type->text().toStdString());

	for (int i = 0; i < numAttributes - 1; i++)
	{
		QLineEdit* name = table.at(2 * i);
		QLineEdit* type = table.at(2 * i + 1);
		sqlCommand.columns(name->text().toStdString(), type->text().toStdString());
	}
	if (Database::instance()->sqlExec(sqlCommand.str()))
	{
		emit tableCreated();
		close();
	}
	else
	{
		
	}
}
