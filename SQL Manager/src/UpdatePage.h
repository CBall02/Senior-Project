#pragma once

#include <QMainWindow>
#include "ui_UpdatePage.h"

class UpdatePage : public QMainWindow
{
	Q_OBJECT

public:
	UpdatePage(QWidget *parent = nullptr);
	~UpdatePage();

private:
	Ui::UpdatePageClass ui;
};
