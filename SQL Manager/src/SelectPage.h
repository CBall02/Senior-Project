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
	Ui::SelectPageClass ui;

private slots:
	void on_tablesComboBox_currentIndexChanged(int index);
};
