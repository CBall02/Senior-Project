#pragma once

#include <QMainWindow>
#include <qlineedit.h>
#include <vector>
#include "ui_UpdatePage.h"
#include <database.h>

class UpdatePage : public QMainWindow
{
	Q_OBJECT

public:
	UpdatePage(QWidget *parent = nullptr);
	~UpdatePage();

signals:
	void tableUpdated(std::string sqlCommand);

private:
	void loadTablesComboBox();
	void addSet();
	void removeSet();
	void addCondition();

	std::vector<QLineEdit*> setStatements;
	std::vector<QComboBox*> setColumns;
	std::vector<QLineEdit*> conditions;
	std::vector<QLabel*> labels;
	int numSetStatements = 0;
	int numConditions = 0;
	Ui::UpdatePageClass ui;

private slots:

	void on_tablesComboBox_currentIndexChanged(int index);
	void on_setPlusButton_clicked();
	void on_setMinusButton_clicked();
	void on_conditionsPlusButton_clicked();
	void on_conditionsMinusButton_clicked();
	void on_updateButton_clicked();
};
