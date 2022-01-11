#ifndef SELECT_H
#define SELECT_H

#include <QWidget>
#include <string>
#include <vector>
using namespace std;
namespace Ui {
class Select;
}

class Select : public QWidget
{
    Q_OBJECT

public:
    explicit Select(QWidget *parent = 0);
    void settablecolumns(map<string, vector<pair<string, string>>>* m);
    ~Select();
signals:
    void sendData(QString);
private slots:
    void on_pushButton_close_clicked();

    void on_comboBox_table_activated(const QString &arg1);

    void on_comboBox_column_activated(const QString &arg1);

    void on_lineEdit_inputclause_textChanged(const QString &arg1);

    void on_pushButton_table_clicked();

    void on_pushButton_column_clicked();

    void on_pushButton_clause_clicked();

    void on_pushButton_do_clicked();

private:
    Ui::Select *ui;
    map<string, vector<pair<string, string>>>* tablecolumns;
    vector<string> tables;
    vector<string> columns;
    vector<string> conditions;
    string table;
    string column;
    string condition;
};

#endif // SELECT_H
