#pragma once

#include <QDialog>
#include <qcombobox.h>
#include <QCheckBox>
#include <vector>
#include <map>
#include "ui_CreatePage.h"
#include "database.h"

class CreatePage : public QDialog
{
	Q_OBJECT

public:
	CreatePage(QWidget* parent = nullptr);
	~CreatePage();

signals:
	void tableCreated(std::string sqlCommand);

private:
	void addAttribute();

	Ui::CreatePageClass ui;
	int numAttributes = 0;
	std::vector<QLineEdit*> tableNames;
	std::vector<QComboBox*> tableTypes;
	std::vector<QCheckBox*> tableConstraints;
	std::map<QComboBox*, QHBoxLayout*> findLayout;

private slots:
	void on_plusButton_clicked();
	void on_createButton_clicked();
	void on_minusButton_clicked();
	void primaryKeyClicked();
	void uniqueOrNotNullClicked();
	void on_type_changed(int index);
};
