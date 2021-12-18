Documentation
=============

Internal Links:

 * [Developer Documentation](dev-docs.md)
 * [Supported SQL Queries](syntax-support.md)
 * [Known Limitations & Missing Features](known-limitations.md)
 * [Basic Usage](basic-usage.md)


External Resources:

 * [Original Dev-Paper (2015)](http://torpedro.com/paper/HyriseSQL-03-2015.pdf)

How to compile and run:

- make
- make install
- cd main/
- make
- ./main

Need to do:
- see src/util/sqltrans.cpp, if want to optimize sql, you need to change the order of tables and conditions
- src/util/desc.md is a description for StatementInfo
