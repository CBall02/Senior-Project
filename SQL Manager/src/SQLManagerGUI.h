#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SQLManagerGUI.h"
#include "database.h"
#include "databaseReturn.h"

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
    void loadQueryOutput(FWDErrorReturn<CppSQLite3Query> table);
    void loadTablesListView();
    void displayError(FWDErrorReturn<CppSQLite3Query> &result);
    void displayError(FWDErrorReturn<bool> &result);

private slots:
    void on_createButton_clicked();
    void on_insertButton_clicked();
    void on_selectButton_clicked();
    void on_actionOpen_triggered();
    void on_commandPromptInputLineEdit_returnPressed();

    void loadTableToMain();
    void updateTableCreated(std::string sqlCommand);
    void dropTable(QString tableName);
    void popupTablesContextMenu(QPoint pos);
};
