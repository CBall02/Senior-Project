#include "SQLManagerGUI.h"
#include "ui_SQLManagerGUI.h"
#include "CreatePage.h"
#include "InsertPage.h"
#include "SelectPage.h"
#include "UpdatePage.h"
#include "DeletePage.h"

#include "database.h"
#include "sqlGenerator.h"
#include "databaseReturn.h"
#include "CppSQLite3.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>
#include<QAbstractItemView>

#include <sstream>
#include <iostream>
#include <string>
#include <exception>

SQLManagerGUI::SQLManagerGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.commandPromptOutputTextEdit->setReadOnly(true);
    ui.splitter->setSizes(QList<int>() << 100 << 500);
    ui.splitter_2->setSizes(QList<int>() << 300 << 100);
    ui.tablesListView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.tablesListView, &QListView::customContextMenuRequested, this, &SQLManagerGUI::popupTablesContextMenu);
    connect(ui.tablesListView, &QAbstractItemView::clicked, this, &SQLManagerGUI::loadTableToMain);
}

SQLManagerGUI::~SQLManagerGUI()
{}

//Functions
void SQLManagerGUI::loadTable(QString tableName) {
    auto table = Database::instance()->getTable(tableName.toStdString());
    
    QStringList labels;
    for (int i = 0; i < table->numFields(); i++) {
        labels << table->fieldName(i);
    }
    ui.tableWidget->setRowCount(0);
    ui.tableWidget->setColumnCount(table->numFields());
    ui.tableWidget->setRowCount(table->numRows());
    ui.tableWidget->setHorizontalHeaderLabels(labels);

    for (int i = 0; i < table->numRows(); i++) {
        table->setRow(i);
        for (int j = 0; j < table->numFields(); j++) {
            ui.tableWidget->setItem(i, j, new QTableWidgetItem(QString::fromStdString(table->fieldValue(j))));
        }
    }
}

void SQLManagerGUI::loadQueryOutput(FWDErrorReturn<CppSQLite3Query> table) {
    QStringList labels;
    for (int i = 0; i < table->numFields(); i++) {
        labels << table->fieldName(i);
    }

    ui.tableWidget->setRowCount(0);
    ui.tableWidget->setColumnCount(table->numFields());
    ui.tableWidget->setHorizontalHeaderLabels(labels);

    while (!table->eof()) {
        ui.tableWidget->insertRow(ui.tableWidget->rowCount());

        for (int j = 0; j < table->numFields(); j++) {
            ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, j, new QTableWidgetItem(QString::fromStdString(table->fieldValue(j))));
        }
        table->nextRow();
    }
}

void SQLManagerGUI::loadTablesListView() {
    std::vector<std::string> tablesVect = Database::instance()->getDatabaseTables();
    QVector<QString> tables;
    std::transform(tablesVect.begin(), tablesVect.end(), std::back_inserter(tables), [](const std::string& v) { return QString::fromStdString(v); });
    ui.tablesListView->setModel(new QStringListModel(tables));
}

void SQLManagerGUI::displayError(FWDErrorReturn<CppSQLite3Query> &result) {
    ui.commandPromptOutputTextEdit->setTextColor(QColor(Qt::red));
    ui.commandPromptOutputTextEdit->append(QString::fromStdString(result.what()));
    ui.commandPromptOutputTextEdit->setTextColor(QColor(Qt::black));
}
void SQLManagerGUI::displayError(FWDErrorReturn<bool> &result) {
    ui.commandPromptOutputTextEdit->setTextColor(QColor(Qt::red));
    ui.commandPromptOutputTextEdit->append(QString::fromStdString(result.what()));
    ui.commandPromptOutputTextEdit->setTextColor(QColor(Qt::black));
}


//Slots
void SQLManagerGUI::on_createButton_clicked() {
    CreatePage* createPg = new CreatePage();
    createPg->setModal(true);
    createPg->show();
    //createPg->exec();
    connect(createPg, &CreatePage::tableCreated, this, &SQLManagerGUI::updateTableCreated);
    connect(createPg, &CreatePage::tableCreated, this, &SQLManagerGUI::sqlCommandExecuted);
}

