# SelectStatement中结构的含义

vector<Expr*>* selectList //选择的列

TableRef* fromTable //涉及的表
包含四种类型，kTableName就是个表, kTableSelect是一个选择子句, kTableJoin是一个join, kTableCrossProduct是多表的笛卡尔积
kTableCrossProduct情况下，vector<TableRef*>* list指向一堆新的表，如果调换表顺序可以在这个地方

Expr* whereClause //涉及的条件
Expr完全是一棵树的结构，打印的时候递归打印
如果Expr是一个kExprOperator，比如等于、AND，则有expr和expr2两个子节点
如果Expr是一个kExprColumnRef，也即为列名，则可以通过expr->name、expr->table访问列名和表名（如果语句中有表名，类似于s.sid）
Expr可以为kExprLiteralFloat、kExprLiteralInt、kExprLiteralString、kExprLiteralDate、kExprLiteralNull，但佳伟那边Date需要转为String查询
如果是String和Date，访问值用expr->name, 如果为Float和Int，访问值用expr->ival
此外，Expr支持嵌套，也就是可以为一个Select语句，但咱们暂时不用管嵌套

# CreateStatement中结构的含义

char* tableName 表名
vector<ColumnDefinition*>* columns; 列的信息
ColumnDefinition主要包含以下几个有用信息
char* name;
ColumnType type;
bool nullable;

char* filePath 可以直接用一个tbl创建表，filepath就是路径
SelectStatement* select 可以用一个select语句创建表

创建索引要用到的
char* indexName;                                  // default: nullptr
std::vector<char*>* indexColumns;                 // default: nullptr

# InsertStatement中结构的含义

schema或者tablename 模式名字或表名
std::vector<char*>* columns; 列名
std::vector<Expr*>* values;  插入的值，只能插入一个tuple，不支持多条记录插入
SelectStatement* select 可以用一个select语句插入数据

# DeleteStatement中结构的含义

模式名 表名 where中的条件，where中的条件同select的格式
  char* schema;
  char* tableName;
  Expr* expr;

# DropStatement中结构的含义

可以删除表，索引，模式，视图等
  DropType type; // kDropTable, kDropSchema, kDropIndex,
  bool ifExists;
  char* schema;
  char* name;
  char* indexName;

# UpdateStatement中结构的含义
struct UpdateClause {
  char* column;
  Expr* value;
};

// Represents SQL Update statements.
struct UpdateStatement : SQLStatement {
  UpdateStatement();
  ~UpdateStatement() override;

  // TODO: switch to char* instead of TableRef
  TableRef* table;
  std::vector<UpdateClause*>* updates;
  Expr* where;
};

# ImportStatement中结构的含义
ImportType type;
char* filePath;
char* schema;
char* tableName;
