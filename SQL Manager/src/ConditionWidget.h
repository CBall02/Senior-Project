#pragma once

#include <QObject>
#include <QLineEdit>
#include <qboxlayout.h>
#include <QLabel>

class ConditionWidget  : public QObject
{
	Q_OBJECT

public:
	ConditionWidget(QObject *parent);
	~ConditionWidget();
	void setLabel(QString text);
	QString getCondition();
	QWidget* getWidget();
private:
	QHBoxLayout* layout;
	QLabel* label;
	QLineEdit* condition;
	QWidget* widget;
	

};
