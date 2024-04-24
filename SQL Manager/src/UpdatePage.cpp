#include "UpdatePage.h"
#include <qmessagebox.h>
#include <sqlGenerator.h>

UpdatePage::UpdatePage(QWidget *parent)
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
    ui.updateButton->setStyleSheet(cs1);
    ui.conditionsScrollArea->widget()->setLayout(ui.ConditionsVLayout);
    ui.ConditionsVLayout->setAlignment(Qt::AlignTop);
    ui.setScrollArea->widget()->setLayout(ui.SetVLayout);
    ui.SetVLayout->setAlignment(Qt::AlignTop);
    loadTablesComboBox();
    addCondition(); 
}

UpdatePage::~UpdatePage()
{}

void UpdatePage::loadTablesComboBox()
{
    ui.tablesComboBox->clear();
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    for (std::string name : tables)
    {
        ui.tablesComboBox->addItem(QString::fromStdString(name));
    }
}

void UpdatePage::on_conditionsPlusButton_clicked()
{
    addCondition();
}

void UpdatePage::on_conditionsMinusButton_clicked()
{
    if (numConditions > 1)
    {
        delete conditions.back();
        conditions.pop_back();
        delete labels.back();
        labels.pop_back();
        delete ui.conditionsScrollArea->widget()->layout()->takeAt(ui.conditionsScrollArea->widget()->layout()->count() - 1);
        numConditions--;
    }
}

void UpdatePage::on_setPlusButton_clicked()
{
    addSet();
}

void UpdatePage::on_setMinusButton_clicked()
{
    if (numSetStatements > 1)
    {
        removeSet();
    }
}

void UpdatePage::on_updateButton_clicked()
{
    sqlGenerator::UpdateModel sqlCommand;
    std::string tableName = ui.tablesComboBox->currentText().toStdString();
    std::vector<Database::Column> columns = Database::instance()->getTableSchema(tableName);
    sqlCommand.update(tableName);
    for (int i = 0; i < numSetStatements; i++)
    {
        std::string columnName = setColumns.at(i)->currentText().toStdString();
        std::string columnType;
        for (Database::Column col : columns)
        {
            if (col.name == columnName)
            {
                columnType = col.type;
            }
        }
        std::string setStatement = setStatements.at(i)->text().toStdString();
        if (columnType == "number")
        {
            try
            {
                sqlCommand.set(columnName, std::stoi(setStatement));
            }
            catch (std::invalid_argument)
            {
                std::string errorMessage;
                errorMessage = "Argument \"" + columnName + "\" should have type: " + columnType;
                QMessageBox messageBox;
                messageBox.setText(QString::fromStdString(errorMessage));
                messageBox.exec();
                return;
            }
        }
        else if (columnType == "int")
        {
            try
            {
                sqlCommand.set(columnName, std::stoi(setStatement));
            }
            catch (std::invalid_argument)
            {
                std::string errorMessage;
                errorMessage = "Argument \"" + columnName + "\" should have type: " + columnType;
                QMessageBox messageBox;
                messageBox.setText(QString::fromStdString(errorMessage));
                messageBox.exec();
                return;
            }
        }
        else if (columnType == "float")
        {
            try
            {
                sqlCommand.set(columnName, std::stof(setStatement));
            }
            catch (std::invalid_argument)
            {
                std::string errorMessage;
                errorMessage = "Argument \"" + columnName + "\" should have type: " + columnType;
                QMessageBox messageBox;
                messageBox.setText(QString::fromStdString(errorMessage));
                messageBox.exec();
                return;
            }
        }
        else
        {
            sqlCommand.set(columnName, setStatement);
        }
    }

    for (int i = 0; i < numConditions; i++)
    {
        sqlCommand.where(conditions.at(i)->text().toStdString());
    }
    if (auto result = Database::instance()->sqlExec(sqlCommand.str()))
    {
        emit tableUpdated(sqlCommand.str());
        close();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(result.what()));
        msgBox.exec();
    }
}

void UpdatePage::addCondition()
{
    QHBoxLayout* newLine = new QHBoxLayout();
    QLabel* WHERE = new QLabel();
    QLineEdit* condition = new QLineEdit();
    QWidget* attribute = new QWidget();

    WHERE->setText("WHERE");
    newLine->addWidget(WHERE);
    newLine->addWidget(condition);

    conditions.push_back(condition);
    labels.push_back(WHERE);

    attribute->setLayout(newLine);
    attribute->setFixedHeight(40);
    attribute->setFixedWidth(400);
    ui.conditionsScrollArea->widget()->layout()->addWidget(attribute);

    numConditions++;
}

void UpdatePage::addSet()
{
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    if (tables.empty()) {
        return;
    }
    QHBoxLayout* newLine = new QHBoxLayout();
    QComboBox* column = new QComboBox();
    QLineEdit* condition = new QLineEdit();
    QWidget* attribute = new QWidget();

    newLine->addWidget(column);
    newLine->addWidget(condition);

    int index = ui.tablesComboBox->currentIndex();
    std::vector<Database::Column> columns = Database::instance()->getTableSchema(tables.at(index));
    for (Database::Column _column : columns)
    {
        column->addItem(QString::fromStdString(_column.name));
    }

    setStatements.push_back(condition);
    setColumns.push_back(column);
    
    attribute->setLayout(newLine);
    attribute->setFixedHeight(40);
    attribute->setFixedWidth(400);
    ui.setScrollArea->widget()->layout()->addWidget(attribute);

    numSetStatements++;
}

void UpdatePage::removeSet()
{
    delete setStatements.back();
    setStatements.pop_back();
    delete setColumns.back();
    setColumns.pop_back();
    delete ui.setScrollArea->widget()->layout()->takeAt(ui.setScrollArea->widget()->layout()->count() - 1);
    numSetStatements--;
}

void UpdatePage::on_tablesComboBox_currentIndexChanged(int index)
{
    while (numSetStatements > 0)
    {
        removeSet();
    }
    addSet();

}