#pragma once

#include <QDialog>
#include "ui_InsertPage.h"
#include "database.h"


class InsertPage : public QDialog
{
	Q_OBJECT

public:
	InsertPage(QWidget *parent = nullptr);
	~InsertPage();

private:
	Ui::InsertPageClass ui;
	int numAttributes = 1;
	std::vector<QLineEdit*> elementAttributes;
public slots:
	void on_insertButton_clicked();
	void on_tableDropdown_currentIndexChanged(int index);
};
