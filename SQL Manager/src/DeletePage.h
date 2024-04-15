#pragma once

#include <QMainWindow>
#include "ui_DeletePage.h"
#include "database.h"

class DeletePage : public QMainWindow
{
	Q_OBJECT

public:
	DeletePage(QWidget *parent = nullptr);
	~DeletePage();

private:
	void loadTablesComboBox();


	Ui::DeletePageClass ui;

private slots:
	void on_plusButton_clicked();
	void on_minusButton_clicked();
	void on_deleteButton_clicked();
};
