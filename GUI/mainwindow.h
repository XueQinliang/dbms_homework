#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include <string>
#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include <sstream>
#include<cstring>
#include<QDebug>
#include "form.h"
#include "select.h"
#include "login.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_textEdit_textChanged();

    void on_create_table_clicked();

    void on_excute_clicked();

    void receiveData(QString data);

    void receiveSOCKET(SOCKET s);

    void receiveConnectinfo(QString data);

    void on_select_button_clicked();

    //void on_pushButton_clicked();

    void on_connect_button_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *lab;
    Form *create;
    Select *select;
    login *log;
    string sql;
    SOCKET sclient;
    map<string, vector<pair<string, string>>> tablecolumns;
    map<string, vector<string>> tableindex;
    string showtables;
    void showtablesfunc();
};

#endif // MAINWINDOW_H
