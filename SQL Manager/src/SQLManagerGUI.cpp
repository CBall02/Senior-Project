#include "SQLManagerGUI.h"
#include "ui_SQLManagerGUI.h"
#include "database.h"
#include "CreatePage.h"
#include "InsertPage.h"
#include <QFileDialog>
#include <QMessageBox>

SQLManagerGUI::SQLManagerGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

SQLManagerGUI::~SQLManagerGUI()
{}

void SQLManagerGUI::on_createButton_clicked() {
    CreatePage createPg;
    createPg.setModal(true);
    createPg.exec();
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
}
