#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    // ifstream infile;
    // infile.open("user.txt", ios::in);
    // if (!infile.is_open())
    // {
    //     cout << "读取文件失败" << endl;
    //     return 0;
    // }
    // //第一种读取方法，
    // string user, pswd;
    // while (infile>>user>>pswd)
    // {
    //     cout << user << " " << pswd << endl;//输出读取的文本文件数据
    // }
    string userpswd = "login xql:123456";
    string::size_type pos = userpswd.find_first_of(":");
    cout<<pos<<endl;
    string cuser = userpswd.substr(6, pos-6);
    string cpswd = userpswd.substr(pos+1);
    cout<<cuser << " " << cpswd << endl;
}
