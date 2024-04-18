#include "InsertPage.h"
#include <qlineedit.h>
#include <sqlGenerator.h>

std::vector<std::string> tables;
std::vector<Database::Column> columns;
int ind;//Important to carry these across functions

InsertPage::InsertPage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	for (int i = 0; i < numAttributes; i++) {
		elementAttributes.push_back(new QLineEdit());
		ui.vertLayout->insertWidget(ui.vertLayout->count() - 1, elementAttributes[i]);
	}
	tables = Database::instance()->getDatabaseTables();
	for (int i = 0; i < tables.size(); i++) {
		ui.tableDropdown->insertItem(ui.tableDropdown->count() - 1, QString::fromStdString(tables[i]));
	}
}

InsertPage::~InsertPage()
{}

void InsertPage::on_insertButton_clicked() {
	std::vector<std::string> attributeVector;
	sqlGenerator::InsertModel sqlCommand;
	sqlCommand.into(tables.at(ind));
	
	for (int i = 0; i < columns.size(); i ++) {
		QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(ui.vertLayout->itemAt(i)->layout());
		QLabel* label = qobject_cast<QLabel*>(layout->itemAt(0)->widget());//0 - attr name, 1-type 2-input
		QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAt(2)->widget());

		if (label && lineEdit) {
			// Extract the attribute name from the label and user input from line edit
			std::string attributeName = label->text().toStdString();
			std::string attributeValue = lineEdit->text().toStdString();

			// Add the attribute name and value to the vector
			attributeVector.push_back(attributeValue);
			// Add attribute name and value to the insert command
			sqlCommand(attributeName, attributeValue);
		}
	}
	std::string sql = sqlCommand.str();
	if (Database::instance()->sqlExec(sqlCommand.str())) {
		close();
	}
}

void InsertPage::on_tableDropdown_currentIndexChanged(int index) {
	//This is an example of qt being a really stupid library
	if (index == ui.tableDropdown->count() - 1) {
		ind = 0;
	}
	else {
		ind = index + 1;
	}
	columns = Database::instance()->getTableSchema(tables.at(ind));
	//QLabel *schema = new QLabel();
	
	//schema->setText(QString::fromStdString(tables[ind]));
	while (QLayoutItem* item = ui.vertLayout->takeAt(0)) {
		if (QLayout* layout = item->layout()) {
			for (int i = 0; i < 3; i++) {
				QWidget* widget = layout->takeAt(0)->widget();
				delete widget;
			}
		}
		else if (QWidget* widget = item->widget()) {
			delete widget;
		}
		delete item;
	}
	for (int i = 0; i < columns.size(); i++) {
		QHBoxLayout *itemToInsert = new QHBoxLayout;
		QLabel *attrLabel = new QLabel();
		attrLabel->setText(QString::fromStdString(columns[i].name));
		QLabel* attrTypeLabel = new QLabel();
		attrTypeLabel->setText(QString::fromStdString(columns[i].type));
		QLineEdit* itemAttr = new QLineEdit();
		attrLabel->setFixedHeight(20);
		itemToInsert->insertWidget(itemToInsert->count(), attrLabel);
		itemToInsert->insertWidget(itemToInsert->count(), attrTypeLabel);
		itemToInsert->insertWidget(itemToInsert->count(), itemAttr);
		ui.vertLayout->insertLayout(ui.vertLayout->count(), itemToInsert);
	}
	ui.vertLayout->insertSpacerItem(ui.vertLayout->count(), new QSpacerItem(20,20));
	
}

