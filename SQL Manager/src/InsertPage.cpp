#include "InsertPage.h"
#include <qlineedit.h>


int numAttributes = 3; //TODO find number of attributes

InsertPage::InsertPage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	for (int i = 0; i < numAttributes; i++) {
		ui.vertLayout->insertWidget(ui.vertLayout->count() - 1, new QLineEdit());
	}
}

InsertPage::~InsertPage()
{}

void InsertPage::on_insertButton_clicked() {
	std::vector<std::string> attributeVector;
	for (int i = 0; i < numAttributes; i++) {
		QLineEdit* q = (QLineEdit*)ui.vertLayout->itemAt(i)->widget();
		
		attributeVector.push_back(q->text().toStdString());
	}
}
