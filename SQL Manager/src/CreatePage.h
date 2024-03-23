#pragma once

#include <QDialog>
#include "ui_CreatePage.h"

class CreatePage : public QDialog
{
	Q_OBJECT

public:
	CreatePage(QWidget *parent = nullptr);
	~CreatePage();

private:
	Ui::CreatePageClass ui;
};
