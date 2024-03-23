#include "SQLManagerGUI.h"
#include "ui_SQLManagerGUI.h"
#include "CreatePage.h"
#include "InsertPage.h"


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
