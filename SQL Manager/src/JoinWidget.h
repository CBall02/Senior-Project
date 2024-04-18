#pragma once

#include <QObject>
#include <QComboBox>
#include <QPushButton>
#include "ConditionWidget.h"

class JoinWidget  : public QObject
{
	Q_OBJECT

public:
	JoinWidget(QObject *parent);
	~JoinWidget();

	int getNumConditions();
	ConditionWidget* getConditionAt(int index);
	QString getType();
	QString getTableName();
	//QPushButton* getEditButton();
	QWidget* getWidget();
	QPushButton* getPlusButton();
	QPushButton* getMinusButton();

	signals:
	void showJoinConditions(JoinWidget* join);

private:
	QHBoxLayout* layout;
	QComboBox* type;
	QComboBox* tableName;
	QPushButton* editButton;
	QWidget* widget;
	QPushButton* plusButton;
	QPushButton* minusButton;

	std::vector<ConditionWidget*> conditionsList;

	void plusButtonClicked();
	void minusButtonClicked();
};
