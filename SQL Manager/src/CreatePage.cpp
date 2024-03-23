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
	sqlCommand += "\n{\n";
	for (int i = 0; i < numAttributes; i++)
	{
		QBoxLayout* input_at_i = (QBoxLayout*) ui.verticalLayout->itemAt(i + 3);
		QLineEdit* name_at_i = (QLineEdit*)input_at_i->itemAt(0);
		QLineEdit* type_at_i = (QLineEdit*)input_at_i->itemAt(1);
		sqlCommand += name_at_i->text() + ' ' + type_at_i->text() + '\n';
	}
	sqlCommand += "};";
	if (Database::instance()->sqlExec(sqlCommand.toStdString()))
	{

	}
}
