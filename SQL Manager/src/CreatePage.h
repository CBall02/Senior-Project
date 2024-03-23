#pragma once

#include <QDialog>
#include "ui_CreatePage.h"
#include "../../Backend/src/database.h"

class CreatePage : public QDialog
{
	Q_OBJECT

public:
	CreatePage(QWidget *parent = nullptr);
	~CreatePage();

private:
	Ui::CreatePageClass ui;
	int numAttributes = 1;


private slots:
	void on_plusButton_clicked();
	void on_createButton_clicked();
};
