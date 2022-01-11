#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    model = new QStandardItemModel();
    ui->createtableView->setModel(model);
    this->model->setHorizontalHeaderItem(0, new QStandardItem("列名"));
    this->model->setHorizontalHeaderItem(1, new QStandardItem("类型"));
}

Form::~Form()
{
    delete ui;
}

void Form::on_cancel_clicked()
{
    this->close();
}

void Form::on_tablename_textChanged(const QString &arg1)
{
    tablename = arg1;
}

void Form::on_columnname_textChanged(const QString &arg1)
{
    columnname = arg1;
}

void Form::on_type_textChanged(const QString &arg1)
{
    columntype = arg1;
}

void Form::on_addcolumn_clicked()
{
    int row = tablecolumns.size();
    tablecolumns.push_back(make_pair(columnname, columntype) );
    this->model->setItem(row, 0, new QStandardItem(columnname) );
    this->model->setItem(row, 1, new QStandardItem(columntype) );
    columnname = "";
    columntype = "";
    ui->columnname->setText(columnname);
    ui->type->setText(columntype);
}

void Form::on_createtableView_doubleClicked(const QModelIndex &index)
{
    //this->model->setItem(index.row(), index.column(), new QStandardItem(index.data()));
    int row = index.row();
    if(index.column()==1){
        tablecolumns[row].second = QString(index.data().toString());
    }else{
        tablecolumns[row].first = QString(index.data().toString());
    }
    for(auto iter=tablecolumns.begin();iter!=tablecolumns.end(); iter++)
    {
        if(iter->first == "" and iter->second == ""){
            tablecolumns.erase(iter);
        }
    }
}

void Form::on_createtableb2_clicked()
{
    QString createsql = "create table " + tablename;
    createsql += "(";
    bool first = true;
    for(auto column:tablecolumns){
        if(first){
            first = false;
        }else{
            createsql += ",";
        }
        createsql += (column.first + " " + column.second + " not null");

    }
    createsql += ");";
    emit sendData(createsql);
    this->close();
}
