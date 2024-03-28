#include "SQLManagerGUI.h"
#include "ui_SQLManagerGUI.h"
#include "database.h"
#include "CreatePage.h"
#include "InsertPage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>

SQLManagerGUI::SQLManagerGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

SQLManagerGUI::~SQLManagerGUI()
{}

void SQLManagerGUI::loadTable(QString tableName) {
    QString tableStr = "";
    auto table = Database::instance()->getTable(tableName.toStdString());
    for (int i = 0; i < table.result.numFields(); i++) {
        tableStr += table.result.fieldName(i);
        tableStr += "|";
    }
    tableStr += "\n";
    for (int i = 0; i < table.result.numRows(); i++) {
        table.result.setRow(i);
        for (int j = 0; j < table.result.numFields(); j++) {
            tableStr += table.result.fieldValue(j);
            tableStr += "|";
        }
        tableStr += "\n";
    }
    QMessageBox msgBox;
    msgBox.setText(tableStr);
    msgBox.exec();
}

void SQLManagerGUI::loadTablesComboBox() {
    ui.tablesComboBox->clear();
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    for (std::string name : tables) {
        ui.tablesComboBox->addItem(QString::fromStdString(name));
    }
}
void SQLManagerGUI::addToTablesComboBox() {

}
void SQLManagerGUI::loadTablesListView() {
    std::vector<std::string> tablesVect = Database::instance()->getDatabaseTables();
    QVector<QString> tables;
    std::transform(tablesVect.begin(), tablesVect.end(), std::back_inserter(tables), [](const std::string& v) { return QString::fromStdString(v); });
    ui.tablesListView->setModel(new QStringListModel(tables));
}

void SQLManagerGUI::on_createButton_clicked() {
    /*CreatePage createPg;
    createPg.setModal(true);
    createPg.exec();*/
    CreatePage* createPg = new CreatePage();
    createPg->show();
    connect(createPg, &CreatePage::tableCreated, this, &SQLManagerGUI::on_tableCreated);
}

void SQLManagerGUI::on_insertButton_clicked() {
    InsertPage insertPg;
    insertPg.setModal(true);
    insertPg.exec();
}

void SQLManagerGUI::on_actionOpen_triggered() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), "C:/", tr("Database Files (*.db)"));
    databaseFilepath = fileNames.join("");
  
    QMessageBox msgBox;
    msgBox.setText(databaseFilepath);
    msgBox.exec();

    Database::instance()->openDatabase(databaseFilepath.toStdString());

    loadTablesComboBox();
    loadTablesListView();
}

void SQLManagerGUI::on_loadTableButton_clicked() {
    loadTable(ui.tablesComboBox->currentText());
}
void SQLManagerGUI::on_tableCreated() {
    loadTablesComboBox();
    loadTablesListView();
}
