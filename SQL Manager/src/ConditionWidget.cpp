#include "ConditionWidget.h"

ConditionWidget::ConditionWidget(QObject *parent)
	: QObject(parent)
{
	layout = new QHBoxLayout();
	label = new QLabel();
	condition = new QLineEdit();
	widget = new QWidget();

	layout->addWidget(label);
	layout->addWidget(condition);
	widget->setLayout(layout);
}

ConditionWidget::~ConditionWidget()
{
	delete label;
	delete condition;
	delete layout;
	delete widget;
}

void ConditionWidget::setLabel(QString text) {
	label->setText(text);
}
QString ConditionWidget::getCondition() {
	return condition->text();
}
QWidget* ConditionWidget::getWidget() {
	return widget;
}
