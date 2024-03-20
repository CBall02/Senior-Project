#pragma once

#include <QMainWindow>
#include "ui_CreatePage.h"

class CreatePage : public QMainWindow
{
	Q_OBJECT

public:
	CreatePage(QWidget *parent = nullptr);
	~CreatePage();

private:
	Ui::CreatePageClass ui;
};
