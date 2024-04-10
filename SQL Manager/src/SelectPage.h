#pragma once

#include <QDialog>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <vector>
#include "ui_SelectPage.h"
#include "database.h"
#include <sqlGenerator.h>

class SelectPage : public QDialog
{
	Q_OBJECT

public:
	SelectPage(QWidget* parent = nullptr);
	~SelectPage();

private:

	void loadTablesComboBox();

	std::vector<QCheckBox*> selections;
	std::vector<QLineEdit*> conditions;
	std::vector<QLabel*> labels;
	int numConditions = 0;
	Ui::SelectPageClass ui;

	void addCondition();

private slots:
	void on_tablesComboBox_currentIndexChanged(int index);
	void on_plusButton_clicked();
	void on_minusButton_clicked();
};
