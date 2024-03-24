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
	QLineEdit* name = new QLineEdit();
	QLineEdit* type = new QLineEdit();
	newLine->addWidget(name);
	newLine->addWidget(type);
	table.push_back(name);
	table.push_back(type);
	ui.verticalLayout->insertLayout(ui.verticalLayout->count() - 1, newLine);
	numAttributes++;
}

/*	
	read input from all line edits
	create table with given name and properties
	close window
*/
void CreatePage::on_createButton_clicked()
{
	QString sqlCommand = "CREATE TABLE ";
	sqlCommand += ui.nameEdit->text();
	sqlCommand += "\n(\n";
	sqlCommand += ui.att1name->text() + ' ' + ui.att1type->text() + '\n';
	for (int i = 0; i < numAttributes - 1; i++)
	{
		QLineEdit* name = table.at(2 * i);
		QLineEdit* type = table.at(2 * i + 1);
		sqlCommand += name->text() + ' ' + type->text() + '\n';
	}
	sqlCommand += ");";
	if (Database::instance()->sqlExec(sqlCommand.toStdString()))
	{

		close();
	}
	else
	{
		
	}
}
