#include "JoinWidget.h"
#include <database.h>
#include <QMessageBox>

JoinWidget::JoinWidget(QObject *parent)
	: QObject(parent)
{
	layout = new QHBoxLayout();
	type = new QComboBox();
	tableName = new QComboBox();
	editButton = new QPushButton;
	widget = new QWidget();
	plusButton = new QPushButton("+");
	minusButton = new QPushButton("-");

	plusButton->setMaximumSize(41, 41);
	minusButton->setMaximumSize(41, 41);
	QFont font = plusButton->font();
	font.setPointSize(16);
	plusButton->setFont(font);
	minusButton->setFont(font);

	//ui->pushButton->setStyleSheet("QPushButton{font-size: 12px;font-family: Arial;color: rgb(255, 255, 255);background-color: rgb(38,56,76);}");

	type->addItem("INNER JOIN");
	type->addItem("LEFT JOIN");
	type->addItem("RIGHT JOIN");
	type->addItem("FULL JOIN");
	type->addItem("NATURAL JOIN");
	
	tableName->clear();
	std::vector<std::string> tables = Database::instance()->getDatabaseTables();
	for (std::string name : tables)
	{
		tableName->addItem(QString::fromStdString(name));
	}

	editButton->setText("Edit Conditions");

	layout->addWidget(type);
	layout->addWidget(tableName);
	layout->addWidget(editButton);
	widget->setLayout(layout);

	connect(editButton, &QPushButton::clicked, this, [this]() { emit showJoinConditions(this);  });
	connect(plusButton, &QPushButton::clicked, this, [this]() { plusButtonClicked(); });
	connect(minusButton, &QPushButton::clicked, this, [this]() { minusButtonClicked();  });
	connect(tableName, &QComboBox::currentIndexChanged, this, [this]() { emit showChangedAttributes();  });
}

JoinWidget::~JoinWidget()
{
	delete type;
	delete tableName;
	delete editButton;
	delete plusButton;
	delete minusButton;
	delete layout;
	delete widget;

	for (int i = 0; i < conditionsList.size(); i++) {
		delete conditionsList[i];
	}
}

int JoinWidget::getNumConditions() {
	return conditionsList.size();
}
ConditionWidget* JoinWidget::getConditionAt(int index) {
	if (index <= conditionsList.size() || index >= 0) {
		return conditionsList[index];
	}
	else { return NULL; }
	
}
QString JoinWidget::getType() {
	return type->currentText();
}
QString JoinWidget::getTableName() {
	return tableName->currentText();
}
//QPushButton* JoinWidget::getEditButton(){}
QWidget* JoinWidget::getWidget() {
	return widget;
}
QPushButton* JoinWidget::getPlusButton() {
	return plusButton;

}
QPushButton* JoinWidget::getMinusButton() {
	return minusButton;
}

void JoinWidget::plusButtonClicked(){
	ConditionWidget* condition = new ConditionWidget(this);
	condition->setLabel("ON");
	conditionsList.push_back(condition);
	emit showJoinConditions(this);
}
void JoinWidget::minusButtonClicked() {
	if (conditionsList.size() > 0) {
		delete conditionsList[conditionsList.size() - 1];
		conditionsList.pop_back();
		emit showJoinConditions(this);
	}
}

void JoinWidget::on_tableName_currentIndexChanged(int index) {
	QMessageBox msg;
	msg.setText("update attributes for ");
	msg.exec();
	emit showChangedAttributes();
}