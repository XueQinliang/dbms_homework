#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
map<string, vector<string> > tablecolumns; 
int main(int argc, char** argv) {
	string aname = "A";
    vector<string> Atable;
    Atable.push_back("id");
    Atable.push_back("name");
    tablecolumns[aname] = Atable;
    vector<string> Btable;
    string bname = "B";
    Btable.push_back("id");
    Btable.push_back("name");
    tablecolumns[bname] = Btable;
    FILE *fp = fopen("table_meta","wb");
    int size = sizeof(tablecolumns);
    printf("size:%d\n", size);
    fwrite(&size, sizeof(int), 1, fp);
    fwrite(&tablecolumns, sizeof(tablecolumns), 1, fp);
    fclose(fp);
    
    fp = fopen("table_meta", "rb");
	fread(&size, 4, 1, fp);
	printf("size:%d\n", size);
	map<string, vector<string> > *ts = (map<string, vector<string> > *)malloc(size);
	fread(ts, size, 1, fp);
	fclose(fp);
	string showtables;
	for(auto table:*ts){
        cout<<table.first<<endl;
		showtables += table.first;
		showtables += "\n";
		for(auto columnname : table.second){
			showtables += "\t";
			showtables += columnname;
			showtables += "\n";
		}
	}
    cout<<showtables<<endl;
	return 0;
}