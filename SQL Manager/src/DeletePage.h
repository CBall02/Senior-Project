#pragma once

#include <QMainWindow>
#include <qlineedit.h>
#include <vector>
#include "ui_DeletePage.h"
#include "database.h"

class DeletePage : public QMainWindow
{
	Q_OBJECT

public:
	DeletePage(QWidget *parent = nullptr);
	~DeletePage();

signals:
	void tableDeleted(std::string sqlCommand);

private:
	void loadTablesComboBox();
	void addCondition();

	std::vector<QLineEdit*> conditions;
	std::vector<QLabel*> labels;
	int numConditions = 0;
	Ui::DeletePageClass ui;

private slots:
	void on_plusButton_clicked();
	void on_minusButton_clicked();
	void on_deleteButton_clicked();
};
