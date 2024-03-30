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
    void loadTablesListView();

private slots:
    void on_createButton_clicked();
    void on_insertButton_clicked();
    void on_actionOpen_triggered();
    void loadTableToMain();
    void on_tableCreated();
    void dropTable(QString tableName);
    void on_commandPromptInputLineEdit_returnPressed();
    void popupTablesContextMenu(QPoint pos);
};
