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

    QString cs1 = "QPushButton {"
        "background-color: rgb(210, 226, 231);"
        "color: rgb(0, 0, 0);"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(255, 255, 255);"
        "color: rgb(0, 0, 0);"
        "}";
    ui.createButton->setStyleSheet(cs1);
    ui.insertButton->setStyleSheet(cs1);
    ui.selectButton->setStyleSheet(cs1);
    ui.updateButton->setStyleSheet(cs1);
    ui.deleteButton->setStyleSheet(cs1);

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
    int result;
    if (databaseFilepath == "" && showNoFileMessageBox) {
        QCheckBox* cb = new QCheckBox("Don't show this again.");
        QMessageBox msgbox;
        msgbox.setText("No database file is currently opened. Using this command will create a table in a database in memory. Do you want to proceed?");
        msgbox.setIcon(QMessageBox::Icon::Question);
        msgbox.addButton(QMessageBox::Ok);
        msgbox.addButton(QMessageBox::Cancel);
        msgbox.setDefaultButton(QMessageBox::Cancel);
        msgbox.setCheckBox(cb);

        QObject::connect(cb, &QCheckBox::stateChanged, [this](int state) {
            if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked) {
                this->showNoFileMessageBox = false;
            }
            });

        result = msgbox.exec();

        if (result == QMessageBox::Ok) {
            CreatePage* createPg = new CreatePage();
            createPg->setModal(true);
            createPg->show();
            //createPg->exec();
            connect(createPg, &CreatePage::tableCreated, this, [this]() {loadTablesListView(); });
            connect(createPg, &CreatePage::tableCreated, this, &SQLManagerGUI::sqlCommandExecuted);
        }
    }
    else {
        CreatePage* createPg = new CreatePage();
        createPg->setModal(true);
        createPg->show();
        //createPg->exec();
        connect(createPg, &CreatePage::tableCreated, this, [this]() {loadTablesListView(); });
        connect(createPg, &CreatePage::tableCreated, this, &SQLManagerGUI::sqlCommandExecuted);
    }
}

void SQLManagerGUI::on_insertButton_clicked() {
    if (Database::instance()->getDatabaseTables().size() > 0) {
        InsertPage insertPg;
        insertPg.setModal(true);
        insertPg.exec();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("No tables in database. Create tables to use this command.");
        msgBox.exec();
    }
}

void SQLManagerGUI::on_selectButton_clicked() {
    if (Database::instance()->getDatabaseTables().size() > 0) {
        SelectPage* selectPg = new SelectPage();
        selectPg->setModal(true);
        selectPg->show();
        connect(selectPg, &SelectPage::selectCommandRequested, this, &SQLManagerGUI::performSelectCommand);
        connect(this, &SQLManagerGUI::selectCommandSuccessful, this, [this, selectPg]() {selectPg->close(); });
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("No tables in database. Create tables to use this command.");
        msgBox.exec();
    }
}

void SQLManagerGUI::on_updateButton_clicked() {
    if (Database::instance()->getDatabaseTables().size() > 0) {
    UpdatePage* updatePg = new UpdatePage();
    updatePg->show();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("No tables in database. Create tables to use this command.");
        msgBox.exec();
    }
}

void SQLManagerGUI::on_deleteButton_clicked() {
    if (Database::instance()->getDatabaseTables().size() > 0) {
        DeletePage* deletePg = new DeletePage();
        deletePg->show();
        connect(deletePg, &DeletePage::tableDeleted, this, &SQLManagerGUI::sqlCommandExecuted);
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("No tables in database. Create tables to use this command.");
        msgBox.exec();
    }
}

void SQLManagerGUI::on_actionNew_triggered() {
    databaseFilepath = QFileDialog::getSaveFileName(this, tr("Create New File"), "C:/", tr("Database Files (*.db)"));
    QFile file(databaseFilepath);
    FWDErrorReturn<bool> openResult = Database::instance()->openDatabase(databaseFilepath.toStdString());
    if (openResult) {
        loadTablesListView();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(openResult.what()));
        msgBox.exec();
    }
}

void SQLManagerGUI::on_actionOpen_triggered() {
    databaseFilepath = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Database Files (*.db)"));
    FWDErrorReturn<bool> openResult = Database::instance()->openDatabase(databaseFilepath.toStdString());
    if (openResult) {
        loadTablesListView();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(openResult.what()));
        msgBox.exec();
    }
}

void SQLManagerGUI::on_actionClose_triggered() {
     
    if (Database::instance()->closeDatabase()) {
        loadTablesListView();
        databaseFilepath = "";
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Error closing database.");
        msgBox.exec();
    }
}

void SQLManagerGUI::on_commandPromptInputLineEdit_returnPressed() {
    QString input = ui.commandPromptInputLineEdit->text();
    ui.commandPromptOutputTextEdit->append("> " + input);
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

void SQLManagerGUI::performSelectCommand(std::string sqlCommand) {
    if (auto result = Database::instance()->queryDatabase(sqlCommand)) {
        ui.commandPromptOutputTextEdit->append(QString::fromStdString("> " + sqlCommand + ";"));
        loadQueryOutput(result);
        emit selectCommandSuccessful();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(result.what()));
        msgBox.exec();
    }
}

void SQLManagerGUI::sqlCommandExecuted(std::string sqlCommand) {
    ui.commandPromptOutputTextEdit->append(QString::fromStdString("> " + sqlCommand + ";"));
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