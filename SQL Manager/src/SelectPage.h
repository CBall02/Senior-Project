#pragma once

#include <QDialog>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <vector>
#include "ui_SelectPage.h"
#include "database.h"
#include <sqlGenerator.h>
#include "ConditionWidget.h"
#include "JoinWidget.h"
class SelectPage : public QDialog
{
	Q_OBJECT

public:
	SelectPage(QWidget* parent = nullptr);
	~SelectPage();
signals:
	void selectCommandRequested(std::string sqlCommand);
	void addConditionButtonClicked(int index);
private:

	void loadTablesComboBox(QComboBox* comboBox);

	std::vector<QCheckBox*> selections;
	std::vector<QLineEdit*> whereConditions;
	std::vector<QLabel*> whereLabels;
	std::vector<JoinWidget*> joins;

	int numWhereConditions;
	int numJoins;

	Ui::SelectPageClass ui;

	void addWhereCondition();
	void addJoin();

private slots:
	void on_tablesComboBox_currentIndexChanged(int index);
	void on_whereConditionsPlusButton_clicked();
	void on_whereConditionsMinusButton_clicked();
	void on_joinsPlusButton_clicked();
	void on_joinsMinusButton_clicked();
	void on_selectButton_clicked();
	void displayOnConditions(JoinWidget* join);
};
