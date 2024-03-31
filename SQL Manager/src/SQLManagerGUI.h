#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SQLManagerGUI.h"
#include "database.h"

class SQLManagerGUI : public QMainWindow
{
    Q_OBJECT

public:
    SQLManagerGUI(QWidget *parent = nullptr);
    ~SQLManagerGUI();  

private:
    Ui::SQLManagerGUIClass ui;
    QString databaseFilepath;

    void loadTable(QString tableName);
    void loadTablesComboBox();
    void addToTablesComboBox();
    void loadTablesListView();

private slots:
    void on_createButton_clicked();
    void on_insertButton_clicked();
    void on_selectButton_clicked();
    void on_actionOpen_triggered();
    void on_loadTableButton_clicked();
    void on_tableCreated();
};
