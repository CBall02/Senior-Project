#include "CreatePage.h"
#include <sqlGenerator.h>

CreatePage::CreatePage(QWidget* parent)
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
		for (int i = 0; i < 3; i++)
		{
			delete tableConstraints.back();
			tableConstraints.pop_back();
		}
		delete ui.verticalLayout->takeAt(ui.verticalLayout->count() - 2);
		numAttributes--;
	}
}

/*
	read input from all line edits and checkboxes
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
		bool primary = tableConstraints.at(3 * i)->isChecked();
		bool notNull = tableConstraints.at(3 * i + 2)->isChecked();
		bool unique = tableConstraints.at(3 * i + 1)->isChecked();
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
		sqlCommand.columns(name->text().toStdString(), typeString, primary, notNull, unique);
	}
	if (Database::instance()->sqlExec(sqlCommand.str()))
	{
		emit tableCreated(sqlCommand.str());
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
	QCheckBox* primaryKey = new QCheckBox();
	QCheckBox* unique = new QCheckBox();
	QCheckBox* notNull = new QCheckBox();
	type->addItem("Type");
	type->addItem("int");
	type->addItem("varchar");
	type->addItem("boolean");
	type->addItem("datetime");
	type->addItem("float");
	primaryKey->setText("Primary Key");
	unique->setText("Unique");
	notNull->setText("Not Null");
	newLine->addWidget(name);
	newLine->addWidget(type);
	newLine->addWidget(primaryKey);
	newLine->addWidget(unique);
	newLine->addWidget(notNull);

	/*connect(primaryKey, &QCheckBox::clicked, this, &CreatePage::primaryKeyClicked);
	connect(unique, &QCheckBox::clicked, this, &CreatePage::uniqueOrNotNullClicked);
	connect(notNull, &QCheckBox::clicked, this, &CreatePage::uniqueOrNotNullClicked);*/

	connect(primaryKey, SIGNAL(clicked()), this, SLOT(primaryKeyClicked()));
	connect(unique, SIGNAL(clicked()), this, SLOT(uniqueOrNotNullClicked()));
	connect(notNull, SIGNAL(clicked()), this, SLOT(uniqueOrNotNullClicked()));

	tableNames.push_back(name);
	tableTypes.push_back(type);
	tableConstraints.push_back(primaryKey);
	tableConstraints.push_back(unique);
	tableConstraints.push_back(notNull);
	ui.verticalLayout->insertLayout(ui.verticalLayout->count() - 1, newLine);
	numAttributes++;
}

void CreatePage::primaryKeyClicked() {
	QCheckBox* primaryKey = qobject_cast<QCheckBox*>(sender());
	if (primaryKey->isChecked()) {
		for (int i = 0; i < numAttributes; i++)
		{
			if (primaryKey == tableConstraints.at(3 * i))
			{
				tableConstraints.at(3 * i + 1)->setCheckState(Qt::Checked);
				tableConstraints.at(3 * i + 2)->setCheckState(Qt::Checked);
			}
		}
	}
}
void CreatePage::uniqueOrNotNullClicked() {
	QCheckBox* uniqueOrNull = qobject_cast<QCheckBox*>(sender());
	if (!uniqueOrNull->isChecked()) {
		for (int i = 0; i < numAttributes; i++)
		{
			if (uniqueOrNull == tableConstraints.at(3 * i + 1) || uniqueOrNull == tableConstraints.at(3 * i + 2))
			{
				tableConstraints.at(3 * i)->setCheckState(Qt::Unchecked);
			}
		}
	}
}