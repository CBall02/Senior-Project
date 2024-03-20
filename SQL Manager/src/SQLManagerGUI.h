#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SQLManagerGUI.h"

class SQLManagerGUI : public QMainWindow
{
    Q_OBJECT

public:
    SQLManagerGUI(QWidget *parent = nullptr);
    ~SQLManagerGUI();

private:
    Ui::SQLManagerGUIClass ui;
};
