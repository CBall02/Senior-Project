#include "SelectPage.h"
#include "SQLManagerGUI.h"
#include <QMessageBox>

SelectPage::SelectPage(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    numWhereConditions = 0;
    numJoins = 0;

    ui.attributesScrollArea->widget()->setLayout(ui.verticalAttributes);
    ui.verticalAttributes->setAlignment(Qt::AlignTop);
    ui.verticalAttributes->setContentsMargins(15, 15, 15, 15);

    ui.whereConditionsScrollArea->widget()->setLayout(ui.verticalWhereConditions);
    ui.verticalWhereConditions->setAlignment(Qt::AlignTop);
    ui.verticalWhereConditions->setContentsMargins(15, 15, 15, 15);

    ui.joinScrollArea->widget()->setLayout(ui.verticalJoins);
    ui.verticalJoins->setAlignment(Qt::AlignTop);
    
    ui.onConditionsScrollArea->widget()->setLayout(ui.verticalOnConditions);
    ui.verticalOnConditions->setAlignment(Qt::AlignTop);
    ui.verticalWhereConditions->setContentsMargins(15, 15, 15, 15);

    loadTablesComboBox(ui.tablesComboBox);
    addWhereCondition();
}

SelectPage::~SelectPage()
{}

void SelectPage::loadTablesComboBox(QComboBox* comboBox)
{
    comboBox->clear();
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    for (std::string name : tables)
    {
        comboBox->addItem(QString::fromStdString(name));
    }
}

void SelectPage::addWhereCondition()
{
    QHBoxLayout* newLine = new QHBoxLayout();
    QLabel* WHERE = new QLabel();
    QLineEdit* condition = new QLineEdit();

    WHERE->setText("WHERE");
    newLine->addWidget(WHERE);
    newLine->addWidget(condition);

    ui.verticalWhereConditions->insertLayout(ui.verticalWhereConditions->count(), newLine);
    whereConditions.push_back(condition);
    whereLabels.push_back(WHERE);
    numWhereConditions++;
}

void SelectPage::addJoin()
{
    JoinWidget* join = new JoinWidget(this);
    ui.verticalJoins->addWidget(join->getWidget());
    joins.push_back(join);
    numJoins++;

    connect(join, &JoinWidget::showJoinConditions, this, &SelectPage::displayOnConditions);
}


void SelectPage::on_tablesComboBox_currentIndexChanged(int index)
{
    while (selections.size() > 0)
    {
        delete selections.back();
        selections.pop_back();
    }
    std::vector<std::string> tables = Database::instance()->getDatabaseTables();
    std::vector<Database::Column> columns = Database::instance()->getTableSchema(tables.at(index));
    for (Database::Column column : columns)
    {
        QCheckBox* attribute = new QCheckBox();
        attribute->setText(QString::fromStdString(column.name));
        selections.push_back(attribute);
        ui.attributesScrollArea->widget()->layout()->addWidget(attribute);
    }
}

void SelectPage::on_whereConditionsPlusButton_clicked()
{
    addWhereCondition();
}

void SelectPage::on_whereConditionsMinusButton_clicked()
{
    if (numWhereConditions > 0)
    {
        delete whereConditions.back();
        whereConditions.pop_back();
        delete whereLabels.back();
        whereLabels.pop_back();
        delete ui.verticalWhereConditions->takeAt(ui.verticalWhereConditions->count() - 1);
        numWhereConditions--;
    }
}

void SelectPage::on_joinsPlusButton_clicked()
{
    addJoin();
}

void SelectPage::on_joinsMinusButton_clicked()
{
    QLayoutItem* item;
    if (numJoins > 0) {
        delete joins[numJoins - 1];
        joins.pop_back();
        delete ui.verticalJoins->takeAt(ui.verticalJoins->count() - 1);
        numJoins--;
    }
}

void SelectPage::on_selectButton_clicked() {
    sqlGenerator::SelectModel sqlCommand;
    std::string tableName = ui.tablesComboBox->currentText().toStdString();
    int numAttribute = Database::instance()->getTableSchema(tableName).size();
    
    for (int i = 0; i < selections.size(); i++) {
        QCheckBox* attribute = selections[i];
        if (attribute->isChecked()) {
            sqlCommand.select(attribute->text().toStdString());
        }
    }
    sqlCommand.from(tableName);

    for (int i = 0; i < numJoins; i++) {
        QString type = joins[i]->getType();
        std::string joinTableName = joins[i]->getTableName().toStdString();

        if (type == "INNER JOIN") {
            
        }
        else if (type == "LEFT JOIN") {
            sqlCommand.left_join(joinTableName);
        }
        else if (type == "RIGHT JOIN") {
            sqlCommand.right_join(joinTableName);
        }
        else if (type == "FULL JOIN") {
            sqlCommand.full_join(joinTableName);
        }
        else if (type == "NATURAL JOIN") {
            sqlCommand.join(joinTableName);
        }
        else {
            qDebug() << "Error finding type of join.";
        }
        for (int j = 0; j < joins[i]->getNumConditions(); j++) {
            QString condition = joins[i]->getConditionAt(i)->getCondition();
            sqlCommand.on(condition.toStdString());
        }
    }
  
    for (int i = 0; i < numWhereConditions; i++) {
        sqlCommand.where(whereConditions[i]->text().toStdString());
    }
    if (ui.groupByLineEdit->text() != "") {
        sqlCommand.group_by(ui.groupByLineEdit->text().toStdString());
    }
    if (ui.orderByLineEdit->text() != "") {
        sqlCommand.order_by(ui.orderByLineEdit->text().toStdString());
    }
    emit selectCommandRequested(sqlCommand.str());
}

void SelectPage::displayOnConditions(JoinWidget* join) {
  
    QLayoutItem* item;
    int count = ui.verticalOnConditions->count();
    for (int i = 0; i < count; i++) {
        item = ui.verticalOnConditions->takeAt(ui.verticalOnConditions->count() - 1);
        item->widget()->hide();
        delete item;
    }
    QWidget* widget;
    for (int i = 0; i < join->getNumConditions(); i++) {
        widget = join->getConditionAt(i)->getWidget();
        widget->show();
        ui.verticalOnConditions->addWidget(widget);

    }
    count = ui.plusMinusButtonsLayout->count();
    for (int i = 0; i < count; i++) {
        item = ui.plusMinusButtonsLayout->takeAt(ui.plusMinusButtonsLayout->count() - 1);
        item->widget()->hide();
        delete item;
    }
    ui.plusMinusButtonsLayout->addWidget(join->getMinusButton());
    join->getMinusButton()->show();
    ui.plusMinusButtonsLayout->addWidget(join->getPlusButton());
    join->getPlusButton()->show();
}


