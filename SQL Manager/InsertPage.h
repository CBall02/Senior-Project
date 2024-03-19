#pragma once

#include <QMainWindow>
#include "ui_InsertPage.h"

class InsertPage : public QMainWindow
{
	Q_OBJECT

public:
	InsertPage(QWidget *parent = nullptr);
	~InsertPage();

private:
	Ui::InsertPageClass ui;
};
