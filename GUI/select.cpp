#include "select.h"
#include "ui_select.h"

Select::Select(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Select)
{
    ui->setupUi(this);
}

Select::~Select()
{
    delete ui;
}

void Select::settablecolumns(map<string, vector<pair<string, string> > > *m){
    this->tablecolumns = m;
    for(auto table:*tablecolumns){
        const QString item = QString::fromStdString(table.first);
        ui->comboBox_table->addItem(item);
        for(auto column:table.second){
            ui->comboBox_column->addItem(QString::fromStdString(table.first + "." + column.first));
        }
    }
}

void Select::on_pushButton_close_clicked()
{
    this->close();
}

void Select::on_comboBox_table_activated(const QString &arg1)
{
    table = arg1.toStdString();
}

void Select::on_comboBox_column_activated(const QString &arg1)
{
    column = arg1.toStdString();
}

void Select::on_lineEdit_inputclause_textChanged(const QString &arg1)
{
    condition = arg1.toStdString();
}

void Select::on_pushButton_table_clicked()
{
    tables.push_back(table);
    table = "";
    string tablesstr;
    for(auto table: tables){
        tablesstr += (table + "\n");
    }
    ui->textBrowser_tables->setText(QString::fromStdString(tablesstr));
}

void Select::on_pushButton_column_clicked()
{
    columns.push_back(column);
    column = "";
    string columnsstr;
    for(auto column: columns){
        columnsstr += (column + "\n");
    }
    ui->textBrowser_columns->setText(QString::fromStdString(columnsstr));
}

void Select::on_pushButton_clause_clicked()
{
    conditions.push_back(condition);
    condition = "";
    ui->pushButton_clause->setText("");
    string conditionsstr;
    for(auto condition: conditions){
        conditionsstr += (condition + "\n");
    }
    ui->textBrowser_clause->setText(QString::fromStdString(conditionsstr));
}

void Select::on_pushButton_do_clicked()
{
    string selectsql = "select ";
    bool first = true;
    for(auto column:columns){
        if(first){
            first = false;
        }else{
            selectsql += ",";
        }
        selectsql += column;
    }
    selectsql += " from ";
    first = true;
    for(auto table:tables){
        if(first){
            first = false;
        }else{
            selectsql += ",";
        }
        selectsql += table;
    }
    selectsql += " where ";
    first = true;
    for(auto condition:conditions){
        if(first){
            first = false;
        }else{
            selectsql += " and ";
        }
        selectsql += condition;
    }
    selectsql += ";";
    emit sendData(QString::fromStdString(selectsql));
    this->close();
}
