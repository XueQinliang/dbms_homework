# 小型DBMS设计——BabeDB

> *It is just a Babe!*

本项目是信院DBMS实现课程期末作业，小组成员有薛钦亮、刘佳伟、黄伟琛、郭云达，本次作业分工明确，层次分明，与用户交互较为友好。
本系统的复杂性可以比得上一次计算机系学生本科毕设，如果有时间，应该可以完善得更好。

![](https://github.com/XueQinliang/dbms_homework/raw/main/GUI/商标大.png)

## 系统架构
大体上分为客户端、翻译层、优化层、执行层、存储层，翻译层（parser）从client接收sql语句，转换为类SQL语句通过网络发给优化器，优化器优化之后，传递给执行层，去执行select、project、join，执行结束之后把结果通过网络发给client端。数据应该存入底层的磁盘，要用到的时候用块的方式读入内存。事务的处理就是允许单个事务显式地commit和rollback，程序会在内存里面存一份副本以恢复或者落盘，并发没有做，但允许多个client连接去读数据。
如果要启动图形界面的程序的话，则需要额外启动一个GUI的连接程序，再启动QT，这个连接程序既有server的功能，也有client的功能，从QT的程序端接收sql语句，调用parser解析然后发给server，同时接收server那边传来的数据，传递给QT图形界面，这样做的原因主要是我们的QT程序运行在Windows上，而parser是Linux下编译出来的一个动态链接库。
![](https://github.com/XueQinliang/dbms_homework/raw/main/系统架构.png)

## 界面展示
图形界面
![](https://github.com/XueQinliang/dbms_homework/raw/main/界面展示.png)

命令行界面
![](https://github.com/XueQinliang/dbms_homework/raw/main/client.png)

# How to compile and run:

- put tpch dataset as tbl file in data/
- make
- make install
- export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
- cd main/
- make
- ./sqlserver + port
- ./sqlclient + ip(or localhost) + port
    - port must be the same as server
    - login as a user and password, you can add in main/user.txt
- ./gui is a middle program, to transform information between server and GUI, GUI is written by QT Creator, a QT C++ program, you can run it in Windows or any system.

Need to do:

- see src/util/sqltrans.cpp, if want to optimize sql, you need to change the order of tables and conditions
- src/util/desc.md is a description for StatementInfo

Parser Documentation
====================

这里使用了github用户hyrise开发的sql-parser开源代码，感谢贡献！希望大家都多多贡献开源代码。
这里为了接入到我们的系统，把parser生成的语法树转成了我们自定义的另一种类sql的标记语言，说明请见[类sql说明.pdf](https://github.com/XueQinliang/dbms_homework/raw/main/类sql说明.pdf)

Internal Links:

* [Developer Documentation](dev-docs.md)
* [Supported SQL Queries](syntax-support.md)
* [Known Limitations & Missing Features](known-limitations.md)
* [Basic Usage](basic-usage.md)

External Resources:

* [Original Dev-Paper (2015)](http://torpedro.com/paper/HyriseSQL-03-2015.pdf)
