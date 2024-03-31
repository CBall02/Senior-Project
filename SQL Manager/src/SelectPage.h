#pragma once

#include <QDialog>
#include "ui_SelectPage.h"
#include "database.h"
#include <sqlGenerator.h>

class SelectPage : public QDialog
{
	Q_OBJECT

public:
	SelectPage(QWidget *parent = nullptr);
	~SelectPage();

private:

	void loadTablesComboBox();
	Ui::SelectPageClass ui;

private slots:
	void on_loadTablesComboBox_currentIndexChanged(int index);
};
