
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;
// include the sql parser
#include "SQLParser.h"

// contains printing utilities
#include "util/sqlhelper.h"
#include "util/sqltrans.h"

int main(int argc, char* argv[]) {
  printf("Hello to use BabeDB!\n");
  while(true){
    std::string query;
    cout<<"(babe)> ";
    getline(cin, query);
    // cout<<query;
    // parse a given query
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);

    // check whether the parsing was successful

    if (result.isValid()) {
      printf("Parsed successfully!\n");
      printf("Number of statements: %lu\n", result.size());

      for (auto i = 0u; i < result.size(); ++i) {
        // Print a statement summary.
        hsql::printStatementInfo(result.getStatement(i));
      }
    } else {
      fprintf(stderr, "Given string is not a valid SQL query.\n");
      fprintf(stderr, "%s (L%d:%d)\n",
              result.errorMsg(),
              result.errorLine(),
              result.errorColumn());
    }
  }
}
