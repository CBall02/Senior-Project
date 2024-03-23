#pragma once

#include <QDialog>
#include "ui_InsertPage.h"


class InsertPage : public QDialog
{
	Q_OBJECT

public:
	InsertPage(QWidget *parent = nullptr);
	~InsertPage();

private:
	Ui::InsertPageClass ui;
	int numAttributes = 1;
public slots:
	void on_insertButton_clicked();
};
