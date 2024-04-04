#include "SQLManagerGUI.h"
#include "ui_SQLManagerGUI.h"
#include "CreatePage.h"
#include "InsertPage.h"
#include "SelectPage.h"


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

    ui.tableWidget->setHorizontalHeaderLabels(labels);
    ui.tableWidget->setColumnCount(table->numFields());

    int i = 0;
    while (!table->eof()) {
        
        for (int j = 0; j < table->numFields(); j++) {
            ui.tableWidget->setItem(i, j, new QTableWidgetItem(QString::fromStdString(table->fieldValue(j))));
        }
        table->nextRow();
        i++;
    }
}

void SQLManagerGUI::loadTablesListView() {
    std::vector<std::string> tablesVect = Database::instance()->getDatabaseTables();
    QVector<QString> tables;
    std::transform(tablesVect.begin(), tablesVect.end(), std::back_inserter(tables), [](const std::string& v) { return QString::fromStdString(v); });
    ui.tablesListView->setModel(new QStringListModel(tables));
}


//Slots
void SQLManagerGUI::on_createButton_clicked() {
    CreatePage* createPg = new CreatePage();
    createPg->setModal(true);
    createPg->show();
    //createPg->exec();
    connect(createPg, &CreatePage::tableCreated, this, &SQLManagerGUI::on_tableCreated);
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

void SQLManagerGUI::on_actionOpen_triggered() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "C:/", tr("Database Files (*.db)"));
    databaseFilepath = fileNames.join("");

    bool opened = Database::instance()->openDatabase(databaseFilepath.toStdString());
    if (opened) {
        loadTablesListView();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Error opening database.");
        msgBox.exec();
    }
}

void SQLManagerGUI::loadTableToMain() {
    QModelIndex index = ui.tablesListView->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    loadTable(itemText);
}
void SQLManagerGUI::on_tableCreated() {
    loadTablesListView();
}

void SQLManagerGUI::dropTable(QString tableName) {

    QMessageBox msgBox;
    msgBox.setText("drop table: " + tableName);
    msgBox.exec();

    sqlGenerator::DropModel sqlCommand;
    sqlCommand.tableName(tableName.toStdString());
    
    if (Database::instance()->sqlExec(sqlCommand.str()))
    {
        qDebug() << "Successful Drop";
        loadTablesListView();
    }
    else
    {
        qDebug() << "Successful Drop";
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
        auto table = Database::instance()->queryDatabase(input.toStdString());
        if (table) {
            loadQueryOutput(table);
        }
        else {
            ui.commandPromptOutputTextEdit->append(QString::fromStdString(table.what()));
        }
    }
    else {
        auto cmdSucceeded = Database::instance()->sqlExec(input.toStdString());
        if (cmdSucceeded)
        {
            if (firstWord.toUpper() == "CREATE" || firstWord.toUpper() == "DROP") {
                loadTablesListView();
            }
        }
        else {
            ui.commandPromptOutputTextEdit->append(QString::fromStdString(cmdSucceeded.what()));
        }
    }
    
    
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