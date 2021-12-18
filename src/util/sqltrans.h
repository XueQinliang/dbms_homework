#ifndef SQLPARSER_SQLTRANS_H
#define SQLPARSER_SQLTRANS_H

#include "../sql/statements.h"

namespace hsql {
    // 把解析出来的sql转为类sql
    std::string transcolumntype(DataType type);
    std::string TransStatementInfo(const SQLStatement* stmt);
    std::string TransInsertStatementInfo(const InsertStatement* stmt);
    std::string TransCreateStatementInfo(const CreateStatement* stmt);
    std::string TransSelectStatementInfo(const SelectStatement* stmt);
    std::string TransDeleteStatementInfo(const DeleteStatement* stmt);
    std::string TransDropStatementInfo(const DropStatement* stmt);
    std::string TransUpdateStatementInfo(const UpdateStatement* stmt);
    std::string TransImportStatementInfo(const ImportStatement* stmt);
} // namespace hsql
#endif
