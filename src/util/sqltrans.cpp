#include "sqlhelper.h"
#include <iostream>
#include <map>
#include <string>
using namespace std;

namespace hsql {

enum Status {
  OK,
  Fail,
};

bool intransaction = false;
bool commit_or_rollback = -1; //commit 0, rollback 1

typedef struct Condition{
  string type;
  string op;
  string lefttable;
  string leftiscolumn;
  string leftname;
  string leftvalue;
  string righttable;
  string rightiscolumn;
  string rightname;
  string rightvalue;
}Condition;

map<string, vector<string>> tablecolumns;
map<string, map<string, string>> tablecolumntypes;

Status transexpr(string tablename, Expr* expr, vector<Condition>& result){
  //cout<<expr->type<<endl;
  if(expr->type == ExprType::kExprOperator){
    //cout<<expr->expr->type<<ends<<expr->expr2->type<<endl;
    if(expr->expr!=nullptr && expr->expr2!=nullptr && expr->expr->type!=kExprOperator && expr->expr2->type!=kExprOperator){
      Condition con;
      switch (expr->opType)
      {
      case OperatorType::kOpEquals:
        con.op = "=";
        break;
      case OperatorType::kOpLess:
        con.op = "<";
        break;
      case OperatorType::kOpLessEq:
        con.op = "<=";
        break;
      case OperatorType::kOpGreater:
        con.op = ">";
        break;
      case OperatorType::kOpGreaterEq:
        con.op = ">=";
        break;
      case OperatorType::kOpNotEquals:
        con.op = "<>";
        break;
      default:
        break;
      }
      con.leftiscolumn = expr->expr->type == kExprColumnRef? "0":"1";
      if(con.leftiscolumn == "0"){
        con.leftname = string(expr->expr->name);
        con.leftvalue = "0";
        if(expr->expr->table!=nullptr){
          con.lefttable = string(expr->expr->table);
        }else{
          con.lefttable = tablename;
        }
        con.type = tablecolumntypes[con.lefttable][con.leftname];
      }else{
        con.leftname = "0";
        switch (expr->expr->type)
        {
        case ExprType::kExprLiteralInt:
          con.type = "int";
          con.leftvalue = to_string(expr->expr->ival);
          break;
        case ExprType::kExprLiteralFloat:
          con.type = "double";
          con.leftvalue = to_string(expr->expr->fval);
          break;
        case ExprType::kExprLiteralString: case ExprType::kExprLiteralDate:
          con.type = "string";
          con.leftvalue = string(expr->expr->name);
          break;
        default:
          break;
        }
      }
      con.rightiscolumn = expr->expr2->type == kExprColumnRef? "0":"1";
      if(con.rightiscolumn == "0"){
        con.rightname = string(expr->expr2->name);
        con.rightvalue = "0";
        if(expr->expr2->table!=nullptr){
          con.righttable = string(expr->expr2->table);
        }else{
          con.righttable = tablename;
        }
        con.type = tablecolumntypes[con.righttable][con.rightname];
      }else{
        con.rightname = "0";
        switch (expr->expr2->type)
        {
        case ExprType::kExprLiteralInt:
          con.type = "int";
          con.rightvalue = to_string(expr->expr2->ival);
          break;
        case ExprType::kExprLiteralFloat:
          con.type = "double";
          con.rightvalue = to_string(expr->expr2->fval);
          break;
        case ExprType::kExprLiteralString: case ExprType::kExprLiteralDate:
          con.type = "string";
          con.rightvalue = string(expr->expr2->name);
          break;
        default:
          break;
        }
      }
      // cout<<con.lefttable<<" "<<con.righttable<<endl;
      // cout<<con.lefttable.size()<<" "<<con.righttable.size()<<endl;
      if(!con.lefttable.size() && con.righttable.size()){
        con.lefttable = con.righttable;
      }
      if(con.lefttable.size() && !con.righttable.size()){
        con.righttable = con.lefttable;
      }
      result.push_back(con);
      return Status::OK;
    }else if(expr->expr!=nullptr && expr->expr2!=nullptr){
      //printf("test\n");
      if(expr->expr->type==kExprOperator){
        Status s = transexpr(tablename, expr->expr, result);
        if(s == Status::Fail){
          return Status::Fail;
        }
      }
      if(expr->expr2->type==kExprOperator){
        Status s = transexpr(tablename, expr->expr2, result);
        if(s == Status::Fail){
          return Status::Fail;
        }
      }
      return Status::OK;
    }
  }
  return Status::Fail;
}

Status transexpr(Expr* expr, vector<Condition>& result){
  //cout<<expr->type<<endl;
  if(expr->type == ExprType::kExprOperator){
    //cout<<expr->expr->type<<ends<<expr->expr2->type<<endl;
    if(expr->expr!=nullptr && expr->expr2!=nullptr && expr->expr->type!=kExprOperator && expr->expr2->type!=kExprOperator){
      Condition con;
      switch (expr->opType)
      {
      case OperatorType::kOpEquals:
        con.op = "=";
        break;
      case OperatorType::kOpLess:
        con.op = "<";
        break;
      case OperatorType::kOpLessEq:
        con.op = "<=";
        break;
      case OperatorType::kOpGreater:
        con.op = ">";
        break;
      case OperatorType::kOpGreaterEq:
        con.op = ">=";
        break;
      case OperatorType::kOpNotEquals:
        con.op = "<>";
        break;
      default:
        break;
      }
      con.leftiscolumn = expr->expr->type == kExprColumnRef? "0":"1";
      if(con.leftiscolumn == "0"){
        con.leftname = string(expr->expr->name);
        con.leftvalue = "0";
        if(expr->expr->table!=nullptr){
          con.lefttable = string(expr->expr->table);
        }else{
          for(auto table: tablecolumns){
            for(auto columnname : table.second){
              if(columnname == con.leftname){
                con.lefttable = string(table.first);
                goto end1;
              }
            }
          }
          printf("can't find this column in any table");
          return Status::Fail;
          end1:;
        }
        //printf("left\n");
        con.type = tablecolumntypes[con.lefttable][con.leftname];
      }else{
        con.leftname = "0";
        switch (expr->expr->type)
        {
        case ExprType::kExprLiteralInt:
          con.type = "int";
          con.leftvalue = to_string(expr->expr->ival);
          break;
        case ExprType::kExprLiteralFloat:
          con.type = "double";
          con.leftvalue = to_string(expr->expr->fval);
          break;
        case ExprType::kExprLiteralString: case ExprType::kExprLiteralDate:
          con.type = "string";
          con.leftvalue = string(expr->expr->name);
          break;
        default:
          break;
        }
      }
      con.rightiscolumn = expr->expr2->type == kExprColumnRef? "0":"1";
      if(con.rightiscolumn == "0"){
        con.rightname = string(expr->expr2->name);
        con.rightvalue = "0";
        if(expr->expr2->table!=nullptr){
          con.righttable = string(expr->expr2->table);
        }else{
          for(auto table: tablecolumns){
            for(auto columnname : table.second){
              if(columnname == con.rightname){
                con.righttable = string(table.first);
                goto end2;
              }
            }
          }
          printf("can't find this column in any table");
          return Status::Fail;
          end2:;
        }
        //printf("right\n");
        con.type = tablecolumntypes[con.righttable][con.rightname];
      }else{
        con.rightname = "0";
        switch (expr->expr2->type)
        {
        case ExprType::kExprLiteralInt:
          con.type = "int";
          con.rightvalue = to_string(expr->expr2->ival);
          break;
        case ExprType::kExprLiteralFloat:
          con.type = "double";
          con.rightvalue = to_string(expr->expr2->fval);
          break;
        case ExprType::kExprLiteralString: case ExprType::kExprLiteralDate:
          con.type = "string";
          con.rightvalue = string(expr->expr2->name);
          break;
        default:
          break;
        }
      }
      // cout<<con.lefttable<<" "<<con.righttable<<endl;
      // cout<<con.lefttable.size()<<" "<<con.righttable.size()<<endl;
      if(!con.lefttable.size() && con.righttable.size()){
        con.lefttable = con.righttable;
      }
      if(con.lefttable.size() && !con.righttable.size()){
        con.righttable = con.lefttable;
      }
      result.push_back(con);
      return Status::OK;
    }else if(expr->expr!=nullptr && expr->expr2!=nullptr){
      //printf("test\n");
      if(expr->expr->type==kExprOperator){
        Status s = transexpr(expr->expr, result);
        if(s == Status::Fail){
          return Status::Fail;
        }
      }
      if(expr->expr2->type==kExprOperator){
        Status s = transexpr(expr->expr2, result);
        if(s == Status::Fail){
          return Status::Fail;
        }
      }
      return Status::OK;
    }
  }
  return Status::Fail;
}

std::string transcolumntype(DataType type){
  string strtype = "";
  switch (type)
  {
  case DataType::INT: case DataType::LONG: case DataType::SMALLINT:
      strtype = "int";
      break;
  case DataType::FLOAT: case DataType::DOUBLE: case DataType::REAL: case DataType::DECIMAL:
      strtype = "double";
      break;
  case DataType::TEXT: case DataType::TIME: case DataType::DATETIME: case DataType::VARCHAR: case DataType::CHAR:
      strtype = "string";
      break;
  default:
      break;
  }
  return strtype;
}

std::string TransInsertStatementInfo(const InsertStatement* stmt){
  string result = "-3\n";
  string tablename = stmt->tableName;
  if(tablecolumns.find(tablename) == tablecolumns.end()){
    char returnstr[100];
    sprintf(returnstr, "can't find this table:%s\n", tablename.c_str());
    return string(returnstr);
  }
  result += (string(tablename) + "\n");
  int valuelen = stmt->values->size();
  result += (to_string(valuelen) + "\n");
  for(Expr* value : *stmt->values){
    ExprType exprtype = value->type;
    string strtype = "";
    string strvalue = "";
    switch (exprtype)
    {
    case ExprType::kExprLiteralInt:
      strtype = "int";
      strvalue = to_string(value->ival);
      break;
    case ExprType::kExprLiteralFloat:
      strtype = "double";
      strvalue = to_string(value->fval);
      break;
    case ExprType::kExprLiteralString: case ExprType::kExprLiteralDate:
      strtype = "string";
      strvalue = string(value->name);
      break;
    default:
      break;
    }
    result += (strtype + "\n");
    result += (strvalue + "\n");
  }
  return result;
}

std::string TransCreateStatementInfo(const CreateStatement* stmt){
  if(stmt->type == CreateType::kCreateIndex){
    string result = "-6\n";
    string tablename = string(stmt->tableName);
    
    if(tablecolumns.find(tablename) == tablecolumns.end()){
      printf("can't find this table:%s\n", tablename.c_str());
      char returnstr[100];
      sprintf(returnstr, "can't find this table:%s\nFail\n", tablename.c_str());
      return string(returnstr);
    }
    string indexname = string(stmt->indexName);
    string columnname;
    if(stmt->indexColumns->size()!=1){
      printf("you can only use index on one column, in future we will develop multicolumn index\n");
      char returnstr[100];
      sprintf(returnstr, "you can only use index on one column, in future we will develop multicolumn index\n");
      return string(returnstr);
    }else{
      columnname = string((*stmt->indexColumns)[0]);
    }
    result += (tablename + " " + columnname + "\n"); 
    return result;
  }else if(stmt->type == CreateType::kCreateTable){
    string tablename = string(stmt->tableName);
    if(tablecolumns.find(tablename)!=tablecolumns.end()){
      //?????????????????????
      string result = "This table has existed!\nFail\n";
      return result;
    }
    string result = "-1\n";
    result += tablename;
    result += "\n";
    int columnlen = stmt->columns->size();
    result += to_string(columnlen);
    result += "\n";
    tablecolumns[stmt->tableName] = vector<string>();
    tablecolumntypes[stmt->tableName] = map<string,string>();
    for(ColumnDefinition* column : *stmt->columns){
        string name = string(column->name);
        DataType type = column->type.data_type;
        string strtype = transcolumntype(type);
        result += (name + " " + strtype + "\n");
        tablecolumns[stmt->tableName].push_back(name);
        tablecolumntypes[stmt->tableName][name] = strtype;
    }
    // printf("start to write\n");
    // FILE *fp = fopen("../../main/table_meta","w");
    // int size = sizeof(tablecolumns);
    // printf("size:%d\n", size);
    // fwrite(&size, sizeof(int), 1, fp);
    // fwrite(&tablecolumns, sizeof(tablecolumns), 1, fp);
    // fclose(fp);
    return result;
  }else{
    return "Fail\n";
  }
}

std::string TransSelectStatementInfo(const SelectStatement* stmt){
  string result = "";
  TableRef* tableref = stmt->fromTable;
  int tablenum;
  vector<string> tables;
  //printf("test1 begin tables\n");
  if(tableref->type == TableRefType::kTableName){
    //printf("test table\n");
    tablenum = 1;
    if(tablecolumns.find(string(tableref->name)) == tablecolumns.end()){
      printf("can't find this table: %s\n", tableref->name);
      char returnstr[100];
      sprintf(returnstr, "can't find this table:%s\nFail\n", tableref->name);
      return string(returnstr);
    }
    tables.push_back(string(tableref->name));
  }else if(tableref->type == TableRefType::kTableCrossProduct){
    //printf("test cross\n");
    tablenum = tableref->list->size();
    //printf("test %d\n", tablenum);
    for(TableRef* tf : *(tableref->list)){
      if(tablecolumns.find(string(tf->name)) == tablecolumns.end()){
        printf("can't find this table: %s\n", tableref->name);
        char returnstr[100];
        sprintf(returnstr, "can't find this table:%s\nFail\n", tableref->name);
        return string(returnstr);
      }
      tables.push_back(string(tf->name));
    }  
  }
  //printf("test1 end tables\n");
  int columnnum = stmt->selectList->size();
  vector<pair<string,string>> columns;
  for(Expr* expr : *(stmt->selectList)){
    if(expr->type == ExprType::kExprColumnRef){
      string columnname = string(expr->name);
      string tablename = "";
      if(expr->table){
        tablename = string(expr->table);
        columns.push_back(make_pair(tablename, columnname));
      }else{
        for(auto table: tablecolumns){
          for(auto tablecolumnname : table.second){
            if(tablecolumnname == columnname){
              columns.push_back(make_pair(string(table.first), columnname));
              // columns.push_back(make_pair(string(createtable->tableName), columnname));
            }
          }
        }
      }
    }else if(expr->type == ExprType::kExprStar){
      for(auto table: tables){
        for(auto tablecolumnname : tablecolumns[table]){
          columns.push_back(make_pair(table, tablecolumnname));
        }
      }
      columnnum = columns.size();
    }
  }
  result += (to_string(tablenum) + "\n");
  for(auto tablename : tables){
    result += ( tablename + "\n");
  }
  result += (to_string(columnnum) + "\n");
  for(auto columnname : columns){
    result += ( columnname.first + " " + columnname.second + "\n");
  }

  vector<Condition> conresult;
  if(stmt->whereClause){
    Status s = transexpr(stmt->whereClause, conresult);
    if(s == Status::Fail){
      return "Fail\n";
    }
    result += (to_string(conresult.size()) + "\n");
    for(auto con : conresult){
      result += (con.type + "\n");
      result += (con.leftiscolumn + " " + con.lefttable + " " + con.leftname + " " + con.leftvalue + "\n");
      result += (con.rightiscolumn + " " + con.righttable + " " + con.rightname + " " + con.rightvalue + "\n");
      result += (con.op + "\n");
    }
  }else{
    result += "0\n";
  }
  result += "AND\n";
  return result;
}

std::string TransDeleteStatementInfo(const DeleteStatement* stmt){
  string result = "-4\n";
  string tablename = string(stmt->tableName);
  if(tablecolumns.find(tablename) == tablecolumns.end()){
    printf("can't find this table:%s\n", tablename.c_str());
    char returnstr[100];
    sprintf(returnstr, "can't find this table:%s\nFail\n", tablename.c_str());
    return string(returnstr);
  }
  result += (tablename + "\n");
  vector<Condition> conresult;
  if(stmt->expr){
    Status s = transexpr(tablename, stmt->expr, conresult);
    if(s == Status::Fail){
      return "Fail\n";
    }
    result += (to_string(conresult.size()) + "\n");
    for(auto con : conresult){
      result += (con.type + "\n");
      result += (con.leftiscolumn + " " + con.lefttable + " " + con.leftname + " " + con.leftvalue + "\n");
      result += (con.rightiscolumn + " " + con.righttable + " " + con.rightname + " " + con.rightvalue + "\n");
      result += (con.op + "\n");
    }
  }else{
    result += "0\n";
  }
  result += "AND\n";
  return result;
}

std::string TransDropStatementInfo(const DropStatement* stmt){
  string result = "-7\n";
  string tablename = string(stmt->name);
  if(tablecolumns.find(tablename) == tablecolumns.end()){
    printf("can't find this table:%s\n", tablename.c_str());
    char returnstr[100];
    sprintf(returnstr, "can't find this table:%s\nFail\n", tablename.c_str());
    return string(returnstr);
  }
  result += tablename;
  tablecolumns.erase(tablename);
  tablecolumntypes.erase(tablename);
  // FILE *fp = fopen("../../main/table_meta","wb");
  // fwrite(&tablecolumns, sizeof(tablecolumns), 1, fp);
  // fclose(fp);
  return result;
}

std::string TransUpdateStatementInfo(const UpdateStatement* stmt){
  string result = "-5\n";
  string tablename = string(stmt->table->name); //?????????????????????
  if(tablecolumns.find(tablename) == tablecolumns.end()){
    printf("can't find this table:%s\n", tablename.c_str());
    char returnstr[100];
    sprintf(returnstr, "can't find this table:%s\nFail\n", tablename.c_str());
    return string(returnstr);
  }
  if(stmt->updates->size()==0){
    printf("update nothing\n");
    char returnstr[100];
    sprintf(returnstr, "update nothing\nFail\n");
    return string(returnstr);
  }
  string columnname = string((*stmt->updates)[0]->column); //??????????????????????????????????????????????????????????????????
  ExprType type = (*stmt->updates)[0]->value->type;
  string strtype;
  string strvalue;
  switch (type)
  {
  case ExprType::kExprLiteralInt:
    strtype = "int";
    strvalue = to_string((*stmt->updates)[0]->value->ival);
    break;
  case ExprType::kExprLiteralFloat:
    strtype = "double";
    strvalue = to_string((*stmt->updates)[0]->value->fval);
    break;
  case ExprType::kExprLiteralString: case ExprType::kExprLiteralDate:
    strtype = "string";
    strvalue = string((*stmt->updates)[0]->value->name);
    break;
  default:
    break;
  }
  result += (tablename + "\n");
  result += (columnname + " " + strtype + "\n");
  result += (strvalue + "\n");
  vector<Condition> conresult;
  if(stmt->where){
    Status s = transexpr(tablename, stmt->where, conresult);
    if(s == Status::Fail){
      return "Fail\n";
    }
    result += (to_string(conresult.size()) + "\n");
    for(auto con : conresult){
      result += (con.type + "\n");
      result += (con.leftiscolumn + " " + con.lefttable + " " + con.leftname + " " + con.leftvalue + "\n");
      result += (con.rightiscolumn + " " + con.righttable + " " + con.rightname + " " + con.rightvalue + "\n");
      result += (con.op + "\n");
    }
  }else{
    result += "0\n";
  }
  result += "AND\n";
  return result;
}

std::string TransImportStatementInfo(const ImportStatement* stmt){
  string result = "-2\n";
  string filepath = string(stmt->filePath);
  if(!fopen(filepath.c_str(), "r")){
    printf("open file %s wrong!\n", filepath.c_str());
    char returnstr[100];
    sprintf(returnstr, "open file %s wrong!\n", filepath.c_str());
    return string(returnstr);
  }
  string tablename = string(stmt->tableName);
  if(tablecolumns.find(tablename) == tablecolumns.end()){
    printf("can't find this table:%s\n", tablename.c_str());
    char returnstr[100];
    sprintf(returnstr, "can't find this table:%s\n", tablename.c_str());
    return string(returnstr);
  }
  result += (filepath + "\n");
  result += tablename;
  return result;
}

std::string TransStatementInfo(const SQLStatement* stmt){
  //cout<<stmt->type()<<endl;
  switch (stmt->type()) {
    case kStmtSelect:
      return TransSelectStatementInfo((const SelectStatement*)stmt);
      break;
    case kStmtInsert:
      return TransInsertStatementInfo((const InsertStatement*)stmt);
      break;
    case kStmtCreate:
      return TransCreateStatementInfo((const CreateStatement*)stmt);
      break;
    case kStmtImport:
      return TransImportStatementInfo((const ImportStatement*)stmt);
      break;
    case kStmtDelete:
      return TransDeleteStatementInfo((const DeleteStatement*)stmt);
      break;
    case kStmtDrop:
      return TransDropStatementInfo((const DropStatement*)stmt);
      break;
    case kStmtUpdate:
      return TransUpdateStatementInfo((const UpdateStatement*)stmt);
      break;
    case kStmtShow:
      for(auto table: tablecolumns){
        cout << table.first << endl;
        // for(auto columnname : table.second){
        //   cout << columnname << endl;
        // }
      }
      return "";
      break;
    case kStmtTransaction:
      //????????????
      switch (((const TransactionStatement*)stmt)->command)
      {
      case kBeginTransaction:
        intransaction = true;
        return "-8\n";
        break;
      case kCommitTransaction:
        /* code */
        commit_or_rollback = 0;
        intransaction = false;
        return "-9\n";
        break;
      case kRollbackTransaction:
        /* code */
        commit_or_rollback = 1;
        intransaction = false;
        return "-10\n";
        break;
      default:
        break;
      }
      break;
    default:
      break;
  };
  return "can't match any sql";
}

}