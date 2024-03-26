#pragma once

#include <QDialog>
#include <vector>
#include "ui_CreatePage.h"
#include "database.h"

class CreatePage : public QDialog
{
	Q_OBJECT

public:
	CreatePage(QWidget *parent = nullptr);
	~CreatePage();

signals:
	void tableCreated();

private:
	Ui::CreatePageClass ui;
	int numAttributes = 1;
	std::vector<QLineEdit*> table;


private slots:
	void on_plusButton_clicked();
	void on_createButton_clicked();
	void on_minusButton_clicked();
};
