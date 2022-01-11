#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<WINSOCK2.H>
#include <string>
#include <QDebug>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
signals:
    void sendSOCKET(SOCKET);
    void sendCONNECTINFO(QString);
private slots:
    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::login *ui;
    QString ip;
    QString port;
    QString user;
    QString pswd;
    SOCKET s;
};

#endif // LOGIN_H