void SQLManagerGUI::on_insertButton_clicked() {
    InsertPage insertPg;
    insertPg.setModal(true);
    insertPg.exec();
}

void SQLManagerGUI::on_selectButton_clicked() {
    SelectPage* selectPg = new SelectPage();
    selectPg->show();
}

void SQLManagerGUI::on_updateButton_clicked() {
    UpdatePage* updatePg = new UpdatePage();
    updatePg->show();
}

void SQLManagerGUI::on_deleteButton_clicked() {
    DeletePage* deletePg = new DeletePage();
    deletePg->show();
    connect(deletePg, &DeletePage::tableDeleted, this, &SQLManagerGUI::sqlCommandExecuted);
}

void SQLManagerGUI::on_actionOpen_triggered() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "C:/", tr("Database Files (*.db)"));
    databaseFilepath = fileNames.join("");

    FWDErrorReturn<bool> openResult= Database::instance()->openDatabase(databaseFilepath.toStdString());
    if (openResult) {
        loadTablesListView();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(openResult.what()));
        msgBox.exec();
    }
}

void SQLManagerGUI::on_commandPromptInputLineEdit_returnPressed() {
    QString input = ui.commandPromptInputLineEdit->text();
    ui.commandPromptOutputTextEdit->append(input);
    ui.commandPromptInputLineEdit->clear();

    std::stringstream ss(input.toStdString());
    std::string temp = "";
    getline(ss, temp, ' ');

    QTextStream s(&input);
    QString firstWord;
    s >> firstWord;

    if (firstWord.toUpper() == "SELECT") {
        FWDErrorReturn<CppSQLite3Query> queryResult= Database::instance()->queryDatabase(input.toStdString());
        if (queryResult) {
            loadQueryOutput(queryResult);
        }
        else {
            displayError(queryResult);
        }
    }
    else {
        FWDErrorReturn<bool> cmdResult= Database::instance()->sqlExec(input.toStdString());
        if (cmdResult)
        {
            if (firstWord.toUpper() == "CREATE" || firstWord.toUpper() == "DROP") {
                loadTablesListView();
            }
        }
        else {
            displayError(cmdResult);
        }
    }
}

void SQLManagerGUI::loadTableToMain() {
    QModelIndex index = ui.tablesListView->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    loadTable(itemText);
}
void SQLManagerGUI::updateTableCreated(std::string sqlCommand) {
    loadTablesListView();
}

void SQLManagerGUI::sqlCommandExecuted(std::string sqlCommand) {
    ui.commandPromptOutputTextEdit->append(QString::fromStdString(sqlCommand));
}

void SQLManagerGUI::dropTable(QString tableName) {
    QMessageBox::StandardButton warnMsg;
    warnMsg = QMessageBox::warning(this, "Drop Table", "You are attempting to drop table \"" + tableName + ".\"\nThis action will delete the table permanently.\nDo you want to proceed?",
        QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);

    if (warnMsg == QMessageBox::Yes) {
        sqlGenerator::DropModel sqlCommand;
        sqlCommand.tableName(tableName.toStdString());

        if (FWDErrorReturn<bool> dropResult = Database::instance()->sqlExec(sqlCommand.str()))
        {
            loadTablesListView();
            ui.commandPromptOutputTextEdit->append(QString::fromStdString(sqlCommand.str()));
        }
        else
        {
            displayError(dropResult);
        }
    }
    else {}
}

void SQLManagerGUI::popupTablesContextMenu(QPoint pos) {
    QModelIndex index = ui.tablesListView->indexAt(pos);
    QMenu* menu = new QMenu(this);

    QAction* action1 = new QAction("Drop Table", this);
    menu->addAction(action1);
    menu->popup(ui.tablesListView->viewport()->mapToGlobal(pos));

    QModelIndex in = ui.tablesListView->currentIndex();
    QString itemText = in.data(Qt::DisplayRole).toString();

    connect(action1, &QAction::triggered, [this, itemText]() {emit dropTable(itemText); });
}