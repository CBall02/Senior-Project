#include "InsertPage.h"
#include <qmessagebox.h>
#include <sqlGenerator.h>

InsertPage::InsertPage(QWidget* parent)
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
    ui.insertButton->setStyleSheet(cs1);
    ui.scrollArea->widget()->setLayout(ui.insertLayout);
    ui.insertLayout->setAlignment(Qt::AlignTop);
    loadTablesComboBox();
}

InsertPage::~InsertPage()
{}


void InsertPage::loadTablesComboBox()
{
    ui.tablesComboBox->clear();
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    for (std::string name : tables)
    {
        ui.tablesComboBox->addItem(QString::fromStdString(name));
    }
}

void InsertPage::on_tablesComboBox_currentIndexChanged(int index)
{
    while (numColumns > 0)
    {
        delete columnNames.back();
        columnNames.pop_back();
        delete columnTypes.back();
        columnTypes.pop_back();
        delete columns.back();
        columns.pop_back();
        delete ui.scrollArea->widget()->layout()->takeAt(ui.scrollArea->widget()->layout()->count() - 1);
        numColumns--;
    }
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    std::vector<Database::Column> insertColumns = Database::instance()->getTableSchema(tables.at(index));
    for (Database::Column col : insertColumns)
    {
        QHBoxLayout* newLine = new QHBoxLayout();
        QLabel* name = new QLabel();
        QLabel* type = new QLabel();
        QLineEdit* insert = new QLineEdit();
        QWidget* widget = new QWidget();

        name->setText(QString::fromStdString(col.name));
        type->setText(QString::fromStdString(col.type));
        newLine->addWidget(name);
        newLine->addWidget(type);
        newLine->addWidget(insert);

        columnNames.push_back(name);
        columnTypes.push_back(type);
        columns.push_back(insert);

        insert->setFixedWidth(200);
        widget->setLayout(newLine);
        widget->setFixedHeight(40);
        widget->setFixedWidth(400);
        ui.scrollArea->widget()->layout()->addWidget(widget);

        numColumns++;
    }
}

void InsertPage::on_insertButton_clicked()
{
    sqlGenerator::InsertModel sqlCommand;
    std::string tableName = ui.tablesComboBox->currentText().toStdString();
    sqlCommand.into(tableName);
    for (int i = 0; i < columns.size(); i++)
    {
        std::string columnName = columnNames.at(i)->text().toStdString();
        std::string columnType = columnTypes.at(i)->text().toStdString();
        std::string input = columns.at(i)->text().toStdString();
        if (columnType == "number")
        {
            try
            {
                sqlCommand.insert(columnName, std::stoi(input));
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
                sqlCommand.insert(columnName, std::stof(input));
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
            sqlCommand.insert(columnName, input);
        }
    }
    if (Database::instance()->sqlExec(sqlCommand.str()))
    {
        emit dataInserted(sqlCommand.str());
        close();
    }
}