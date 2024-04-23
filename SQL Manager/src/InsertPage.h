#pragma once

#include <QMainWindow>
#include <qlineedit.h>
#include <vector>
#include "ui_InsertPage.h"
#include <database.h>

class InsertPage : public QMainWindow
{
	Q_OBJECT

public:
	InsertPage(QWidget* parent = nullptr);
	~InsertPage();

signals:
	void dataInserted(std::string sqlCommand);

private:
	void loadTablesComboBox();

	std::vector<QLabel*> columnNames;
	std::vector<QLabel*> columnTypes;
	std::vector<QLineEdit*> columns;
	int numColumns = 0;
	Ui::InsertPageClass ui;

private slots:
	void on_tablesComboBox_currentIndexChanged(int index);
	void on_insertButton_clicked();
};
