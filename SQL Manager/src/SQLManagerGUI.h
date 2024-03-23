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

private slots:
    void on_createButton_clicked();
    void on_insertButton_clicked();
};
