#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <vector>
#include <QStandardItemModel>
using namespace std;
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    QString tablename;
    QString columnname;
    QString columntype;
    vector<pair<QString,QString>> tablecolumns;
    ~Form();
signals:
    void sendData(QString);

private slots:
    void on_cancel_clicked();

    void on_tablename_textChanged(const QString &arg1);

    void on_columnname_textChanged(const QString &arg1);

    void on_type_textChanged(const QString &arg1);

    void on_addcolumn_clicked();

    void on_createtableView_doubleClicked(const QModelIndex &index);

    void on_createtableb2_clicked();

private:
    Ui::Form *ui;
    QStandardItemModel* model;
};

#endif // FORM_H
