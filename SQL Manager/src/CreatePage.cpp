#include "CreatePage.h"
#include <sqlGenerator.h>

CreatePage::CreatePage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	addAttribute();
}

CreatePage::~CreatePage()
{}


//	add a new row to vertical layout above the spacer with two line edits
void CreatePage::on_plusButton_clicked()
{
	addAttribute();
}

// remove a row of line edits from vertical layout if numAttributes > 1
void CreatePage::on_minusButton_clicked()
{
	if (numAttributes > 1)
	{
		delete tableNames.back();
		tableNames.pop_back();
		delete tableTypes.back();
		tableTypes.pop_back();
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
	for (int i = 0; i < numAttributes; i++)
	{
		QLineEdit* name = tableNames.at(i);
		int typeIndex = tableTypes.at(i)->currentIndex();
		std::string typeString;
		if (typeIndex == 0)
		{
			break;
		}
		if (typeIndex == 1)
		{
			typeString = "int";
		}
		else if (typeIndex == 2)
		{
			typeString = "varchar(20)";
		}
		else if (typeIndex == 3)
		{
			typeString = "boolean";
		}
		else if (typeIndex == 4)
		{
			typeString = "datetime";
		}
		else if (typeIndex == 5)
		{
			typeString = "float";
		}
		sqlCommand.columns(name->text().toStdString(), typeString);
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

void CreatePage::addAttribute()
{
	QHBoxLayout* newLine = new QHBoxLayout();
	QLineEdit* name = new QLineEdit();
	QComboBox* type = new QComboBox();
	type->addItem("Type");
	type->addItem("int");
	type->addItem("varchar");
	type->addItem("boolean");
	type->addItem("datetime");
	type->addItem("float");
	newLine->addWidget(name);
	newLine->addWidget(type);
	tableNames.push_back(name);
	tableTypes.push_back(type);
	ui.verticalLayout->insertLayout(ui.verticalLayout->count() - 1, newLine);
	numAttributes++;
}