#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
using namespace std;

template<typename T> void func(void *p) {
	T a = *(T *)p;
	cout << a << endl;
}

template<class T> class Data {
	public:
		T x;
};

template<class T> void print(void *p) {
	T y = (*(Data<T> *)p).x;
	cout << y << endl;
}

void func_void(vector<pair<string, void *> > *t) {
	for (int i = 0; i < (*t).size(); i++) {
		if ((*t)[i].first == "string")
			print<string>((*t)[i].second);
		else if ((*t)[i].first == "int")
			print<int>((*t)[i].second);
		else if ((*t)[i].first == "double")
			print<double>((*t)[i].second);
	}
}

vector<int> *kmerge(vector<vector<int> *> *arr) {
	vector<int> *res = new vector<int>;
	vector<int> i;
	for (int j = 0; j < (*arr).size(); j++)
		i.push_back(0);

	while (true) {
		int mini_pk = -1, mini_pk_j = -1;
		for (int j = 0; j < i.size(); j++) {
			vector<int> *arr_j = (*arr)[j];
			if (i[j] < (*arr_j).size() && (mini_pk == -1 || (*arr_j)[i[j]] < mini_pk)) {
				mini_pk = (*arr_j)[i[j]];
				mini_pk_j = j;
			}
		}

		if (mini_pk == -1)
			break;

		(*res).push_back(mini_pk);
		i[mini_pk_j]++;
	}

	return res;
}

vector<int> *kmerge_union(vector<vector<int> *> *arr) {
	vector<int> *res = new vector<int>;
	vector<int> i;
	for (int j = 0; j < (*arr).size(); j++)
		i.push_back(0);

	while (true) {
		int mini_pk = -1;
		for (int j = 0; j < i.size(); j++) {
			vector<int> *arr_j = (*arr)[j];
			if (i[j] < (*arr_j).size() && (mini_pk == -1 || (*arr_j)[i[j]] < mini_pk))
				mini_pk = (*arr_j)[i[j]];
		}

		if (mini_pk == -1)
			break;

		if (!(*res).size() || mini_pk > (*res)[(*res).size() - 1])
			(*res).push_back(mini_pk);
		for (int j = 0; j < i.size(); j++) {
			vector<int> *arr_j = (*arr)[j];
			if (i[j] < (*arr_j).size() && (*arr_j)[i[j]] == mini_pk)
				i[j]++;
		}
	}

	return res;
}

/*
3
3 0 2 4
4 0 1 1 2
3 2 3 4
*/

/*
3
3 0 2 4
5 1 2 4 8 9
4 2 4 8 8
*/
vector<int> *kmerge_intersection(vector<vector<int> *> *arr) {
	vector<int> *res = new vector<int>;
	vector<int> i;
	for (int j = 0; j < (*arr).size(); j++)
		i.push_back(0);

	while (true) {

//		cout << "S: ";
//		for (int j = 0; j < i.size(); j++)
//			cout << i[j] << ' ';
//		cout << endl;

		int maxi_pk = -1;
		for (int j = 0; j < i.size(); j++) {
			vector<int> *arr_j = (*arr)[j];
			if (i[j] >= (*arr_j).size())
				return res;
			if ((*arr_j)[i[j]] > maxi_pk)
				maxi_pk = (*arr_j)[i[j]];
		}

		bool same = true;
		for (int j = 0; j < i.size(); j++) {
			vector<int> *arr_j = (*arr)[j];
			if ((*arr_j)[i[j]] < maxi_pk) {
				same = false;
				break;
			}
		}

		if (same) {
			if (!(*res).size() || maxi_pk > (*res)[(*res).size() - 1])
				(*res).push_back(maxi_pk);
			for (int j = 0; j < i.size(); j++)
				i[j]++;
		} else
			for (int j = 0; j < i.size(); j++) {
				vector<int> *arr_j = (*arr)[j];
				if ((*arr_j)[i[j]] < maxi_pk)
					i[j]++;
			}
	}

	return res;
}

void test_kmerge() {
	vector<vector<int> *> *arr = new vector<vector<int> *>;

	int n;
	cin >> n;

	for (int i = 0; i < n; i++) {
		vector<int> *arri = new vector<int>;
		(*arr).push_back(arri);

		int ni;

		cin >> ni;
		for (int j = 0; j < ni; j++) {
			int x;
			cin >> x;
			(*arri).push_back(x);
		}
	}

	//vector<int> *res = kmerge(arr);
	//vector<int> *res = kmerge_union(arr);
	vector<int> *res = kmerge_intersection(arr);

	for (int i = 0; i < (*res).size(); i++)
		cout << (*res)[i] << ' ';
	cout << endl;
}

template<class T> T get_value(T x) {
	return x;
}

void print_value(void *p, string type) {
	int x_i = *(int *)p;
	string x_s = *(string *)p;
	double x_d = *(double *)p;
	if (type == "int")
		cout << x_i << endl;
	else if (type == "string")
		cout << x_s << endl;
	else if (type == "double")
		cout << x_d << endl;
}

void test_value() {
	int key_int = get_value(77);
	string key_string = get_value("hello world");
	double key_double = get_value((double)3.1415926);

	void *p;
	p = &key_int;
	print_value(p, "int");
	p = &key_string;
	print_value(p, "string");
	p = &key_double;
	print_value(p, "double");
}

template<class T> T str2any(string s) {
	istringstream iss(s);
	T x;
	iss >> x;
	return x;
}

template<class T> string any2str(T x) {
	ostringstream oss;
	oss << x;
	return oss.str();
}

void test_str2any() {
	cout << str2any<int>("77") << endl;
	cout << str2any<double>("3.1415926") << endl;
}

/*
table 1:
|pk|name|age|score|
|0|alice|21|55.77|
|1|bob|21|89.26|
|2|cathy|19|77.66|
|3|doge|21|64.5|
|4|eddie|21|92.3|

table 2:
|pk|name|grade|
|0|alice|3|
|1|bob|3|
|2|cathy|1|
|3|doge|3|
|4|eddie|3|

tabel 3:
|pk|score_l|score_h|level|
|0|90|100|A|
|1|80|89|B|
|2|70|79|C|
|3|60|69|D|
|4|0|59|F|

table 4:
|pk|mid|
|0|1|
|0|2|

table 5:
|pk|did|mid|
|0|2|1|
|1|3|1|
|2|4|2|

table 6:
|pk|eid|did|
|0|2|2|
|1|3|2|
|2|4|2|

*/

vector<vector<string> > tbl_input(string path) {
	vector<vector<string> > data;
	ifstream fin(path);

	string line;
	while (getline(fin, line)) {
		vector<string> data_line;
		//cout << "OK" << endl;
		vector<int> loc;
		loc.push_back(-1);
		for (int i = 0; i < line.size(); i++)
			if (line[i] == '|')
				loc.push_back(i);
		loc.push_back(line.size());
		//cout << line << endl;
		for (int i = 0; i < loc.size() - 1; i++)
			data_line.push_back(line.substr(loc[i] + 1, loc[i + 1] - loc[i] - 1));

		data.push_back(data_line);
	}
	fin.close();

	return data;
}

class Table {
	private:
		void create_table1() {
			table_name = "t1";
			attr["pk"] = "0";
			attr["name"] = "1";
			attr["age"] = "2";
			attr["score"] = "3";

			type["pk"] = "string";
			type["name"] = "string";
			type["age"] = "int";
			type["score"] = "double";

			const int n = 5;
			num_items = n;

			string *pk[n], *name[n];
			int *age[n];
			double *score[n];

			for (int i = 0; i < n; i++) {
				pk[i] = new string;
				name[i] = new string;
				age[i] = new int;
				score[i] = new double;

				vector<pair<string, void *> > data_i;

				switch (i) {
					case 0:
						*pk[i] = "0";
						*name[i] = "alice";
						*age[i] = 21;
						*score[i] = 55.77;
						break;
					case 1:
						*pk[i] = "1";
						*name[i] = "bob";
						*age[i] = 21;
						*score[i] = 89.26;
						break;
					case 2:
						*pk[i] = "2";
						*name[i] = "cathy";
						*age[i] = 19;
						*score[i] = 77.66;
						break;
					case 3:
						*pk[i] = "3";
						*name[i] = "doge";
						*age[i] = 21;
						*score[i] = 64.5;
						break;
					case 4:
						*pk[i] = "4";
						*name[i] = "eddie";
						*age[i] = 21;
						*score[i] = 92.3;
						break;
				}

				void *ppk = pk[i], *pname = name[i], *page = age[i], *pscore = score[i];
				data_i.push_back(make_pair("string", ppk));
				data_i.push_back(make_pair("string", pname));
				data_i.push_back(make_pair("int", page));
				data_i.push_back(make_pair("double", pscore));
				data.push_back(data_i);
			}

			//age_index
			map<string, vector<string> > age_index;
			lut4idx_tables["age"] = age_index;
			for (int i = 0; i < n; i++) {
				if (!lut4idx_tables["age"].count(any2str(*(int *)data[i][2].second))) {
					vector<string> l;
					l.push_back(*(string *)data[i][0].second);
					lut4idx_tables["age"][any2str(*(int *)data[i][2].second)] = l;
				} else
					lut4idx_tables["age"][any2str(*(int *)data[i][2].second)].push_back(*(string *)data[i][0].second);
			}
			column_index_name.insert("age");

			//name_index
			map<string, vector<string> > name_index;
			lut4idx_tables["name"] = name_index;
			for (int i = 0; i < n; i++) {
				if (!lut4idx_tables["name"].count(any2str(*(string *)data[i][1].second))) {
					vector<string> l;
					l.push_back(*(string *)data[i][0].second);
					lut4idx_tables["name"][any2str(*(string *)data[i][1].second)] = l;
				} else
					lut4idx_tables["name"][any2str(*(string *)data[i][1].second)].push_back(*(string *)data[i][0].second);
			}
			column_index_name.insert("name");
		}

		void create_table2() {
			table_name = "t2";
			attr["pk"] = "0";
			attr["name"] = "1";
			attr["grade"] = "2";

			type["pk"] = "string";
			type["name"] = "string";
			type["grade"] = "int";

			const int n = 5;
			num_items = n;

			string *pk[n], *name[n];
			int *grade[n];

			for (int i = 0; i < n; i++) {
				pk[i] = new string;
				name[i] = new string;
				grade[i] = new int;

				vector<pair<string, void *> > data_i;

				switch (i) {
					case 0:
						*pk[i] = "0";
						*name[i] = "alice";
						*grade[i] = 3;
						break;
					case 1:
						*pk[i] = "1";
						*name[i] = "bob";
						*grade[i] = 3;
						break;
					case 2:
						*pk[i] = "2";
						*name[i] = "cathy";
						*grade[i] = 1;
						break;
					case 3:
						*pk[i] = "3";
						*name[i] = "doge";
						*grade[i] = 3;
						break;
					case 4:
						*pk[i] = "4";
						*name[i] = "eddie";
						*grade[i] = 3;
						break;
				}

				void *ppk = pk[i], *pname = name[i], *pgrade = grade[i];
				data_i.push_back(make_pair("string", ppk));
				data_i.push_back(make_pair("string", pname));
				data_i.push_back(make_pair("int", pgrade));
				data.push_back(data_i);
			}
		}
		void create_table3() {
			table_name = "t3";
			attr["pk"] = "0";
			attr["score_l"] = "1";
			attr["score_h"] = "2";
			attr["level"] = "3";

			type["pk"] = "string";
			type["score_l"] = "double";
			type["score_h"] = "double";
			type["level"] = "string";

			const int n = 5;
			num_items = n;

			string *pk[n];
			double *score_l[n], *score_h[n];
			string *level[n];

			for (int i = 0; i < n; i++) {
				pk[i] = new string;
				score_l[i] = new double;
				score_h[i] = new double;
				level[i] = new string;

				vector<pair<string, void *> > data_i;

				switch (i) {
					case 0:
						*pk[i] = "0";
						*score_l[i] = 90;
						*score_h[i] = 100;
						*level[i] = "A";
						break;
					case 1:
						*pk[i] = "1";
						*score_l[i] = 80;
						*score_h[i] = 89.99;
						*level[i] = "B";
						break;
					case 2:
						*pk[i] = "2";
						*score_l[i] = 70;
						*score_h[i] = 79.99;
						*level[i] = "C";
						break;
					case 3:
						*pk[i] = "3";
						*score_l[i] = 60;
						*score_h[i] = 69.99;
						*level[i] = "D";
						break;
					case 4:
						*pk[i] = "4";
						*score_l[i] = 0;
						*score_h[i] = 59.99;
						*level[i] = "F";
						break;
				}

				void *ppk = pk[i], *pscore_l = score_l[i], *pscore_h = score_h[i], *plevel = level[i];
				data_i.push_back(make_pair("string", ppk));
				data_i.push_back(make_pair("double", pscore_l));
				data_i.push_back(make_pair("double", pscore_h));
				data_i.push_back(make_pair("string", plevel));
				data.push_back(data_i);
			}
		}
		void create_table4() {
			table_name = "t4";
			attr["pk"] = "0";
			attr["mid"] = "1";

			type["pk"] = "string";
			type["mid"] = "int";

			const int n = 2;
			num_items = n;

			string *pk[n];
			int *mid[n];

			for (int i = 0; i < n; i++) {
				pk[i] = new string;
				mid[i] = new int;

				vector<pair<string, void *> > data_i;

				switch (i) {
					case 0:
						*pk[i] = "0";
						*mid[i] = 1;
						break;
					case 1:
						*pk[i] = "1";
						*mid[i] = 2;
						break;
				}

				void *ppk = pk[i], *pmid = mid[i];
				data_i.push_back(make_pair("string", ppk));
				data_i.push_back(make_pair("int", pmid));
				data.push_back(data_i);
			}
		}
		void create_table5() {
			table_name = "t5";
			attr["pk"] = "0";
			attr["did"] = "1";
			attr["mid"] = "2";

			type["pk"] = "string";
			type["did"] = "int";
			type["mid"] = "int";

			const int n = 3;
			num_items = n;

			string *pk[n];
			int *did[n], *mid[n];

			for (int i = 0; i < n; i++) {
				pk[i] = new string;
				did[i] = new int;
				mid[i] = new int;

				vector<pair<string, void *> > data_i;

				switch (i) {
					case 0:
						*pk[i] = "0";
						*did[i] = 2;
						*mid[i] = 1;
						break;
					case 1:
						*pk[i] = "1";
						*did[i] = 3;
						*mid[i] = 1;
						break;
					case 2:
						*pk[i] = "2";
						*did[i] = 4;
						*mid[i] = 2;
						break;
				}

				void *ppk = pk[i], *pdid = did[i], *pmid = mid[i];
				data_i.push_back(make_pair("string", ppk));
				data_i.push_back(make_pair("int", pdid));
				data_i.push_back(make_pair("int", pmid));
				data.push_back(data_i);
			}
		}
		void create_table6() {
			table_name = "t6";
			attr["pk"] = "0";
			attr["eid"] = "1";
			attr["did"] = "2";

			type["pk"] = "string";
			type["eid"] = "int";
			type["did"] = "int";

			const int n = 3;
			num_items = n;

			string *pk[n];
			int *eid[n], *did[n];

			for (int i = 0; i < n; i++) {
				pk[i] = new string;
				eid[i] = new int;
				did[i] = new int;

				vector<pair<string, void *> > data_i;

				switch (i) {
					case 0:
						*pk[i] = "0";
						*eid[i] = 2;
						*did[i] = 2;
						break;
					case 1:
						*pk[i] = "1";
						*eid[i] = 3;
						*did[i] = 2;
						break;
					case 2:
						*pk[i] = "2";
						*eid[i] = 4;
						*did[i] = 2;
						break;
				}

				void *ppk = pk[i], *peid = eid[i], *pdid = did[i];
				data_i.push_back(make_pair("string", ppk));
				data_i.push_back(make_pair("int", peid));
				data_i.push_back(make_pair("int", pdid));
				data.push_back(data_i);
			}
		}
		void create_table7() {
			table_name = "REGION";
			attr["pk"] = "0";
			attr["R_REGIONKEY"] = "1";
			attr["R_NAME"] = "2";
			attr["R_COMMENT"] = "3";

			type["pk"] = "string";
			type["R_REGIONKEY"] = "int";
			type["R_NAME"] = "string";
			type["R_COMMENT"] = "string";

			vector<vector<string> > data_str = tbl_input("region.tbl");
			//cout << "7 OK" << endl;

			for (int i = 0; i < data_str.size(); i++) {
				vector<pair<string, void *> > data_i;

				string *pk = new string;
				int *R_REGIONKEY = new int;
				string *R_NAME = new string;
				string *R_COMMENT = new string;

				*pk = any2str(i);
				*R_REGIONKEY = str2any<int>(data_str[i][0]);
				*R_NAME = data_str[i][1];
				*R_COMMENT = data_str[i][2];

				data_i.push_back(make_pair("string", (void *)pk));
				data_i.push_back(make_pair("int", (void *)R_REGIONKEY));
				data_i.push_back(make_pair("string", (void *)R_NAME));
				data_i.push_back(make_pair("string", (void *)R_COMMENT));

				data.push_back(data_i);
			}

			num_items = data.size();
		}

		void create_table8() {
			table_name = "NATION";

			attr["pk"] = "0";
			attr["N_NATIONKEY"] = "1";
			attr["N_NAME"] = "2";
			attr["N_REGIONKEY"] = "3";
			attr["N_COMMENT"] = "4";

			type["pk"] = "string";
			type["N_NATIONKEY"] = "int";
			type["N_NAME"] = "string";
			type["N_REGIONKEY"] = "int";
			type["N_COMMENT"] = "string";

			vector<vector<string> > data_str = tbl_input("nation.tbl");
			//cout << "7 OK" << endl;

			for (int i = 0; i < data_str.size(); i++) {
				vector<pair<string, void *> > data_i;

				string *pk = new string;
				int *N_NATIONKEY = new int;
				string *N_NAME = new string;
				int *N_REGIONKEY = new int;
				string *N_COMMENT = new string;

				*pk = any2str(i);
				*N_NATIONKEY = str2any<int>(data_str[i][0]);
				*N_NAME = data_str[i][1];
				*N_REGIONKEY = str2any<int>(data_str[i][2]);
				*N_COMMENT = data_str[i][3];

				data_i.push_back(make_pair("string", (void *)pk));
				data_i.push_back(make_pair("int", (void *)N_NATIONKEY));
				data_i.push_back(make_pair("string", (void *)N_NAME));
				data_i.push_back(make_pair("int", (void *)N_REGIONKEY));
				data_i.push_back(make_pair("string", (void *)N_COMMENT));

				data.push_back(data_i);
			}

			num_items = data.size();
		}

		void create_table9() {
			table_name = "SUPPLIER";

			attr["pk"] = "0";
			attr["S_SUPPKEY"] = "1";
			attr["S_NAME"] = "2";
			attr["S_ADDRESS"] = "3";
			attr["S_NATIONKEY"] = "4";
			attr["S_PHONE"] = "5";
			attr["S_ACCTBAL"] = "6";
			attr["S_COMMENT"] = "7";

			type["pk"] = "string";
			type["S_SUPPKEY"] = "int";
			type["S_NAME"] = "string";
			type["S_ADDRESS"] = "string";
			type["S_NATIONKEY"] = "int";
			type["S_PHONE"] = "string";
			type["S_ACCTBAL"] = "float";
			type["S_COMMENT"] = "string";

			vector<vector<string> > data_str = tbl_input("supplier.tbl");
			//cout << "7 OK" << endl;

			for (int i = 0; i < data_str.size(); i++) {
				vector<pair<string, void *> > data_i;

				string *pk = new string;
				int *S_SUPPKEY = new int;
				string *S_NAME = new string;
				string *S_ADDRESS = new string;
				int *S_NATIONKEY = new int;
				string *S_PHONE = new string;
				float *S_ACCTBAL = new float;
				string *S_COMMENT = new string;

				*pk = any2str(i);
				*S_SUPPKEY = str2any<int>(data_str[i][0]);
				*S_NAME = data_str[i][1];
				*S_ADDRESS = data_str[i][2];
				*S_NATIONKEY = str2any<int>(data_str[i][3]);
				*S_PHONE = data_str[i][4];
				*S_ACCTBAL = str2any<float>(data_str[i][5]);
				*S_COMMENT = data_str[i][6];

				data_i.push_back(make_pair("string", (void *)pk));
				data_i.push_back(make_pair("int", (void *)S_SUPPKEY));
				data_i.push_back(make_pair("string", (void *)S_NAME));
				data_i.push_back(make_pair("string", (void *)S_ADDRESS));
				data_i.push_back(make_pair("int", (void *)S_NATIONKEY));
				data_i.push_back(make_pair("string", (void *)S_PHONE));
				data_i.push_back(make_pair("float", (void *)S_ACCTBAL));
				data_i.push_back(make_pair("string", (void *)S_COMMENT));

				data.push_back(data_i);
			}

			num_items = data.size();

			//S_NATIONKEY_index
			map<string, vector<string> > S_NATIONKEY_index;
			lut4idx_tables["S_NATIONKEY"] = S_NATIONKEY_index;
			for (int i = 0; i < num_items; i++) {
				if (!lut4idx_tables["S_NATIONKEY"].count(any2str(*(int *)data[i][4].second))) {
					vector<string> l;
					l.push_back(*(string *)data[i][0].second);
					lut4idx_tables["S_NATIONKEY"][any2str(*(int *)data[i][4].second)] = l;
				} else
					lut4idx_tables["S_NATIONKEY"][any2str(*(int *)data[i][4].second)].push_back(*(string *)data[i][0].second);
			}
			column_index_name.insert("S_NATIONKEY");
		}
		void create_table_metadata(const string &t_name, const vector<pair<string, string> > &columns) {
			table_name = t_name;
			attr["pk"] = "0";
			attr_name.push_back("pk");
			type["pk"] = "string";
			for (int i = 0; i < columns.size(); i++) {
				attr[columns[i].first] = any2str(i + 1);
				attr_name.push_back(columns[i].first);
				type[columns[i].first] = columns[i].second;
			}

			num_items = 0;
		}
		template<class T> void *load_data_ij(const T &value) {
			T *p = new T;
			*p = value;
			return (void *)p;
		}
		void insert_index(const string &column, const string &k, const string &v) {
			if (!column_index_name.count(column))
				return;

			if (!lut4idx_tables[column].count(k)) {
				vector<string> l;
				l.push_back(v);
				lut4idx_tables[column][k] = l;
			} else
				lut4idx_tables[column][k].push_back(v);
		}
		void create_all_index() {
			for (set<string>::iterator i = column_index_name.begin(); i != column_index_name.end(); ++i)
				create_index(*i);
		}
		void transection_backup_write_ij(ofstream &fout, const int i, const int j) {
			string type = data[i][j].first;
			if (strcmp(type.c_str(), "int") == 0)
				fout << *(int *)data[i][j].second;
			else if (strcmp(type.c_str(), "long") == 0)
				fout << *(long long *)data[i][j].second;
			else if (strcmp(type.c_str(), "float") == 0)
				fout << *(float *)data[i][j].second;
			else if (strcmp(type.c_str(), "double") == 0)
				fout << *(double *)data[i][j].second;
			else
				fout << *(string *)data[i][j].second;
		}
		void transection_backup_write_i(ofstream &fout, const int i) {
			transection_backup_write_ij(fout, i, 1);
			for (int j = 2; j < data[i].size(); j++) {
				fout << '|';
				transection_backup_write_ij(fout, i, j);
			}
		}
	public:
		int block_no;
		map<string, string> attr;
		vector<string> attr_name;
		map<string, string> type;
		vector<vector<pair<string, void *> > > data;
		set<string>column_index_name;
		map<string, map<string, vector<string> > > lut4idx_tables;
		int num_items;
		string table_name;
		Table(const string &t_name, const vector<pair<string, string> > &columns) {
			create_table_metadata(t_name, columns);
		}
		Table(int x) {
			switch (x) {
				case 1:
					create_table1();
					break;
				case 2:
					create_table2();
					break;
				case 3:
					create_table3();
					break;
				case 4:
					create_table4();
					break;
				case 5:
					create_table5();
					break;
				case 6:
					create_table6();
					break;
				case 7:
					create_table7();
					break;
				case 8:
					create_table8();
					break;
				case 9:
					create_table9();
					break;
			}
		}
		void create_index(const string &column) {
			map<string, vector<string> > index_column;

			if (!column_index_name.count(column))
				column_index_name.insert(column);

			lut4idx_tables[column] = index_column;

			string type_ij = type[column];
			int j = str2any<int>(attr[column]);
			for (int i = 0; i < num_items; i++) {
				string k, v;

				if (strcmp(type_ij.c_str(), "int") == 0)
					k = any2str(*(int *)data[i][j].second);
				else if (strcmp(type_ij.c_str(), "long") == 0)
					k = any2str(*(long long *)data[i][j].second);
				else if (strcmp(type_ij.c_str(), "float") == 0)
					k = any2str(*(float *)data[i][j].second);
				else if (strcmp(type_ij.c_str(), "double") == 0)
					k = any2str(*(double *)data[i][j].second);
				else
					k = any2str(*(string *)data[i][j].second);

				v = *(string *)data[i][0].second;

				if (!lut4idx_tables[column].count(k)) {
					vector<string> l;
					l.push_back(v);
					lut4idx_tables[column][k] = l;
				} else
					lut4idx_tables[column][k].push_back(v);
			}
		}
		void load_data(const string &path) {
			vector<vector<string> > data_str = tbl_input(path);
			cout << data_str.size() << endl;
			for (int i = 0; i < data_str.size(); i++) {
				vector<pair<string, void *> > data_i;

				data_i.push_back(make_pair("string", load_data_ij(any2str(i))));
				for (int j = 1; j < attr_name.size(); j++) {
					string type_ij = type[attr_name[j]];

					if (strcmp(type_ij.c_str(), "int") == 0)
						data_i.push_back(make_pair(type_ij, load_data_ij(str2any<int>(data_str[i][j - 1]))));
					else if (strcmp(type_ij.c_str(), "long") == 0)
						data_i.push_back(make_pair(type_ij, load_data_ij(str2any<long long>(data_str[i][j - 1]))));
					else if (strcmp(type_ij.c_str(), "float") == 0)
						data_i.push_back(make_pair(type_ij, load_data_ij(str2any<float>(data_str[i][j - 1]))));
					else if (strcmp(type_ij.c_str(), "double") == 0)
						data_i.push_back(make_pair(type_ij, load_data_ij(str2any<double>(data_str[i][j - 1]))));
					else
						data_i.push_back(make_pair(type_ij, load_data_ij(data_str[i][j - 1])));
				}

				data.push_back(data_i);
			}

			num_items = data.size();
			create_all_index();
		}
		void insert(const vector<string> &data_str_i) {
			vector<pair<string, void *> > data_i;

			data_i.push_back(make_pair("string", load_data_ij(any2str(num_items))));
			for (int j = 1; j < attr_name.size(); j++) {
				string type_ij = type[attr_name[j]];

				if (strcmp(type_ij.c_str(), "int") == 0)
					data_i.push_back(make_pair(type_ij, load_data_ij(str2any<int>(data_str_i[j - 1]))));
				else if (strcmp(type_ij.c_str(), "long") == 0)
					data_i.push_back(make_pair(type_ij, load_data_ij(str2any<long long>(data_str_i[j - 1]))));
				else if (strcmp(type_ij.c_str(), "float") == 0)
					data_i.push_back(make_pair(type_ij, load_data_ij(str2any<float>(data_str_i[j - 1]))));
				else if (strcmp(type_ij.c_str(), "double") == 0)
					data_i.push_back(make_pair(type_ij, load_data_ij(str2any<double>(data_str_i[j - 1]))));
				else
					data_i.push_back(make_pair(type_ij, load_data_ij(data_str_i[j - 1])));

				insert_index(attr_name[j], data_str_i[j - 1], any2str(num_items));
			}

			data.push_back(data_i);
			num_items++;
		}
		void update_j(const vector<int> &upd_pk, const string &column, const string &value) {
			string type_ij = type[column];
			int j = str2any<int>(attr[column]);

			for (int upd_pk_i = 0; upd_pk_i < upd_pk.size(); upd_pk_i++) {
				int i = upd_pk[upd_pk_i];

				if (strcmp(type_ij.c_str(), "int") == 0)
					data[i][j].second = load_data_ij(str2any<int>(value));
				else if (strcmp(type_ij.c_str(), "long") == 0)
					data[i][j].second = load_data_ij(str2any<long long>(value));
				else if (strcmp(type_ij.c_str(), "float") == 0)
					data[i][j].second = load_data_ij(str2any<float>(value));
				else if (strcmp(type_ij.c_str(), "double") == 0)
					data[i][j].second = load_data_ij(str2any<double>(value));
				else
					data[i][j].second = load_data_ij(value);
			}

			create_index(column);
		}
		void del(const vector<int> &del_pk) {
			vector<vector<pair<string, void *> > > left_data;
			for (int i = 0; i < del_pk.size(); i++) {
				string *pk_i = (string *)(data[del_pk[i]][0].second);
				*pk_i = "-1";
			}

			for (int i = 0; i < num_items; i++) {
				string *pk_i = (string *)(data[i][0].second);
				if (*pk_i != "-1")
					left_data.push_back(data[i]);
				else
					for (int j = 0; j < data[i].size(); j++) {
						string type_ij = data[i][j].first;
						void *data_ij = data[i][j].second;

						int *value_i = NULL;
						long long *value_ll = NULL;
						float *value_f = NULL;
						double *value_d = NULL;
						string *value_s = NULL;
						if (strcmp(type_ij.c_str(), "int") == 0)
							value_i = (int *)data_ij;
						else if (strcmp(type_ij.c_str(), "long") == 0)
							value_ll = (long long *)data_ij;
						else if (strcmp(type_ij.c_str(), "float") == 0)
							value_f = (float *)data_ij;
						else if (strcmp(type_ij.c_str(), "double") == 0)
							value_d = (double *)data_ij;
						else
							value_s = (string *)data_ij;

						if (strcmp(type_ij.c_str(), "int") == 0)
							delete value_i;
						else if (strcmp(type_ij.c_str(), "long") == 0)
							delete value_ll;
						else if (strcmp(type_ij.c_str(), "float") == 0)
							delete value_f;
						else if (strcmp(type_ij.c_str(), "double") == 0)
							delete value_d;
						else
							delete value_s;
					}
			}

			num_items -= del_pk.size();
			data.clear();

			for (int i = 0; i < num_items; i++) {
				string *pk_i = (string *)(left_data[i][0].second);
				*pk_i = any2str(i);
				data.push_back(left_data[i]);
			}

			create_all_index();
		}

		void drop() {
			for (int i = 0; i < num_items; i++) {
				for (int j = 0; j < data[i].size(); j++) {
					string type_ij = data[i][j].first;
					void *data_ij = data[i][j].second;

					int *value_i = NULL;
					long long *value_ll = NULL;
					float *value_f = NULL;
					double *value_d = NULL;
					string *value_s = NULL;
					if (strcmp(type_ij.c_str(), "int") == 0)
						value_i = (int *)data_ij;
					else if (strcmp(type_ij.c_str(), "long") == 0)
						value_ll = (long long *)data_ij;
					else if (strcmp(type_ij.c_str(), "float") == 0)
						value_f = (float *)data_ij;
					else if (strcmp(type_ij.c_str(), "double") == 0)
						value_d = (double *)data_ij;
					else
						value_s = (string *)data_ij;

					if (strcmp(type_ij.c_str(), "int") == 0)
						delete value_i;
					else if (strcmp(type_ij.c_str(), "long") == 0)
						delete value_ll;
					else if (strcmp(type_ij.c_str(), "float") == 0)
						delete value_f;
					else if (strcmp(type_ij.c_str(), "double") == 0)
						delete value_d;
					else
						delete value_s;
				}
				data[i].clear();
			}
			data.clear();
			num_items = 0;
		}

		void transection_backup_write(const string &path) {
			ofstream fout(path);

			transection_backup_write_i(fout, 0);
			for (int i = 1; i < data.size(); i++) {
				fout << endl;
				transection_backup_write_i(fout, i);
			}

			fout.close();
		}

		void print_self() {
			cout << "num_items = " << num_items << endl;
			for (int i = 0; i < attr_name.size(); i++)
				cout << "|" << attr[attr_name[i]] << ' ' << attr_name[i] << ' ' << type[attr_name[i]];
			cout << "|" << endl;

			/*for (map<string, string>::iterator i = attr.begin(); i != attr.end(); ++i)
				cout << "|" << i->first;
			cout << "|" << endl;*/

			for (int i = 0; i < data.size(); i++) {
				for (int j = 0; j < data[i].size(); j++) {
					string type = data[i][j].first;
					if (strcmp(type.c_str(), "int") == 0)
						cout << "|" << *(int *)data[i][j].second;
					else if (strcmp(type.c_str(), "long") == 0)
						cout << "|" << *(long long *)data[i][j].second;
					else if (strcmp(type.c_str(), "float") == 0)
						cout << "|" << *(float *)data[i][j].second;
					else if (strcmp(type.c_str(), "double") == 0)
						cout << "|" << *(double *)data[i][j].second;
					else
						cout << "|" << *(string *)data[i][j].second;
				}
				cout << "|" << endl;
			}

			cout << endl << "column_index_name:" << endl;
			for (set<string>::iterator i = column_index_name.begin(); i != column_index_name.end(); ++i)
				cout << *i << ' ';
			cout << endl;

			cout << endl;
			for (map<string, map<string, vector<string> > >::iterator i = lut4idx_tables.begin(); i != lut4idx_tables.end(); ++i)
				for (map<string, vector<string> >::iterator j = i->second.begin(); j != i->second.end(); ++j) {
					cout << "lut4idx_tables[" << i->first << "][" << j->first << "]:";
					for (int k = 0; k < j->second.size(); k++)
						cout << ' ' << j->second[k];
					cout << endl;
				}
		}
};

template<class T> T call_get_value(Table *t, int pk, string var_name) {
	int j = str2any<int>(t->attr[var_name]);

	string type = (t->data[pk][j]).first;
	void *p = (t->data[pk][j]).second;
	return *(T *)p;
	//else return regex_replace(string(buffer_reader, get<2>(attr_dict[var_name])), redundant, "");
}

template<class T> class spj_condition_obj { //һ��Ԫ�ص���
	public:
		int var_con; //0:��һ���У�1:��һ������
		Table *table; //��ָ�루������0��
		string var_name; //������������0��
		T value; //������������1��
};

template<class T> class spj_condition { //һ��where�Ӿ���￿
	public:
		spj_condition_obj<T> a, b; //Ԫ��a, b
		string op; //������������￿��<>��<��<=��>��>=
};

template<class T> pair<spj_condition_obj<T>, spj_condition_obj<T>> get_spj_condition_obj(void *spj_c) {
	return make_pair((*(spj_condition<T> *)spj_c).a, (*(spj_condition<T> *)spj_c).b);
}

string get_spj_condition_op(pair<string, void *> expr) {
	string type = expr.first;

	if (strcmp(type.c_str(), "int") == 0)
		return (*(spj_condition<int> *)expr.second).op;
	else if (strcmp(type.c_str(), "long") == 0)
		return (*(spj_condition<long long> *)expr.second).op;
	else if (strcmp(type.c_str(), "float") == 0)
		return (*(spj_condition<float> *)expr.second).op;
	else if (strcmp(type.c_str(), "double") == 0)
		return (*(spj_condition<double> *)expr.second).op;
	else
		return (*(spj_condition<string> *)expr.second).op;
}

int get_spj_condition_obj_var_con(pair<string, void *> expr, int ab) { //0:a; 1:b
	string type = expr.first;
	if (strcmp(type.c_str(), "int") == 0) {
		pair<spj_condition_obj<int>, spj_condition_obj<int> > o = get_spj_condition_obj<int>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_con;
			case 1:
				return o.second.var_con;
		}
	} else if (strcmp(type.c_str(), "long") == 0) {
		pair<spj_condition_obj<long long>, spj_condition_obj<long long> > o = get_spj_condition_obj<long long>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_con;
			case 1:
				return o.second.var_con;
		}
	} else if (strcmp(type.c_str(), "float") == 0) {
		pair<spj_condition_obj<float>, spj_condition_obj<float> > o = get_spj_condition_obj<float>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_con;
			case 1:
				return o.second.var_con;
		}
	} else if (strcmp(type.c_str(), "double") == 0) {
		pair<spj_condition_obj<double>, spj_condition_obj<double> > o = get_spj_condition_obj<double>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_con;
			case 1:
				return o.second.var_con;
		}
	} else {
		pair<spj_condition_obj<string>, spj_condition_obj<string> > o = get_spj_condition_obj<string>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_con;
			case 1:
				return o.second.var_con;
		}
	}
}

Table *get_spj_condition_obj_table(pair<string, void *> expr, int ab) { //0:a; 1:b
	string type = expr.first;
	if (strcmp(type.c_str(), "int") == 0) {
		pair<spj_condition_obj<int>, spj_condition_obj<int> > o = get_spj_condition_obj<int>(expr.second);
		switch (ab) {
			case 0:
				return o.first.table;
			case 1:
				return o.second.table;
		}
	} else if (strcmp(type.c_str(), "long") == 0) {
		pair<spj_condition_obj<long long>, spj_condition_obj<long long> > o = get_spj_condition_obj<long long>(expr.second);
		switch (ab) {
			case 0:
				return o.first.table;
			case 1:
				return o.second.table;
		}
	} else if (strcmp(type.c_str(), "float") == 0) {
		pair<spj_condition_obj<float>, spj_condition_obj<float> > o = get_spj_condition_obj<float>(expr.second);
		switch (ab) {
			case 0:
				return o.first.table;
			case 1:
				return o.second.table;
		}
	} else if (strcmp(type.c_str(), "double") == 0) {
		pair<spj_condition_obj<double>, spj_condition_obj<double> > o = get_spj_condition_obj<double>(expr.second);
		switch (ab) {
			case 0:
				return o.first.table;
			case 1:
				return o.second.table;
		}
	} else {
		pair<spj_condition_obj<string>, spj_condition_obj<string> > o = get_spj_condition_obj<string>(expr.second);
		switch (ab) {
			case 0:
				return o.first.table;
			case 1:
				return o.second.table;
		}
	}
}

string get_spj_condition_obj_var_name(pair<string, void *> expr, int ab) { //0:a; 1:b
	string type = expr.first;
	if (strcmp(type.c_str(), "int") == 0) {
		pair<spj_condition_obj<int>, spj_condition_obj<int> > o = get_spj_condition_obj<int>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_name;
			case 1:
				return o.second.var_name;
		}
	} else if (strcmp(type.c_str(), "long") == 0) {
		pair<spj_condition_obj<long long>, spj_condition_obj<long long> > o = get_spj_condition_obj<long long>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_name;
			case 1:
				return o.second.var_name;
		}
	} else if (strcmp(type.c_str(), "float") == 0) {
		pair<spj_condition_obj<float>, spj_condition_obj<float> > o = get_spj_condition_obj<float>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_name;
			case 1:
				return o.second.var_name;
		}
	} else if (strcmp(type.c_str(), "double") == 0) {
		pair<spj_condition_obj<double>, spj_condition_obj<double> > o = get_spj_condition_obj<double>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_name;
			case 1:
				return o.second.var_name;
		}
	} else {
		pair<spj_condition_obj<string>, spj_condition_obj<string> > o = get_spj_condition_obj<string>(expr.second);
		switch (ab) {
			case 0:
				return o.first.var_name;
			case 1:
				return o.second.var_name;
		}
	}
}

template<class T> T get_spj_condition_obj_value(pair<string, void *> expr, int ab) { //0:a; 1:b
	string type = expr.first;

	pair<spj_condition_obj<T>, spj_condition_obj<T> > o = get_spj_condition_obj<T>(expr.second);
	switch (ab) {
		case 0:
			return o.first.value;
		case 1:
			return o.second.value;
	}
}

template<class T> bool spj_condition_tf(T a, string op, T b) {
	//cout << a << op << b << endl;
	if (op == "=")
		return a == b;
	else if (op == "<>")
		return a != b;
	else if (op == "<")
		return a < b;
	else if (op == "<=")
		return a <= b;
	else if (op == ">")
		return a > b;
	else if (op == ">=")
		return a >= b;

	return false;
}

template<class T> vector<int> *spj_select_get_arr_index(Table *t, string var_name, T key) {
	vector<int> *arr = new vector<int>;
	cout << "Index(" << var_name << "):" << endl;
	if (!t->lut4idx_tables[var_name].count(any2str(key))) {
		cout << "NULL" << endl;
		return arr;
	}
	for (const auto &pk : (*t).lut4idx_tables[var_name][any2str(key)]) {
		cout << pk << endl;
		(*arr).push_back(str2any<int>(pk));
	}

	return arr;
}

template<class T> vector<int> *spj_select_get_arr_loop(Table *t, string var_name, T key, string type, string op,
        int var_loc) {
	vector<int> *arr = new vector<int>;
	cout << "loop(" << var_name << "):" << endl;
	for (int j = 0; j < t->num_items; j++) {
		bool tf = false;
		if (var_loc)
			tf = spj_condition_tf(key, op, call_get_value<T>(t, j, var_name));
		else
			tf = spj_condition_tf(call_get_value<T>(t, j, var_name), op, key);

		if (tf) {
			cout << j << endl;
			(*arr).push_back(j);
		}
	}

	return arr;
}

vector<int> *spj_select(Table *t, vector<pair<string, void *> > exprs,
                        string conj) { //���ص���item������
	vector<vector<int> *> *arrs = new vector<vector<int> *>;

	for (int i = 0; i < exprs.size(); i++) {
		vector<int> *arr = new vector<int>;

		int var_loc = -1;
		//cout << exprs[i].first << endl;
		if (!get_spj_condition_obj_var_con(exprs[i], 0))
			var_loc = 0;
		else if (!get_spj_condition_obj_var_con(exprs[i], 1))
			var_loc = 1;

		string type = exprs[i].first;
		string op = get_spj_condition_op(exprs[i]);

		if (var_loc == -1) {
			int key_i[2];
			long long key_ll[2];
			float key_f[2];
			double key_d[2];
			string key_s[2];
			for (int j = 0; j <= 1; j++) {
				if (strcmp(type.c_str(), "int") == 0)
					key_i[j] = get_spj_condition_obj_value<int>(exprs[i], j);
				else if (strcmp(type.c_str(), "long") == 0)
					key_ll[j] = get_spj_condition_obj_value<long long>(exprs[i], j);
				else if (strcmp(type.c_str(), "float") == 0)
					key_f[j] = get_spj_condition_obj_value<float>(exprs[i], j);
				else if (strcmp(type.c_str(), "double") == 0)
					key_d[j] = get_spj_condition_obj_value<double>(exprs[i], j);
				else
					key_s[j] = get_spj_condition_obj_value<string>(exprs[i], j);
			}

			bool tf = false;
			if (strcmp(type.c_str(), "int") == 0)
				tf = spj_condition_tf(key_i[0], op, key_i[1]);
			else if (strcmp(type.c_str(), "long") == 0)
				tf = spj_condition_tf(key_ll[0], op, key_ll[1]);
			else if (strcmp(type.c_str(), "float") == 0)
				tf = spj_condition_tf(key_f[0], op, key_f[1]);
			else if (strcmp(type.c_str(), "double") == 0)
				tf = spj_condition_tf(key_d[0], op, key_d[1]);
			else
				tf = spj_condition_tf(key_s[0], op, key_s[1]);
			//cout << "OK" << endl;
			if (tf) {
				cout << "Const_expr True" << endl;
				for (int j = 0; j < t->num_items; j++)
					(*arr).push_back(j);
			} else
				cout << "Const_expr False" << endl;

			(*arrs).push_back(arr);
			continue;
		}

		string var_name = get_spj_condition_obj_var_name(exprs[i], var_loc);
		//cout << var_loc << endl;

		int key_i;
		long long key_ll;
		float key_f;
		double key_d;
		string key_s;
		if (strcmp(type.c_str(), "int") == 0)
			key_i = get_spj_condition_obj_value<int>(exprs[i], var_loc ^ 1);
		else if (strcmp(type.c_str(), "long") == 0)
			key_ll = get_spj_condition_obj_value<long long>(exprs[i], var_loc ^ 1);
		else if (strcmp(type.c_str(), "float") == 0)
			key_f = get_spj_condition_obj_value<float>(exprs[i], var_loc ^ 1);
		else if (strcmp(type.c_str(), "double") == 0)
			key_d = get_spj_condition_obj_value<double>(exprs[i], var_loc ^ 1);
		else
			key_s = get_spj_condition_obj_value<string>(exprs[i], var_loc ^ 1);

		if ((*t).column_index_name.count(var_name) && op == "=") {
			if (strcmp(type.c_str(), "int") == 0)
				arr = spj_select_get_arr_index(t, var_name, key_i);
			else if (strcmp(type.c_str(), "long") == 0)
				arr = spj_select_get_arr_index(t, var_name, key_ll);
			else if (strcmp(type.c_str(), "float") == 0)
				arr = spj_select_get_arr_index(t, var_name, key_f);
			else if (strcmp(type.c_str(), "double") == 0)
				arr = spj_select_get_arr_index(t, var_name, key_d);
			else
				arr = spj_select_get_arr_index(t, var_name, key_s);
		} else {
			if (strcmp(type.c_str(), "int") == 0)
				arr = spj_select_get_arr_loop(t, var_name, key_i, type, op, var_loc);
			else if (strcmp(type.c_str(), "long") == 0)
				arr = spj_select_get_arr_loop(t, var_name, key_ll, type, op, var_loc);
			else if (strcmp(type.c_str(), "float") == 0)
				arr = spj_select_get_arr_loop(t, var_name, key_f, type, op, var_loc);
			else if (strcmp(type.c_str(), "double") == 0)
				arr = spj_select_get_arr_loop(t, var_name, key_d, type, op, var_loc);
			else
				arr = spj_select_get_arr_loop(t, var_name, key_s, type, op, var_loc);
		}

		(*arrs).push_back(arr);
	}

	vector<int> *res;
	if (!exprs.size()) {
		res = new vector<int>;
		for (int i = 0; i < t->num_items; i++)
			(*res).push_back(i);
	} else if (conj == "AND")
		res = kmerge_intersection(arrs);
	else if (conj == "OR")
		res = kmerge_union(arrs);
	else {
		res = new vector<int>;
//		cout << "arrs[0]:" << endl;
		for (int i = 0; i < (*(*arrs)[0]).size(); i++) {
			(*res).push_back((*(*arrs)[0])[i]);
//			cout << (*(*arrs)[0])[i] << endl;
		}
	}

	//GC
	for (int i = 0; i < (*arrs).size(); i++)
		delete (*arrs)[i];
	delete arrs;

//	cout << "res:" << endl;
//	for (int i = 0; i < (*res).size(); i++)
//		cout << (*res)[i] << ' ';
//	cout << endl;
	return res;
}

vector<vector<pair<string, void *> > *> *spj_project(Table *t, vector<string> *var_names,
        vector<int> *pk) { //string��ʾ����, void *ָ������
	vector<vector<pair<string, void *> > *> *arrs = new vector<vector<pair<string, void *> > *>;

	for (int i = 0; i < (*pk).size(); i++) {
		vector<pair<string, void *> > *arr = new vector<pair<string, void *> >;
		(*arrs).push_back(arr);
	}

	for (int j = 0; j < (*var_names).size(); j++) {
		string type = t->type[(*var_names)[j]];
		for (int i = 0; i < (*pk).size(); i++) {
			vector<pair<string, void *> > *arr = (*arrs)[i];

			int *value_i = new int;
			long long *value_ll = new long long;
			float *value_f = new float;
			double *value_d = new double;
			string *value_s = new string;
			if (strcmp(type.c_str(), "int") == 0)
				*value_i = call_get_value<int>(t, (*pk)[i], (*var_names)[j]);
			else if (strcmp(type.c_str(), "long") == 0)
				*value_ll = call_get_value<long long>(t, (*pk)[i], (*var_names)[j]);
			else if (strcmp(type.c_str(), "float") == 0)
				*value_f = call_get_value<float>(t, (*pk)[i], (*var_names)[j]);
			else if (strcmp(type.c_str(), "double") == 0)
				*value_d = call_get_value<double>(t, (*pk)[i], (*var_names)[j]);
			else *value_s = call_get_value<string>(t, (*pk)[i], (*var_names)[j]);

			void *value = NULL;
			if (strcmp(type.c_str(), "int") == 0)
				value = value_i;
			else if (strcmp(type.c_str(), "long") == 0)
				value = value_ll;
			else if (strcmp(type.c_str(), "float") == 0)
				value = value_f;
			else if (strcmp(type.c_str(), "double") == 0)
				value = value_d;
			else
				value = value_s;

			(*arr).push_back(make_pair(type, value));
		}
	}

	return arrs;
}

vector<vector<pair<string, void *> > *> *select_project(Table *t, vector<string> *var_names,
        vector<pair<string, void *> > exprs, string conj) {
	vector<int> *pk = spj_select(t, exprs, conj);
	return spj_project(t, var_names, pk);
}

string output_select_project(Table *t, vector<string> *var_names, vector<pair<string, void *> > exprs, string conj) {
	vector<vector<pair<string, void *> > *> *arrs = select_project(t, var_names, exprs, conj);
	ostringstream oss;

	for (int j = 0; j < (*var_names).size(); j++)
		oss << "| " << (*var_names)[j];
	oss << " |\n";

	for (int i = 0; i < (*arrs).size(); i++) {
		vector<pair<string, void *> > *arr = (*arrs)[i];
		for (int j = 0; j < (*arr).size(); j++) {
			string type = (*arr)[j].first;
			void *data = (*arr)[j].second;
			if (strcmp(type.c_str(), "int") == 0)
				oss << "| " << *(int *)data;
			else if (strcmp(type.c_str(), "long") == 0)
				oss << "| " << *(long long *)data;
			else if (strcmp(type.c_str(), "float") == 0)
				oss << "| " << *(float *)data;
			else if (strcmp(type.c_str(), "double") == 0)
				oss << "| " << *(double *)data;
			else
				oss << "| " << *(string *)data;
		}
		oss << " |\n";
	}

	//GC
	for (int i = 0; i < (*arrs).size(); i++) {
		vector<pair<string, void *> > *arr = (*arrs)[i];
		for (int j = 0; j < (*arr).size(); j++) {
			string type = (*arr)[j].first;
			void *data = (*arr)[j].second;

			int *value_i = NULL;
			long long *value_ll = NULL;
			float *value_f = NULL;
			double *value_d = NULL;
			string *value_s = NULL;
			if (strcmp(type.c_str(), "int") == 0)
				value_i = (int *)data;
			else if (strcmp(type.c_str(), "long") == 0)
				value_ll = (long long *)data;
			else if (strcmp(type.c_str(), "float") == 0)
				value_f = (float *)data;
			else if (strcmp(type.c_str(), "double") == 0)
				value_d = (double *)data;
			else
				value_s = (string *)data;

			if (strcmp(type.c_str(), "int") == 0)
				delete value_i;
			else if (strcmp(type.c_str(), "long") == 0)
				delete value_ll;
			else if (strcmp(type.c_str(), "float") == 0)
				delete value_f;
			else if (strcmp(type.c_str(), "double") == 0)
				delete value_d;
			else
				delete value_s;
		}
		delete arr;
	}
	delete arrs;

	return oss.str();
}

void test_select_project() {
	Table *t = new Table(1);
	t->print_self();

	vector<string> var_names;
	//var_names.push_back("pk");
	var_names.push_back("name");
	//var_names.push_back("age");
	//var_names.push_back("score");

	vector<pair<string, void *> > exprs;

	spj_condition_obj<int> age_l;
	age_l.var_con = 0;
	age_l.table = t;
	age_l.var_name = "age";

	spj_condition_obj<int> age_r;
	age_r.var_con = 1;
	age_r.value = 21;

	spj_condition<int> age_expr;
	age_expr.a = age_r;
	age_expr.b = age_l;
	age_expr.op = "=";

	spj_condition_obj<double> score_l;
	score_l.var_con = 0;
	score_l.table = t;
	score_l.var_name = "score";

	spj_condition_obj<double> score_r;
	score_r.var_con = 1;
	score_r.value = 90;

	spj_condition<double> score_expr;
	score_expr.a = score_r;
	score_expr.b = score_l;
	score_expr.op = "<";

	spj_condition_obj<int> happy_l;
	happy_l.var_con = 1;
	happy_l.value = 3;

	spj_condition_obj<int> happy_r;
	happy_r.var_con = 1;
	happy_r.value = 5;

	spj_condition<int> happy;
	happy.a = happy_r;
	happy.b = happy_l;
	happy.op = ">";

	exprs.push_back(make_pair("double", &score_expr));
	exprs.push_back(make_pair("int", &age_expr));
	exprs.push_back(make_pair("int", &happy));

	string conj = "AND";

	//vector<int> *pk = spj_select(t, exprs, conj);
	cout << output_select_project(t, &var_names, exprs, conj) << endl;
}

void spj_alljoin(int i, vector<map<int, vector<int> *> *> each_join, vector<pair<Table *, vector <int> > > &res,
                 vector<int> pk) {
	if (i >= each_join.size()) {
		for (int j = 0; j <= i; j++)
			res[j].second.push_back(pk[j]);
		return;
	}

	if (i == 0) {
		pk.push_back(-1);
		pk.push_back(-1);
		for (map<int, vector<int> *>::iterator j = (*each_join[i]).begin(); j != (*each_join[i]).end(); ++j) {
			pk[0] = j->first;
			vector<int> *nex = j->second;
			for (int k = 0; k < (*nex).size(); k++) {
				pk[1] = (*nex)[k];
				spj_alljoin(i + 1, each_join, res, pk);
			}

		}
	} else {
		pk.push_back(-1);
		vector<int> *nex = (*each_join[i])[pk[i]];
		for (int k = 0; k < (*nex).size(); k++) {
			pk[i + 1] = (*nex)[k];
			spj_alljoin(i + 1, each_join, res, pk);
		}
	}
}
/*
join 1:
1 2
1 3
2 4

join 2:
2 2
2 3
2 4
*/
void test_alljoin() {
	vector<map<int, vector<int> *> *> each_join;

	map<int, vector<int> *> *join1 = new map<int, vector<int> *>;
	(*join1)[1] = new vector<int>;
	(*(*join1)[1]).push_back(2);
	(*(*join1)[1]).push_back(3);
	(*join1)[2] = new vector<int>;
	(*(*join1)[2]).push_back(4);
	each_join.push_back(join1);

	map<int, vector<int> *> *join2 = new map<int, vector<int> *>;
	(*join2)[2] = new vector<int>;
	(*join2)[3] = new vector<int>;
	(*join2)[4] = new vector<int>;
	(*(*join2)[2]).push_back(2);
	(*(*join2)[2]).push_back(3);
	(*(*join2)[2]).push_back(4);
	each_join.push_back(join2);

	//print
	for (int i = 0; i < each_join.size(); i++) {
		cout << "join" << i << ":" << endl;
		for (map<int, vector<int> *>::iterator j = (*each_join[i]).begin(); j != (*each_join[i]).end(); ++j) {
			vector<int> *v = j->second;
			for (int k = 0; k < (*v).size(); k++)
				cout << j->first << ' ' << (*v)[k] << endl;
		}
		cout << endl;
	}

	vector<pair<Table *, vector<int> > > res;

	for (int i = 1; i <= 3; i++) {
		vector<int> resinull;
		Table *ti = (Table *)i;
		res.push_back(make_pair(ti, resinull));
	}

	vector<int> alljoin_pk;
	spj_alljoin(0, each_join, res, alljoin_pk);

	//print res
	for (int i = 0; i < res.size(); i++)
		cout << "|t" << res[i].first;
	cout << "|" << endl;

	for (int j = 0; j < res[0].second.size(); j++) {
		for (int i = 0; i < res.size(); i++)
			cout << "|" << res[i].second[j];
		cout << "|" << endl;
	}

//GC
	for (int i = 0; i < each_join.size(); i++) {
		for (map<int, vector<int> *>::iterator j = (*each_join[i]).begin(); j != (*each_join[i]).end(); ++j)
			delete j->second;
		delete each_join[i];
	}
}

template<class T> map<int, vector<int> *> *nested_join(pair<Table *, Table *> t, pair<vector<int> *, vector<int> *> pk,
        pair<string, string> var_name, string op) {
	//call_get_value<int>(t, (*pk)[i], (*var_names)[j])
	Table *t1 = t.first, *t2 = t.second;
	vector<int> *pk1 = pk.first, *pk2 = pk.second;
	string var_name1 = var_name.first, var_name2 = var_name.second;

	map<int, vector<int> *> *res = new map<int, vector<int> *>;

	cout << "NestedJoin(" << t1->table_name << ',' << t2->table_name << ")" << endl;

	for (int i = 0; i < (*pk1).size(); i++) {
		vector<int> *resi = new vector<int>;
		(*res)[(*pk1)[i]] = resi;
	}

	for (int i = 0; i < (*pk1).size(); i++)
		for (int j = 0; j < (*pk2).size(); j++) {
			int l = (*pk1)[i], r = (*pk2)[j];
			if (op == "")
				(*(*res)[l]).push_back(r);
			else {
				T value1 = call_get_value<T>(t1, l, var_name1);
				T value2 = call_get_value<T>(t2, r, var_name2);
				if (spj_condition_tf<T>(value1, op, value2))
					(*(*res)[l]).push_back(r);
			}
		}

	return res;
}

template<class T> map<int, vector<int> *> *hash_join(pair<Table *, Table *> t, pair<vector<int> *, vector<int> *> pk,
        pair<string, string> var_name, string op) {
	//call_get_value<int>(t, (*pk)[i], (*var_names)[j])
	Table *t1 = t.first, *t2 = t.second;
	vector<int> *pk1 = pk.first, *pk2 = pk.second;
	string var_name1 = var_name.first, var_name2 = var_name.second;

	map<int, vector<int> *> *res = new map<int, vector<int> *>;

	set<int> *pk1_4hash = new set<int>;
	set<int> *pk2_4hash = new set<int>;

	for (int i = 0; i < (*pk1).size(); i++)
		(*pk1_4hash).insert((*pk1)[i]);
	for (int i = 0; i < (*pk2).size(); i++)
		(*pk2_4hash).insert((*pk2)[i]);

	for (int i = 0; i < (*pk1).size(); i++) {
		vector<int> *resi = new vector<int>;
		(*res)[(*pk1)[i]] = resi;
	}

	if ((*t1).column_index_name.count(var_name1) && op == "=") { //index on pk1
		cout << "HashJoin(" << t2->table_name << "->" << t1->table_name << ")" << endl;
		for (int i = 0; i < (*pk2).size(); i++) {
			int l = (*pk2)[i];
			T key = call_get_value<T>(t2, l, var_name2);
			for (const auto &j : (*t1).lut4idx_tables[var_name1][any2str(key)]) {
				int r = str2any<int>(j);
				if ((*pk1_4hash).count(r))
					(*(*res)[r]).push_back(l);
			}
		}
	} else if ((*t2).column_index_name.count(var_name2) && op == "=") { //index on pk2
		cout << "HashJoin(" << t1->table_name << "->" << t2->table_name << ")" << endl;
		for (int i = 0; i < (*pk1).size(); i++) {
			int l = (*pk1)[i];
			T key = call_get_value<T>(t1, l, var_name1);
			for (const auto &j : (*t2).lut4idx_tables[var_name2][any2str(key)]) {
				int r = str2any<int>(j);
				if ((*pk2_4hash).count(r))
					(*(*res)[l]).push_back(r);
			}
		}
	} else
		res = nested_join<T>(t, pk, var_name, op);

	return res;
}

vector<pair<Table *, vector<int> > > spj_join(map<Table *, vector<int> *> pk, vector<pair<string, void *> > exprs,
        string algo) { //���ص���pkֵ��������￿
	//algo = "nested", "merge", "hash"
	vector<pair<Table *, vector<int> > > res;
	vector<map<int, vector<int> *> *> each_join;

	for (map<Table *, vector<int> *>::iterator i = pk.begin(); i != pk.end(); i++) {
		vector<int> resinull;
		res.push_back(make_pair(i->first, resinull));
	}

	for (int i = 0; i < exprs.size(); i++) {
		string type = exprs[i].first;

		Table *t1 = get_spj_condition_obj_table(exprs[i], 0);
		Table *t2 = get_spj_condition_obj_table(exprs[i], 1);

		string var_name1 = get_spj_condition_obj_var_name(exprs[i], 0);
		string var_name2 = get_spj_condition_obj_var_name(exprs[i], 1);

		vector<int> *pk1 = pk[t1];
		vector<int> *pk2 = pk[t2];

		string op = get_spj_condition_op(exprs[i]);

		map<int, vector<int> *> *resi;

		//cout << pk1->size() << ' ' << pk2->size() << ' ' << var_name1 << ' ' << var_name2 << ' ' << op;
		if (algo == "nested") {
			if (strcmp(type.c_str(), "int") == 0)
				resi = hash_join<int>(make_pair(t1, t2), make_pair(pk1, pk2), make_pair(var_name1, var_name2), op);
			else if (strcmp(type.c_str(), "long") == 0)
				resi = hash_join<long long>(make_pair(t1, t2), make_pair(pk1, pk2), make_pair(var_name1, var_name2), op);
			else if (strcmp(type.c_str(), "float") == 0)
				resi = hash_join<float>(make_pair(t1, t2), make_pair(pk1, pk2), make_pair(var_name1, var_name2), op);
			else if (strcmp(type.c_str(), "double") == 0)
				resi = hash_join<double>(make_pair(t1, t2), make_pair(pk1, pk2), make_pair(var_name1, var_name2), op);
			else
				resi = hash_join<string>(make_pair(t1, t2), make_pair(pk1, pk2), make_pair(var_name1, var_name2), op);

		} else if (algo == "merge") {

		} else if (algo == "hash") {

		} else {
			resi = new map<int, vector<int> *>;
		}

		each_join.push_back(resi);
	}

	vector<int> alljoin_pk;
	spj_alljoin(0, each_join, res, alljoin_pk);

	//GC
	for (int i = 0; i < each_join.size(); i++) {
		for (map<int, vector<int> *>::iterator j = (*each_join[i]).begin(); j != (*each_join[i]).end(); ++j)
			delete j->second;
		delete each_join[i];
	}

	return res;
}

bool cmp_pair_g(pair<int, int> &a, pair<int, int> &b) {
	if (a.first == b.first)
		return a.second > b.second;
	return a.first > b.first;
}

bool cmp_pair_l(pair<int, int> &a, pair<int, int> &b) {
	if (a.first == b.first)
		return a.second < b.second;
	return a.first < b.first;
}

bool cmp_pair_eq(pair<int, int> &a, pair<int, int> &b) {
	return a.first == b.first && a.second == b.second;
}


vector<pair<int, int> > kmerge_pairs_intersection(vector<vector<pair<int, int> > > &pairs) {
	vector<pair<int, int> > res;
	vector<int> i;
	for (int j = 0; j < pairs.size(); j++)
		i.push_back(0);

	while (true) {

		//cout << "S: ";
		//for (int j = 0; j < i.size(); j++)
		//	cout << i[j] << ' ';
		//cout << endl;

		pair<int, int> maxi_pk = make_pair(-1, -1);
		for (int j = 0; j < i.size(); j++) {
			vector<pair<int, int> > *pairs_j = &pairs[j];
			if (i[j] >= (*pairs_j).size())
				return res;
			if (cmp_pair_g((*pairs_j)[i[j]], maxi_pk))
				maxi_pk = (*pairs_j)[i[j]];
		}

		bool same = true;
		for (int j = 0; j < i.size(); j++) {
			vector<pair<int, int> > *pairs_j = &pairs[j];
			if (cmp_pair_l((*pairs_j)[i[j]], maxi_pk)) {
				same = false;
				break;
			}
		}

		if (same) {
			if (!res.size() || cmp_pair_g(maxi_pk, res[res.size() - 1]))
				res.push_back(maxi_pk);
			for (int j = 0; j < i.size(); j++)
				i[j]++;
		} else
			for (int j = 0; j < i.size(); j++) {
				vector<pair<int, int> > *pairs_j = &pairs[j];
				if (cmp_pair_l((*pairs_j)[i[j]], maxi_pk))
					i[j]++;
			}
	}

	return res;
}

vector<pair<int, int> > kmerge_pairs_union(vector<vector<pair<int, int> > > &pairs) {
	vector<pair<int, int> > res;
	vector<int> i;
	for (int j = 0; j < pairs.size(); j++)
		i.push_back(0);

	while (true) {
		pair<int, int> mini_pk = make_pair(-1, -1);
		for (int j = 0; j < i.size(); j++) {
			vector<pair<int, int> > *pairs_j = &pairs[j];
			if (i[j] < (*pairs_j).size() && (mini_pk.first == -1 || cmp_pair_l((*pairs_j)[i[j]], mini_pk)))
				mini_pk = (*pairs_j)[i[j]];
		}

		if (mini_pk.first == -1)
			break;

		if (!res.size() || cmp_pair_g(mini_pk, res[res.size() - 1]))
			res.push_back(mini_pk);
		for (int j = 0; j < i.size(); j++) {
			vector<pair<int, int> > *pairs_j = &pairs[j];
			if (i[j] < (*pairs_j).size() && cmp_pair_eq((*pairs_j)[i[j]], mini_pk))
				i[j]++;
		}
	}

	return res;
}

void test_nestedjoin() {
	Table *t[10];
	int tn = 3;

	vector<string> var_names[10];
	vector<pair<string, void *> > exprs;

	for (int i = 1; i <= tn; i++) {
		t[i] = new Table(i);
		//t[i]->print_self();

		for (int j = 0; j < t[i]->attr.size(); j++)
			var_names[i].push_back("");

		for (map<string, string>::iterator j = t[i]->attr.begin(); j != t[i]->attr.end(); ++j)
			var_names[i][str2any<int>(j->second)] = j->first;
	}

	string conj = "";

	//vector<int> *pk = spj_select(t, exprs, conj);

	for (int i = 1; i <= tn; i++)
		cout << output_select_project(t[i], &var_names[i], exprs, conj) << endl << endl;

	vector<map<int, vector<int> *> *> each_join;

	pair<vector<int> *, vector<int> *> pk;

	spj_condition_obj<double> score_l;
	score_l.var_con = 0;
	score_l.table = t[1];
	score_l.var_name = "score";

	spj_condition_obj<double> score_r;
	score_r.var_con = 1;
	score_r.value = 60;

	spj_condition<double> score_expr;
	score_expr.a = score_r;
	score_expr.b = score_l;
	score_expr.op = "<";

	vector<pair<string, void *> > expr1;
	expr1.push_back(make_pair("double", &score_expr));

	/* //��ɸѡ������
		pk.first = spj_select(t[1], expr1, "");

		spj_condition_obj<int> grade_l;
		grade_l.var_con = 0;
		grade_l.table = t[2];
		grade_l.var_name = "grade";

		spj_condition_obj<int> grade_r;
		grade_r.var_con = 1;
		grade_r.value = 1;

		spj_condition<int> grade_expr;
		grade_expr.a = grade_l;
		grade_expr.b = grade_r;
		grade_expr.op = ">";

		vector<pair<string, void *> > expr2;
		expr2.push_back(make_pair("int", &grade_expr));

		pk.second = spj_select(t[2], expr2, "");

		pair<string, string> join_var_name;
		join_var_name.first = "name";
		join_var_name.second = "name";

		string join_op = "=";

		each_join.push_back(nested_join<string>(make_pair(t[1], t[2]), pk, join_var_name, join_op));
	*/

	/* ���ϻ�
	for (int i = 0; i < (*pk.first).size(); i++)
		cout << (*pk.first)[i] << ' ';
	cout << endl;
	for (int i = 0; i < (*pk.second).size(); i++)
		cout << (*pk.second)[i] << ' ';
	cout << endl;
	*/

	/*
	pk.first = new vector<int>;
	pk.second = new vector<int>;

	for (int i = 0; i < t[1]->num_items; i++)
		pk.first->push_back(i);
	for (int i = 0; i < t[2]->num_items; i++)
		pk.second->push_back(i);

	each_join.push_back(nested_join<string>(make_pair(t[1], t[2]), pk, join_var_name, join_op));
	*/

	//�ǵ�ֵ����
	pk.first = new vector<int>;
	pk.second = new vector<int>;
	for (int i = 0; i < t[1]->num_items; i++)
		pk.first->push_back(i);
	for (int i = 0; i < t[3]->num_items; i++)
		pk.second->push_back(i);

	pair<string, string> join_var_name;
	join_var_name.first = "score";
	join_var_name.second = "score_l";

	string join_op = ">=";

	/*  //ֱ�ӵ���nested_join
		each_join.push_back(nested_join<double>(make_pair(t[1], t[3]), pk, join_var_name, join_op));


		for (int i = 0; i < each_join.size(); i++)
			for (map<int, vector<int> *>::iterator j = (*each_join[i]).begin(); j != (*each_join[i]).end(); ++j)
				for (int k = 0; k < (*j->second).size(); k++)
					cout << j->first << ',' << (*j->second)[k] << endl;
	*/

	vector<pair<Table *, vector<int> > > join_res;
	vector<pair<Table *, vector<int> > > join_res0;

	map<Table *, vector<int> *> join_pk;

	vector<pair<string, void *> > join_exprs;

	join_pk[t[1]] = pk.first;
	join_pk[t[3]] = pk.second;

	spj_condition_obj<int> join_kashi_l;
	join_kashi_l.table = t[1];
	spj_condition_obj<int> join_kashi_r;
	join_kashi_r.table = t[3];
	spj_condition<int> join_kashi;
	join_kashi.a = join_kashi_l;
	join_kashi.b = join_kashi_r;
	join_kashi.op = "";

	join_exprs.push_back(make_pair("int", &join_kashi));

	join_res = spj_join(join_pk, join_exprs, "nested");

	for (int i = 0; i < join_res[0].second.size(); i++) {
		for (int j = 0; j < join_res.size(); j++)
			cout << join_res[j].second[i] << ',';
		cout << endl;
	}
	cout << endl;

	join_exprs.clear();

//���ڵ���score_l
	spj_condition_obj<double> join_score_l;
	join_score_l.var_con = 0;
	join_score_l.table = t[1];
	join_score_l.var_name = "score";

	spj_condition_obj<double> join_score_r;
	join_score_r.var_con = 0;
	join_score_r.table = t[3];
	join_score_r.var_name = "score_l";

	spj_condition<double> join_score_expr;
	join_score_expr.a = join_score_l;
	join_score_expr.b = join_score_r;
	join_score_expr.op = ">=";

	join_exprs.push_back(make_pair("double", &join_score_expr));

	join_res = spj_join(join_pk, join_exprs, "nested");

	for (int i = 0; i < join_res[0].second.size(); i++) {
		for (int j = 0; j < join_res.size(); j++)
			cout << join_res[j].second[i] << ',';
		cout << endl;
	}
	cout << endl;

	join_res0 = join_res;

//С�ڵ���score_h
	join_exprs.clear();

	join_score_r.var_name = "score_h";
	join_score_expr.b = join_score_r;
	join_score_expr.op = "<=";

	join_exprs.push_back(make_pair("double", &join_score_expr));

	join_res = spj_join(join_pk, join_exprs, "nested");

	for (int i = 0; i < join_res[0].second.size(); i++) {
		for (int j = 0; j < join_res.size(); j++)
			cout << join_res[j].second[i] << ',';
		cout << endl;
	}
	cout << endl;

	vector<vector<pair<int, int> > > join_pairs;

	vector<pair<int, int> > join_pair, join_pair0;
	for (int i = 0; i < join_res[0].second.size(); i++)
		join_pair.push_back(make_pair(join_res[0].second[i], join_res[1].second[i]));
	for (int i = 0; i < join_res0[0].second.size(); i++)
		join_pair0.push_back(make_pair(join_res0[0].second[i], join_res0[1].second[i]));

	join_pairs.push_back(join_pair0);
	join_pairs.push_back(join_pair);

	vector<pair<int, int> > join_pair_res = kmerge_pairs_intersection(join_pairs);

	for (int i = 0; i < join_pair_res.size(); i++)
		cout << join_pair_res[i].first << ',' << join_pair_res[i].second << endl;
	cout << endl;

	join_pair_res = kmerge_pairs_union(join_pairs);

	for (int i = 0; i < join_pair_res.size(); i++)
		cout << join_pair_res[i].first << ',' << join_pair_res[i].second << endl;
	cout << endl;
}

void test_nestedjoin_triple() {
	Table *t[10];
	int tn = 3;

	vector<string> var_names[10];
	vector<pair<string, void *> > exprs;

	string conj = "";

	for (int i = 1; i <= tn; i++) {
		t[i] = new Table(i + 3);
		//t[i]->print_self();

		for (int j = 0; j < t[i]->attr.size(); j++)
			var_names[i].push_back("");

		for (map<string, string>::iterator j = t[i]->attr.begin(); j != t[i]->attr.end(); ++j)
			var_names[i][str2any<int>(j->second)] = j->first;
	}

	for (int i = 1; i <= tn; i++)
		cout << output_select_project(t[i], &var_names[i], exprs, conj) << endl << endl;

//table 4 join 5
	pair<vector<int> *, vector<int> *> pk;

	pk.first = new vector<int>;
	pk.second = new vector<int>;
	for (int i = 0; i < t[1]->num_items; i++)
		pk.first->push_back(i);
	for (int i = 0; i < t[2]->num_items; i++)
		pk.second->push_back(i);

	vector<pair<Table *, vector<int> > > join_res;

	map<Table *, vector<int> *> join_pk;
	join_pk[t[1]] = pk.first;
	join_pk[t[2]] = pk.second;

	vector<pair<string, void *> > join_exprs;

	spj_condition_obj<int> join_mid_l;
	join_mid_l.var_con = 0;
	join_mid_l.table = t[1];
	join_mid_l.var_name = "mid";

	spj_condition_obj<int> join_mid_r;
	join_mid_r.var_con = 0;
	join_mid_r.table = t[2];
	join_mid_r.var_name = "mid";

	spj_condition<int> join_mid;
	join_mid.a = join_mid_l;
	join_mid.b = join_mid_r;
	join_mid.op = "=";

	join_exprs.push_back((make_pair("int", &join_mid)));

	join_res = spj_join(join_pk, join_exprs, "nested");
	for (int i = 0; i < join_res[0].second.size(); i++) {
		for (int j = 0; j < join_res.size(); j++)
			cout << join_res[j].second[i] << ',';
		cout << endl;
	}
	cout << endl;

//if only one expr
	bool only_one_expr = false;

	if (only_one_expr) {
		join_exprs.clear();
		join_pk.clear();
	}

//one expr: table 5 join 6; two expr: table 4 join 5 join 6
	pair<vector<int> *, vector<int> *> pk2;

	pk2.first = new vector<int>;
	pk2.second = new vector<int>;
	for (int i = 0; i < t[2]->num_items; i++)
		pk2.first->push_back(i);
	for (int i = 0; i < t[3]->num_items; i++)
		pk2.second->push_back(i);

	join_pk[t[2]] = pk2.first;
	join_pk[t[3]] = pk2.second;

	spj_condition_obj<int> join_did_l;
	join_did_l.var_con = 0;
	join_did_l.table = t[2];
	join_did_l.var_name = "did";

	spj_condition_obj<int> join_did_r;
	join_did_r.var_con = 0;
	join_did_r.table = t[3];
	join_did_r.var_name = "did";

	spj_condition<int> join_did;
	join_did.a = join_did_l;
	join_did.b = join_did_r;
	join_did.op = "=";

	join_exprs.push_back((make_pair("int", &join_did)));

	join_res = spj_join(join_pk, join_exprs, "nested");
	for (int i = 0; i < join_res[0].second.size(); i++) {
		for (int j = 0; j < join_res.size(); j++)
			cout << join_res[j].second[i] << ',';
		cout << endl;
	}
	cout << endl;
}

pair<string, void *> spj_project_ij(Table *t, int pk, string var_name) {
	//string type = get<0>(t->attr_dict[var_name]);
	string type = t->type[var_name];

	int *value_i = new int;
	long long *value_ll = new long long;
	float *value_f = new float;
	double *value_d = new double;
	string *value_s = new string;
	if (strcmp(type.c_str(), "int") == 0)
		*value_i = call_get_value<int>(t, pk, var_name);
	else if (strcmp(type.c_str(), "long") == 0)
		*value_ll = call_get_value<long long>(t, pk, var_name);
	else if (strcmp(type.c_str(), "float") == 0)
		*value_f = call_get_value<float>(t, pk, var_name);
	else if (strcmp(type.c_str(), "double") == 0)
		*value_d = call_get_value<double>(t, pk, var_name);
	else *value_s = call_get_value<string>(t, pk, var_name);

	void *value = NULL;
	if (strcmp(type.c_str(), "int") == 0)
		value = value_i;
	else if (strcmp(type.c_str(), "long") == 0)
		value = value_ll;
	else if (strcmp(type.c_str(), "float") == 0)
		value = value_f;
	else if (strcmp(type.c_str(), "double") == 0)
		value = value_d;
	else
		value = value_s;

	return make_pair(type, value);
}

vector<vector<pair<string, void *> > > spj(vector<Table *> &tables, vector<pair<Table *, string> > &table_var_names,
        vector<pair<string, void *> > &exprs, string conj, string join_algo) {
	//join_algo = "nested", "merge", "hash"
	map<Table *, vector<pair<string, void *> > > common_exprs;
	vector<pair<Table *, vector<pair<string, void *> > > > joins_exprs;
	vector<pair<string, void *> > con_exprs;

	for (int i = 0; i < tables.size(); i++) { //table����
		vector<pair<string, void *> > vnull;
		joins_exprs.push_back(make_pair(tables[i], vnull));
		common_exprs[tables[i]] = vnull;
	}

	for (int i = 0; i < exprs.size(); i++) { //exprs����
		int var_con1 = get_spj_condition_obj_var_con(exprs[i], 0);
		int var_con2 = get_spj_condition_obj_var_con(exprs[i], 1);

		if (!var_con1 && !var_con2) {
			Table *t = get_spj_condition_obj_table(exprs[i], 0);
			for (int j = 0; j < joins_exprs.size(); j++)
				if (joins_exprs[j].first == t) {
					joins_exprs[j].second.push_back(exprs[i]);
					break;
				}
		} else if (!var_con1) {
			Table *t = get_spj_condition_obj_table(exprs[i], 0);
			common_exprs[t].push_back(exprs[i]);
		} else if (!var_con2) {
			Table *t = get_spj_condition_obj_table(exprs[i], 1);
			common_exprs[t].push_back(exprs[i]);
		} else
			con_exprs.push_back(exprs[i]);
	}

	map<Table *, vector<int> *> pks; //��ÿ���Լ�select

	for (int i = 0; i < tables.size(); i++) {
		vector<pair<string, void *> > exprs;

		Table *t = tables[i];

		for (int j = 0; j < con_exprs.size(); j++)
			exprs.push_back(con_exprs[j]);

		vector<pair<string, void *> > *common_exprs_t = &common_exprs[t];
		for (int j = 0; j < (*common_exprs_t).size(); j++)
			exprs.push_back((*common_exprs_t)[j]);

		pks[t] = spj_select(t, exprs, conj);
	}

	vector<vector<int> > final_pks; //join

	//cout << "Number of tables: " << final_pks.size() << endl;

	if (tables.size() > 2) {
		for (int i = 0; i < joins_exprs.size() - 1; i++) {
			Table *t1 = joins_exprs[i].first;
			Table *t2 = joins_exprs[i + 1].first;

			if (joins_exprs[i].second.empty()) { //kashi
				spj_condition_obj<int> join_kashi_l;
				join_kashi_l.table = t1;
				spj_condition_obj<int> join_kashi_r;
				join_kashi_r.table = t2;

				spj_condition<int> join_kashi;
				join_kashi.a = join_kashi_l;
				join_kashi.b = join_kashi_r;
				join_kashi.op = "";

				joins_exprs[i].second.push_back(make_pair("int", &join_kashi));
			}
		}

		vector<pair<string, void *> > join_exprs;
		for (int i = 0; i < joins_exprs.size() - 1; i++)
			for (int j = 0; j < joins_exprs[i].second.size(); j++)
				join_exprs.push_back(joins_exprs[i].second[j]);

		vector<pair<Table *, vector<int> > > res = spj_join(pks, join_exprs, join_algo);

		for (int i = 0; i < res.size(); i++) {
			vector<int> resi = res[i].second;
			final_pks.push_back(resi);
		}
	} else if (tables.size() == 2) {
		for (int i = 0; i < joins_exprs.size() - 1; i++) {
			Table *t1 = joins_exprs[i].first;
			Table *t2 = joins_exprs[i + 1].first;

			if (joins_exprs[i].second.empty()) { //kashi
				spj_condition_obj<int> join_kashi_l;
				join_kashi_l.table = t1;
				spj_condition_obj<int> join_kashi_r;
				join_kashi_r.table = t2;

				spj_condition<int> join_kashi;
				join_kashi.a = join_kashi_l;
				join_kashi.b = join_kashi_r;
				join_kashi.op = "";

				joins_exprs[i].second.push_back(make_pair("int", &join_kashi));
			}
		}

		vector<vector<pair<int, int> > > join_pairs;

		vector<pair<string, void *> > join_exprs = joins_exprs[0].second;
		//cout << join_exprs.size() << endl;
		for (int i = 0; i < join_exprs.size(); i++) {
			vector<pair<string, void *> > join_expr;
			join_expr.push_back(join_exprs[i]);
			vector<pair<Table *, vector<int> > > res = spj_join(pks, join_expr, join_algo);

			vector<pair<int, int> > join_pair;
			for (int j = 0; j < res[0].second.size(); j++)
				join_pair.push_back(make_pair(res[0].second[j], res[1].second[j]));

			join_pairs.push_back(join_pair);
		}

		vector<pair<int, int> > kmerge_res;
		if (conj == "AND")
			kmerge_res = kmerge_pairs_intersection(join_pairs);
		else if (conj == "OR")
			kmerge_res = kmerge_pairs_union(join_pairs);
		else
			kmerge_res = join_pairs[0];

		vector<int> kmerge_res0, kmerge_res1;
		for (int i = 0; i < kmerge_res.size(); i++) {
			kmerge_res0.push_back(kmerge_res[i].first);
			kmerge_res1.push_back(kmerge_res[i].second);
		}

		final_pks.push_back(kmerge_res0);
		final_pks.push_back(kmerge_res1);
	} else if (tables.size() == 1) {
		map<Table *, vector<int> *>::iterator i = pks.begin();
		final_pks.push_back(*(i->second));
	}

	cout << "final_pks size=" << final_pks.size() << endl;;
	for (int i = 0; i < final_pks.size(); i++) {
		for (int j = 0; j < final_pks[i].size(); j++)
			cout << final_pks[i][j] << ' ';
		cout << endl;
	}

	vector<vector<pair<string, void *> > > final_res; //project

	for (int i = 0; i < final_pks[0].size(); i++) {
		vector<pair<string, void *> > one_line;
		for (int j = 0; j < table_var_names.size(); j++) {
			pair<Table *, string> var_name = table_var_names[j];

			string *pstr = new string;
			*pstr = "#N/A#";
			one_line.push_back(make_pair("string", (void *)pstr));

			for (int k = 0; k < tables.size(); k++)
				if (tables[k] == var_name.first) {
					one_line[one_line.size() - 1] = spj_project_ij(var_name.first, final_pks[k][i], var_name.second);
					break;
				}
		}

		final_res.push_back(one_line);
	}

	return final_res;
}

vector<Table *> table_list;
map<string, int> search_table;

void spj_init() {
	for (int i = 1; i <= 9; i++) {
		Table *t = new Table(i);
		table_list.push_back(t);
		search_table[t->table_name] = table_list.size() - 1;
	}
}

bool cmp_4var_names(const pair<string, pair<int, int> > &a, const pair<string, pair<int, int> > &b) {
	if (a.second.first == b.second.first)
		return a.second.second < b.second.second;
	return a.second.first < b.second.first;
}

/*int get_V_column0(const vector<vector<pair<string, void *> > > &arrs) {
	set<string> V;
	for (int i = 0; i < arrs.size(); i++) {
		vector<pair<string, void *> > arr = arrs[i];
		string type = arr[0].first;
		void *data = arr[0].second;

		if (strcmp(type.c_str(), "int") == 0)
			V.insert(any2str(*(int *)data));
		else if (strcmp(type.c_str(), "long") == 0)
			V.insert(any2str(*(long long *)data);
			         else if (strcmp(type.c_str(), "float") == 0)
			         V.insert(any2str(*(float *)data);
			                  else if (strcmp(type.c_str(), "double") == 0)
				                  V.insert(any2str(*(double *)data);
				                           else
					                           V.insert(any2str(*(string *)data);
				}

                           return V.size();
}*/

void test_result_print(ostringstream &oss, vector<Table *> &tables, vector<pair<Table *, string> > &table_var_names,
                       vector<pair<string, void *> > &exprs, string conj, string join_algo, vector<vector<pair<string, void *> > > arrs) {
	for (int j = 0; j < table_var_names.size(); j++)
		oss << "| " << table_var_names[j].first->table_name << '.' << table_var_names[j].second << ' ';
	oss << "|\n";

	for (int i = 0; i < arrs.size(); i++) {
		vector<pair<string, void *> > arr = arrs[i];
		for (int j = 0; j < arr.size(); j++) {
			string type = arr[j].first;
			void *data = arr[j].second;
			if (strcmp(type.c_str(), "int") == 0)
				oss << "| " << *(int *)data;
			else if (strcmp(type.c_str(), "long") == 0)
				oss << "| " << *(long long *)data;
			else if (strcmp(type.c_str(), "float") == 0)
				oss << "| " << *(float *)data;
			else if (strcmp(type.c_str(), "double") == 0)
				oss << "| " << *(double *)data;
			else
				oss << "| " << *(string *)data;

			oss << ' ';
		}
		oss << "|\n";
	}
}

Table *create_table(const string &t_name, const vector<pair<string, string> > &columns) {
	Table *t = new Table(t_name, columns);
	return t;
}

void load_data(Table *t, const string &path) {
	t->load_data(path);
}

void insert(Table *t, const vector<pair<string, string> > &data_i) {
	vector<string> data_str_i;

	for (int j = 0; j < data_i.size(); j++)
		data_str_i.push_back(data_i[j].second);
	t->insert(data_str_i);
}

void t_delete(Table *t, vector<int> *del_pk) {
	t->del(*del_pk);
}

void update(Table *t, vector<int> *upd_pk, const string &column, const string &value) {
	t->update_j(*upd_pk, column, value);
}

void create_index(Table *t, const string &column) {
	t->create_index(column);
}

void drop(Table *t) {
	map<string, int>::iterator p = search_table.find(t->table_name);
	search_table.erase(p);

	int i = p->second;
	table_list.erase(table_list.begin() + i);

	for (i = 0; i < table_list.size(); i++)
		search_table[table_list[i]->table_name] = i;

	t->drop();
	delete t;
}

void transection_begin() {
	for (int i = 0; i < table_list.size(); i++) {
		Table *t = table_list[i];
		string filename = "transection_backup\\" + t->table_name + ".tbl";
		t->transection_backup_write(filename);
	}
}

void transection_commit() {
	for (int i = 0; i < table_list.size(); i++) {
		Table *t = table_list[i];
		string filename = "transection_backup\\" + t->table_name + ".tbl";
		t->transection_backup_write(filename);
	}
}

void transection_rollback() {
	for (int i = 0; i < table_list.size(); i++) {
		Table *t = table_list[i];
		string filename = "transection_backup\\" + t->table_name + ".tbl";
		t->drop();
		t->load_data(filename);
	}
}


vector<vector<pair<string, void *> > > execute(const string &user_order) {
	vector<vector<pair<string, void *> > > arrs;

	istringstream cin(user_order);

	vector<Table *> tables;
	vector<pair<Table *, string> > table_var_names;
	vector<pair<string, void *> > exprs;
	string conj, join_algo = "nested";

	int n0, n1, n2;

	cin >> n0;

	if (n0 == -1) { //CREATE TABLE
		string t_name;
		cin >> t_name >> n1;

		vector<pair<string, string> > columns;
		while (n1--) {
			string var_name, var_type;
			cin >> var_name >> var_type;
			columns.push_back(make_pair(var_name, var_type));
		}

		Table *t = create_table(t_name, columns);
		table_list.push_back(t);
		search_table[t_name] = table_list.size() - 1;

		return arrs;
	}

	if (n0 == -2) { //LOAD FILE
		string path, t_name;

		getline(cin, path); //����\n
		getline(cin, path);
		getline(cin, t_name);

		Table *t = table_list[search_table[t_name]];
		load_data(t, path);

		return arrs;
	}

	if (n0 == -3) { //INSERT
		string t_name, type_ij, value_ij;
		vector<pair<string, string> > data_i;

		cin >> t_name >> n1;
		getline(cin, type_ij); //����\n

		while (n1--) {
			getline(cin, type_ij);
			getline(cin, value_ij);
			data_i.push_back(make_pair(type_ij, value_ij));
		}

		Table *t = table_list[search_table[t_name]];
		insert(t, data_i);

		return arrs;
	}

	if (n0 == -4) { //DELETE
		string t_name;
		cin >> t_name >> n2;

		Table *t = table_list[search_table[t_name]];

		while (n2--) {
			string type;

			cin >> type;
			//cout << type << endl;
			if (strcmp(type.c_str(), "int") == 0) {
				spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
				spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
				spj_condition<int> *sc = new spj_condition<int>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
				//cout << sco1 << endl;

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				//cout << sc -> op << endl;

				exprs.push_back(make_pair("int", (void *)sc));

				//cout << exprs[exprs.size() - 1].second << endl;

			} else if (strcmp(type.c_str(), "long") == 0) {
				spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
				spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
				spj_condition<long long> *sc = new spj_condition<long long>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("long long", (void *)sc));
			} else if (strcmp(type.c_str(), "float") == 0) {
				spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
				spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
				spj_condition<float> *sc = new spj_condition<float>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("float", (void *)sc));
			} else if (strcmp(type.c_str(), "double") == 0) {
				spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
				spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
				spj_condition<double> *sc = new spj_condition<double>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("double", (void *)sc));
			} else {
				spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
				spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
				spj_condition<string> *sc = new spj_condition<string>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("string", (void *)sc));
			}
		}
		cin >> conj;
		if (conj == "NO")
			conj = "";

		vector<int> *del_pk = spj_select(t, exprs, conj);
		t_delete(t, del_pk);

		return arrs;
	}

	if (n0 == -5) { //UPDATE
		string t_name, var_name, type_j, value_j;
		cin >> t_name;
		cin >> var_name >> type_j;
		getline(cin, value_j); //����\n
		getline(cin, value_j);
		cin >> n2;

		Table *t = table_list[search_table[t_name]];

		while (n2--) {
			string type;

			cin >> type;
			//cout << type << endl;
			if (strcmp(type.c_str(), "int") == 0) {
				spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
				spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
				spj_condition<int> *sc = new spj_condition<int>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
				//cout << sco1 << endl;

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				//cout << sc -> op << endl;

				exprs.push_back(make_pair("int", (void *)sc));

				//cout << exprs[exprs.size() - 1].second << endl;

			} else if (strcmp(type.c_str(), "long") == 0) {
				spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
				spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
				spj_condition<long long> *sc = new spj_condition<long long>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("long long", (void *)sc));
			} else if (strcmp(type.c_str(), "float") == 0) {
				spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
				spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
				spj_condition<float> *sc = new spj_condition<float>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("float", (void *)sc));
			} else if (strcmp(type.c_str(), "double") == 0) {
				spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
				spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
				spj_condition<double> *sc = new spj_condition<double>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("double", (void *)sc));
			} else {
				spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
				spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
				spj_condition<string> *sc = new spj_condition<string>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("string", (void *)sc));
			}
		}
		cin >> conj;
		if (conj == "NO")
			conj = "";

		vector<int> *upd_pk = spj_select(t, exprs, conj);
		update(t, upd_pk, var_name, value_j);

		return arrs;
	}

	if (n0 == -6) { //CREATE INDEX
		string t_name, var_name;
		cin >> t_name >> var_name;

		Table *t = table_list[search_table[t_name]];
		create_index(t, var_name);

		return arrs;
	}

	if (n0 == -7) { //DROP
		string t_name;
		cin >> t_name;

		Table *t = table_list[search_table[t_name]];
		drop(t);

		return arrs;
	}

	if (n0 == -8) { //BEGIN
		transection_begin();

		return arrs;
	}

	if (n0 == -9) { //COMMIT
		transection_commit();

		return arrs;
	}

	if (n0 == -10) { //ROLLBACK
		transection_rollback();

		return arrs;
	}

	while (n0--) { //spj
		string t_name;
		cin >> t_name;
		tables.push_back(table_list[search_table[t_name]]);
	}

	cin >> n1;
	while (n1--) {
		string t_name, var_name;
		cin >> t_name;
		if (t_name != "*") {
			cin >> var_name;
			Table *t = table_list[search_table[t_name]];
			table_var_names.push_back(make_pair(t, var_name));
		} else {
			vector<pair<string, pair<int, int> > > sort_4var_names;
			for (int i = 0; i < tables.size(); i++) {
				Table *t = tables[i];
				for (map<string, string>::iterator j = t->attr.begin(); j != t->attr.end(); ++j)
					if (str2any<int>(j->second))
						sort_4var_names.push_back(make_pair(j->first, make_pair(i, str2any<int>(j->second))));
			}
			sort(sort_4var_names.begin(), sort_4var_names.end(), cmp_4var_names);

			for (int i = 0; i < sort_4var_names.size(); i++)
				table_var_names.push_back(make_pair(tables[sort_4var_names[i].second.first], sort_4var_names[i].first));
			break;
		}
	}

	cin >> n2;
	while (n2--) {
		string type;

		cin >> type;
		//cout << type << endl;
		if (strcmp(type.c_str(), "int") == 0) {
			spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
			spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
			spj_condition<int> *sc = new spj_condition<int>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
			//cout << sco1 << endl;

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			//cout << sc -> op << endl;

			exprs.push_back(make_pair("int", (void *)sc));

			//cout << exprs[exprs.size() - 1].second << endl;

		} else if (strcmp(type.c_str(), "long") == 0) {
			spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
			spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
			spj_condition<long long> *sc = new spj_condition<long long>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("long long", (void *)sc));
		} else if (strcmp(type.c_str(), "float") == 0) {
			spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
			spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
			spj_condition<float> *sc = new spj_condition<float>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("float", (void *)sc));
		} else if (strcmp(type.c_str(), "double") == 0) {
			spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
			spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
			spj_condition<double> *sc = new spj_condition<double>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("double", (void *)sc));
		} else {
			spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
			spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
			spj_condition<string> *sc = new spj_condition<string>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("string", (void *)sc));
		}
	}
	cin >> conj;
	if (conj == "NO")
		conj = "";

	arrs = spj(tables, table_var_names, exprs, conj, join_algo);

	ostringstream oss;
	test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
	cout << oss.str() << endl;

	return arrs;
}

std::string return_string_execute(const string &user_order) {
	vector<vector<pair<string, void *> > > arrs;

	istringstream cin(user_order);

	vector<Table *> tables;
	vector<pair<Table *, string> > table_var_names;
	vector<pair<string, void *> > exprs;
	string conj, join_algo = "nested";

	int n0, n1, n2;

	cin >> n0;

	if (n0 == -1) { //CREATE TABLE
		string t_name;
		cin >> t_name >> n1;

		vector<pair<string, string> > columns;
		while (n1--) {
			string var_name, var_type;
			cin >> var_name >> var_type;
			columns.push_back(make_pair(var_name, var_type));
		}

		Table *t = create_table(t_name, columns);
		table_list.push_back(t);
		search_table[t_name] = table_list.size() - 1;
        ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -2) { //LOAD FILE
		string path, t_name;

		getline(cin, path); //����\n
		getline(cin, path);
		getline(cin, t_name);

		Table *t = table_list[search_table[t_name]];
		load_data(t, path);

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -3) { //INSERT
		string t_name, type_ij, value_ij;
		vector<pair<string, string> > data_i;

		cin >> t_name >> n1;
		getline(cin, type_ij); //����\n

		while (n1--) {
			getline(cin, type_ij);
			getline(cin, value_ij);
			data_i.push_back(make_pair(type_ij, value_ij));
		}

		Table *t = table_list[search_table[t_name]];
		insert(t, data_i);

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -4) { //DELETE
		string t_name;
		cin >> t_name >> n2;

		Table *t = table_list[search_table[t_name]];

		while (n2--) {
			string type;

			cin >> type;
			//cout << type << endl;
			if (strcmp(type.c_str(), "int") == 0) {
				spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
				spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
				spj_condition<int> *sc = new spj_condition<int>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
				//cout << sco1 << endl;

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				//cout << sc -> op << endl;

				exprs.push_back(make_pair("int", (void *)sc));

				//cout << exprs[exprs.size() - 1].second << endl;

			} else if (strcmp(type.c_str(), "long") == 0) {
				spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
				spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
				spj_condition<long long> *sc = new spj_condition<long long>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("long long", (void *)sc));
			} else if (strcmp(type.c_str(), "float") == 0) {
				spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
				spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
				spj_condition<float> *sc = new spj_condition<float>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("float", (void *)sc));
			} else if (strcmp(type.c_str(), "double") == 0) {
				spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
				spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
				spj_condition<double> *sc = new spj_condition<double>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("double", (void *)sc));
			} else {
				spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
				spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
				spj_condition<string> *sc = new spj_condition<string>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("string", (void *)sc));
			}
		}
		cin >> conj;
		if (conj == "NO")
			conj = "";

		vector<int> *del_pk = spj_select(t, exprs, conj);
		t_delete(t, del_pk);

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -5) { //UPDATE
		string t_name, var_name, type_j, value_j;
		cin >> t_name;
		cin >> var_name >> type_j;
		getline(cin, value_j); //����\n
		getline(cin, value_j);
		cin >> n2;

		Table *t = table_list[search_table[t_name]];

		while (n2--) {
			string type;

			cin >> type;
			//cout << type << endl;
			if (strcmp(type.c_str(), "int") == 0) {
				spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
				spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
				spj_condition<int> *sc = new spj_condition<int>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
				//cout << sco1 << endl;

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				//cout << sc -> op << endl;

				exprs.push_back(make_pair("int", (void *)sc));

				//cout << exprs[exprs.size() - 1].second << endl;

			} else if (strcmp(type.c_str(), "long") == 0) {
				spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
				spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
				spj_condition<long long> *sc = new spj_condition<long long>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("long long", (void *)sc));
			} else if (strcmp(type.c_str(), "float") == 0) {
				spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
				spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
				spj_condition<float> *sc = new spj_condition<float>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("float", (void *)sc));
			} else if (strcmp(type.c_str(), "double") == 0) {
				spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
				spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
				spj_condition<double> *sc = new spj_condition<double>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("double", (void *)sc));
			} else {
				spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
				spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
				spj_condition<string> *sc = new spj_condition<string>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("string", (void *)sc));
			}
		}
		cin >> conj;
		if (conj == "NO")
			conj = "";

		vector<int> *upd_pk = spj_select(t, exprs, conj);
		update(t, upd_pk, var_name, value_j);

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -6) { //CREATE INDEX
		string t_name, var_name;
		cin >> t_name >> var_name;

		Table *t = table_list[search_table[t_name]];
		create_index(t, var_name);

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -7) { //DROP
		string t_name;
		cin >> t_name;

		Table *t = table_list[search_table[t_name]];
		drop(t);

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -8) { //BEGIN
		transection_begin();

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -9) { //COMMIT
		transection_commit();

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	if (n0 == -10) { //ROLLBACK
		transection_rollback();

		ostringstream oss;
	    test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
		return oss.str();
	}

	while (n0--) { //spj
		string t_name;
		cin >> t_name;
		tables.push_back(table_list[search_table[t_name]]);
	}

	cin >> n1;
	while (n1--) {
		string t_name, var_name;
		cin >> t_name;
		if (t_name != "*") {
			cin >> var_name;
			Table *t = table_list[search_table[t_name]];
			table_var_names.push_back(make_pair(t, var_name));
		} else {
			vector<pair<string, pair<int, int> > > sort_4var_names;
			for (int i = 0; i < tables.size(); i++) {
				Table *t = tables[i];
				for (map<string, string>::iterator j = t->attr.begin(); j != t->attr.end(); ++j)
					if (str2any<int>(j->second))
						sort_4var_names.push_back(make_pair(j->first, make_pair(i, str2any<int>(j->second))));
			}
			sort(sort_4var_names.begin(), sort_4var_names.end(), cmp_4var_names);

			for (int i = 0; i < sort_4var_names.size(); i++)
				table_var_names.push_back(make_pair(tables[sort_4var_names[i].second.first], sort_4var_names[i].first));
			break;
		}
	}

	cin >> n2;
	while (n2--) {
		string type;

		cin >> type;
		//cout << type << endl;
		if (strcmp(type.c_str(), "int") == 0) {
			spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
			spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
			spj_condition<int> *sc = new spj_condition<int>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
			//cout << sco1 << endl;

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			//cout << sc -> op << endl;

			exprs.push_back(make_pair("int", (void *)sc));

			//cout << exprs[exprs.size() - 1].second << endl;

		} else if (strcmp(type.c_str(), "long") == 0) {
			spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
			spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
			spj_condition<long long> *sc = new spj_condition<long long>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("long long", (void *)sc));
		} else if (strcmp(type.c_str(), "float") == 0) {
			spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
			spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
			spj_condition<float> *sc = new spj_condition<float>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("float", (void *)sc));
		} else if (strcmp(type.c_str(), "double") == 0) {
			spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
			spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
			spj_condition<double> *sc = new spj_condition<double>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("double", (void *)sc));
		} else {
			spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
			spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
			spj_condition<string> *sc = new spj_condition<string>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("string", (void *)sc));
		}
	}
	cin >> conj;
	if (conj == "NO")
		conj = "";

	arrs = spj(tables, table_var_names, exprs, conj, join_algo);

	ostringstream oss;
	test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
	cout << oss.str() << endl;

	return oss.str();
}

void test_print_all_tables() {
	for (int i = 0; i < table_list.size(); i++) {
		Table *t = table_list[i];

		cout << t->table_name << " :" << endl;
		t->print_self();
		cout << endl;
	}
	if (table_list.empty())
		cout << "NO table" << endl << endl;
}

/*tuple<int, set<string>, int> execute4opt(const string &user_order) {
	vector<vector<pair<string, void *> > > arrs;

	istringstream cin(user_order);
	cout << "user_order:" << endl;
	cout << user_order << endl << endl;

	vector<Table *> tables;
	vector<pair<Table *, string> > table_var_names;
	vector<pair<string, void *> > exprs;
	string conj, join_algo = "nested";

	int n0, n1, n2;

	cin >> n0;
	while (n0--) { //spj
		string t_name;
		cin >> t_name;
		tables.push_back(table_list[search_table[t_name]]);
	}

	cin >> n1;
	while (n1--) {
		string t_name, var_name;
		cin >> t_name;
		if (t_name != "*") {
			cin >> var_name;
			Table *t = table_list[search_table[t_name]];
			table_var_names.push_back(make_pair(t, var_name));
		} else {
			vector<pair<string, pair<int, int> > > sort_4var_names;
			for (int i = 0; i < tables.size(); i++) {
				Table *t = tables[i];
				for (map<string, string>::iterator j = t->attr.begin(); j != t->attr.end(); ++j)
					if (str2any<int>(j->second))
						sort_4var_names.push_back(make_pair(j->first, make_pair(i, str2any<int>(j->second))));
			}
			sort(sort_4var_names.begin(), sort_4var_names.end(), cmp_4var_names);

			for (int i = 0; i < sort_4var_names.size(); i++)
				table_var_names.push_back(make_pair(tables[sort_4var_names[i].second.first], sort_4var_names[i].first));
			break;
		}
	}

	cin >> n2;
	while (n2--) {
		string type;

		cin >> type;
		//cout << type << endl;
		if (strcmp(type.c_str(), "int") == 0) {
			spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
			spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
			spj_condition<int> *sc = new spj_condition<int>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
			//cout << sco1 << endl;

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			//cout << sc -> op << endl;

			exprs.push_back(make_pair("int", (void *)sc));

			//cout << exprs[exprs.size() - 1].second << endl;

		} else if (strcmp(type.c_str(), "long") == 0) {
			spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
			spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
			spj_condition<long long> *sc = new spj_condition<long long>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("long long", (void *)sc));
		} else if (strcmp(type.c_str(), "float") == 0) {
			spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
			spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
			spj_condition<float> *sc = new spj_condition<float>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("float", (void *)sc));
		} else if (strcmp(type.c_str(), "double") == 0) {
			spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
			spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
			spj_condition<double> *sc = new spj_condition<double>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("double", (void *)sc));
		} else {
			spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
			spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
			spj_condition<string> *sc = new spj_condition<string>;
			string t_name;

			cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
			sco1->table = table_list[search_table[t_name]];

			cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
			sco2->table = table_list[search_table[t_name]];

			sc->a = *sco1;
			sc->b = *sco2;
			cin >> sc->op;

			exprs.push_back(make_pair("string", (void *)sc));
		}
	}
	cin >> conj;
	if (conj == "NO")
		conj = "";
	//cout << "execute4opt:" << endl;
	//cout << tables[0]->table_name << ' ' << table_var_names[0].first->table_name << ',' << table_var_names[0].second << " conj=" << conj << endl;
	arrs = spj(tables, table_var_names, exprs, conj, join_algo);

	ostringstream oss;
	test_result_print(oss, tables, table_var_names, exprs, conj, join_algo, arrs);
	cout << oss.str() << endl;

	cout << "ret1: ";
	cout << arrs.size() << endl;
	cout << "ret2: ";
	for (set<string>::iterator i = tables[0]->column_index_name.begin(); i != tables[0]->column_index_name.end(); ++i)
		cout << *i << ' ';
cout << "ret3: ":
	     V_column0 = get_V_column0(arrs);
	cout << V_column0(arrs) << endl;
	cout << endl;

	//tuple<int, set<string>, int> =
	return make_tuple(arrs.size(), tables[0]->column_index_name, get_V_column0);
	//return make_pair(arrs.size(), tables[0]->column_index_name);
}*/

void test_execute() {
	//spj_init();
	//cout << "init OK: " << table_list.size() << "tables" << endl << endl;

	test_print_all_tables();
	system("pause");
	while (1) {
		string str = "", user_order = "";
		ifstream fin("soh.txt");
		getline(fin, str);
		while (str != "") {
			user_order = user_order + str + '\n';
			getline(fin, str);
		}
		fin.close();
		cout << user_order << endl;
		//execute(user_order);

		test_print_all_tables();
		system("pause");
	}

	//fin.close();
}

void output_spj(ostringstream &oss, vector<Table *> &tables, vector<pair<Table *, string> > &table_var_names,
                vector<pair<string, void *> > &exprs, string conj, string join_algo) {
	for (int j = 0; j < table_var_names.size(); j++)
		oss << "| " << table_var_names[j].first->table_name << '.' << table_var_names[j].second << ' ';
	oss << "|\n";

	vector<vector<pair<string, void *> > > arrs = spj(tables, table_var_names, exprs, conj, join_algo);

	for (int i = 0; i < arrs.size(); i++) {
		vector<pair<string, void *> > arr = arrs[i];
		for (int j = 0; j < arr.size(); j++) {
			string type = arr[j].first;
			void *data = arr[j].second;
			if (strcmp(type.c_str(), "int") == 0)
				oss << "| " << *(int *)data;
			else if (strcmp(type.c_str(), "long") == 0)
				oss << "| " << *(long long *)data;
			else if (strcmp(type.c_str(), "float") == 0)
				oss << "| " << *(float *)data;
			else if (strcmp(type.c_str(), "double") == 0)
				oss << "| " << *(double *)data;
			else
				oss << "| " << *(string *)data;

			oss << ' ';
		}
		oss << "|\n";
	}

	//GC
	for (int i = 0; i < arrs.size(); i++) {
		vector<pair<string, void *> > arr = arrs[i];
		for (int j = 0; j < arr.size(); j++) {
			string type = arr[j].first;
			void *data = arr[j].second;

			int *value_i = NULL;
			long long *value_ll = NULL;
			float *value_f = NULL;
			double *value_d = NULL;
			string *value_s = NULL;
			if (strcmp(type.c_str(), "int") == 0)
				value_i = (int *)data;
			else if (strcmp(type.c_str(), "long") == 0)
				value_ll = (long long *)data;
			else if (strcmp(type.c_str(), "float") == 0)
				value_f = (float *)data;
			else if (strcmp(type.c_str(), "double") == 0)
				value_d = (double *)data;
			else
				value_s = (string *)data;

			if (strcmp(type.c_str(), "int") == 0)
				delete value_i;
			else if (strcmp(type.c_str(), "long") == 0)
				delete value_ll;
			else if (strcmp(type.c_str(), "float") == 0)
				delete value_f;
			else if (strcmp(type.c_str(), "double") == 0)
				delete value_d;
			else
				delete value_s;
		}
	}
}

void spj_user() {
	spj_init();
	cout << "init OK: " << table_list.size() << "tables" << endl << endl;

	while (1) {
		vector<Table *> tables;
		vector<pair<Table *, string> > table_var_names;
		vector<pair<string, void *> > exprs;
		string conj, join_algo = "nested";

		int n0, n1, n2;

		cin >> n0;
		while (n0--) {
			string t_name;
			cin >> t_name;
			tables.push_back(table_list[search_table[t_name]]);
		}

		cin >> n1;
		while (n1--) {
			string t_name, var_name;
			cin >> t_name;
			if (t_name != "*") {
				cin >> var_name;
				Table *t = table_list[search_table[t_name]];
				table_var_names.push_back(make_pair(t, var_name));
			} else {
				vector<pair<string, pair<int, int> > > sort_4var_names;
				for (int i = 0; i < tables.size(); i++) {
					Table *t = tables[i];
					for (map<string, string>::iterator j = t->attr.begin(); j != t->attr.end(); ++j)
						if (str2any<int>(j->second))
							sort_4var_names.push_back(make_pair(j->first, make_pair(i, str2any<int>(j->second))));
				}
				sort(sort_4var_names.begin(), sort_4var_names.end(), cmp_4var_names);

				for (int i = 0; i < sort_4var_names.size(); i++)
					table_var_names.push_back(make_pair(tables[sort_4var_names[i].second.first], sort_4var_names[i].first));
				break;
			}
		}

		cin >> n2;
		while (n2--) {
			string type;

			cin >> type;
			//cout << type << endl;
			if (strcmp(type.c_str(), "int") == 0) {
				spj_condition_obj<int> *sco1 = new spj_condition_obj<int>;
				spj_condition_obj<int> *sco2 = new spj_condition_obj<int>;
				spj_condition<int> *sc = new spj_condition<int>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				//cout << sco1->var_con << ' ' << t_name << ' ' << sco1->var_name << ' ' << sco1->value << endl;
				//cout << sco1 << endl;

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				//cout << sc -> op << endl;

				exprs.push_back(make_pair("int", (void *)sc));

				//cout << exprs[exprs.size() - 1].second << endl;

			} else if (strcmp(type.c_str(), "long") == 0) {
				spj_condition_obj<long long> *sco1 = new spj_condition_obj<long long>;
				spj_condition_obj<long long> *sco2 = new spj_condition_obj<long long>;
				spj_condition<long long> *sc = new spj_condition<long long>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("long long", (void *)sc));
			} else if (strcmp(type.c_str(), "float") == 0) {
				spj_condition_obj<float> *sco1 = new spj_condition_obj<float>;
				spj_condition_obj<float> *sco2 = new spj_condition_obj<float>;
				spj_condition<float> *sc = new spj_condition<float>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("float", (void *)sc));
			} else if (strcmp(type.c_str(), "double") == 0) {
				spj_condition_obj<double> *sco1 = new spj_condition_obj<double>;
				spj_condition_obj<double> *sco2 = new spj_condition_obj<double>;
				spj_condition<double> *sc = new spj_condition<double>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("double", (void *)sc));
			} else {
				spj_condition_obj<string> *sco1 = new spj_condition_obj<string>;
				spj_condition_obj<string> *sco2 = new spj_condition_obj<string>;
				spj_condition<string> *sc = new spj_condition<string>;
				string t_name;

				cin >> sco1->var_con >> t_name >> sco1->var_name >> sco1->value;
				sco1->table = table_list[search_table[t_name]];

				cin >> sco2->var_con >> t_name >> sco2->var_name >> sco2->value;
				sco2->table = table_list[search_table[t_name]];

				sc->a = *sco1;
				sc->b = *sco2;
				cin >> sc->op;

				exprs.push_back(make_pair("string", (void *)sc));
			}
		}
		cin >> conj;
		if (conj == "NO")
			conj = "";

		//spj
		ostringstream oss;
		output_spj(oss, tables, table_var_names, exprs, conj, join_algo);

		cout << oss.str() << endl;
	}
}

int string_split(std::string s, std::vector<std::string> &sql_parts, std::string delimiter = "\n") {
	// std::string s = "2\nREGION\nNATION\n4\nREGION R_REGIONKEY\nREGION R_NAME\nNATION N_NAME\nNATION N_NATIONKEY\n0\nNO";
	// std::string delimiter = "\n";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		sql_parts.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	if (s.length() != 0) {
		sql_parts.push_back(s);
	}
	return sql_parts.size();
}

// std::pair<int, std::set<std::string> > execute4opt(const std::string &user_order){
//     std::set<std::string> s;
//     return std::make_pair(1,s);
// };

typedef std::pair<std::string, int> PAIR;

bool cmp_by_value(const PAIR &lhs, const PAIR &rhs) {
	return lhs.second < rhs.second;
}

/*
std::string sql_rewrite(std::string sql) {
	if (sql[0] != '-' && sql[0] != '1') {
		string str = "", user_order = "";
		ifstream fin("join_l_sql.txt");
		getline(fin, str);
		while (str != "") {
			user_order = user_order + str + '\n';
			getline(fin, str);
		}
		fin.close();
		return user_order;
	}
std::vector<std::string> sql_parts;
// const std::string get_index = "1\nSUPPLIER\n1\nS_NAME\n0\nNO";
if (string_split(sql, sql_parts) < 6) {
	std::cout << "syntax error" << std::endl;
	return sql;
}
cout << "STEP1: SPLIT" << endl;
for (auto s : sql_parts) {
	std::cout << s << std::endl;
}
// one table no rewrite
if (sql_parts[0] == "1") {
	return sql;
}
int condition_idx = 1;
condition_idx += std::stoi(sql_parts[0]);
condition_idx += std::stoi(sql_parts[condition_idx]);
condition_idx += 1;
int condition_num = std::stoi(sql_parts[condition_idx]);
std::map<std::string, int> join_cost;
std::vector<std::string> equal_select;
std::vector<std::string> range_select;
for (int i = 0; i < condition_num; i++) {
	int left_idx = condition_idx + i * 4 + 2;
	int right_idx = left_idx + 1;
	int type_idx = left_idx + 2;
	if (std::stoi(sql_parts[left_idx].substr(0, 1)) ^ std::stoi(sql_parts[right_idx].substr(0, 1))  ) {
		//select
		if (sql_parts[type_idx] == "=") {
			for (int j = -1; j <= 2; j++) {
				equal_select.push_back(sql_parts[left_idx + j]);
			}
		} else {
			for (int j = -1; j <= 2; j++) {
				range_select.push_back(sql_parts[left_idx + j]);
			}
		}
	} else {
		//two columns,calculate cost
		std::vector<std::string> left_part;
		std::vector<std::string> right_part;
		if (string_split(sql_parts[left_idx], left_part, " ") != 4) {
			std::cout << "syntax error" << std::endl;
		}
		if (string_split(sql_parts[right_idx], right_part, " ") != 4) {
			std::cout << "syntax error" << std::endl;
		}
		std::string left_table = left_part[1];
		std::string left_col = left_part[2];
		std::string right_table = right_part[1];
		std::string right_col = right_part[2];
		//std::string left_sql = "1\n" + left_table + "\n1\n" + left_col + "\n0\nNO";
		//std::string right_sql = "1\n" + right_table + "\n1\n" + right_col + "\n0\nNO";
		std::string left_sql = "1\n" + left_table + "\n1\n*\n0\nNO";
		std::string right_sql = "1\n" + right_table + "\n1\n*\n0\nNO";
		cout << "STEP2: before e4opt_left" << endl;
		cout << left_sql << endl;
		auto left_result = execute4opt(left_sql);
		cout << "STEP3: before e4opt_right" << endl;
		auto right_result = execute4opt(right_sql);
		cout << "STEP4: after e4opt" << endl;
		// std::set<std::string> t1,t2;
		// auto left_result = std::make_pair(1,t1);
		// auto right_result = std::make_pair(1,t2);
		int left_size = left_result.first;
		int right_size = right_result.first;
		std::set<std::string> left_index_columns = left_result.second;
		std::set<std::string> right_index_columns = right_result.second;
		int left_score = 0, right_score = 0;
		if (left_index_columns.find(left_col) != left_index_columns.end()) {// no index
			left_score = (int)(1000 / left_size);
		} else {
			left_score = -left_size * 10;
		}
		if (right_index_columns.find(right_col) != right_index_columns.end()) {
			right_score = (int)(1000 / right_size);
		} else {
			right_score = -right_size * 10;
		}
		int score = left_score + right_score;
		std::string cond;
		for (int j = -1; j <= 2; j++) {
			cond += sql_parts[left_idx + j] + "\n";
		}
		cond = cond.substr(0, cond.length() - 1);
		join_cost[cond] = score;
	}
}
std::string newsql;
for (int i = 0; i <= condition_idx; i++) {
	newsql += sql_parts[i] + "\n";
}
for (int i = 0; i < equal_select.size(); i++) {
	newsql += equal_select[i] + "\n";
}
for (int i = 0; i < range_select.size(); i++) {
	newsql += range_select[i] + "\n";
}
// std::vector<std::pair<std::string,int> > tmp;
// for(auto& i:join_cost){
//     tmp.push_back(i);
// }
// std::sort(tmp.begin(),tmp.end(),[&](std::pair<std::string,int>& a, std::pair<std::string,int>& b){
//     return a.second < b.second;
// });
std::vector<PAIR> tmp(join_cost.begin(), join_cost.end());
std::sort(tmp.begin(), tmp.end(), cmp_by_value);
for (auto i : tmp) {
	newsql += i.first + "\n";
}
newsql += sql_parts[sql_parts.size() - 1];
return newsql;
} */

void test_05() {
	//spj_init();
	//cout << "init OK: " << table_list.size() << "tables" << endl << endl;

	test_print_all_tables();
	system("pause");
	while (1) {
		string str = "", user_order = "";
		ifstream fin("soh.txt");
		getline(fin, str);
		while (str != "") {
			user_order = user_order + str + '\n';
			getline(fin, str);
		}
		fin.close();
		cout << "ORIGIN order:" << endl;
		cout << user_order << endl;

		if (user_order[0] != '-') {
			cout << endl << "OPTIMIZED order:" << endl;
			//cout << sql_rewrite(user_order) << endl;
		}

		cout << endl << "EXECUTING..." << endl << endl;
		execute(user_order);

		if (user_order[0] == '-')
			test_print_all_tables();
		system("pause");
	}

	//fin.close();
}

void test_final() {
	//spj_init();
	//cout << "init OK: " << table_list.size() << "tables" << endl << endl;
	//istream &fin = cin;
	string str = "", user_order = "";

	test_print_all_tables();
	cout << "Press ENTER to continue";
	getline(cin, str);
	while (1) {
		str = "";
		user_order = "";
		ifstream fin("soh.txt");
		getline(fin, str);
		while (str != "") {
			user_order = user_order + str + '\n';
			getline(fin, str);
		}
		fin.close();
		cout << "ORIGIN order:" << endl;
		cout << user_order << endl;

		if (user_order[0] != '-') {
			cout << endl << "OPTIMIZED order:" << endl;
			//cout << sql_rewrite(user_order) << endl;
		}

		cout << endl << "EXECUTING..." << endl << endl;
		execute(user_order);

		if (user_order[0] == '-')
			test_print_all_tables();
		cout << "Press ENTER to continue";
		getline(cin, str);
		//system("pause");
	}

	//fin.close();
}

void final(const string &user_order) {
	//spj_init();
	//cout << "init OK: " << table_list.size() << "tables" << endl << endl;
	//istream &fin = cin;
	string str = "";

	test_print_all_tables();

	if (user_order[0] != '-') {
		cout << endl << "OPTIMIZED order: No OPT" << endl;
		//cout << sql_rewrite(user_order) << endl;
	}

	cout << endl << "EXECUTING..." << endl << endl;
	execute(user_order);

	if (user_order[0] == '-')
		test_print_all_tables();

	//fin.close();
}

/*
int main() {
//	string *t = new string;
//	*t = "hello world";
//	func<string>(t);

	test_final();
	return 0;

	test_05();
	return 0;

	test_execute();
	return 0;

	spj_user();
	return 0;

	test_nestedjoin();
	return 0;

	test_nestedjoin_triple();
	return 0;

	test_alljoin();
	return 0;

	test_select_project();
	return 0;

	test_kmerge();
	return 0;

	test_str2any();
	return 0;

	test_value();
	return 0;

	int x = 33;
	ostringstream oss;
	oss << x;
	cout << oss.str() << endl;

	void *p = NULL;

	vector<pair<string, void *> > *t = new vector<pair<string, void *> >;

	Data<int> a;
	a.x = 77;
	void *pa = &a;

	Data<double> b;
	b.x = 3.1415926;
	void *pb = &b;

	Data<string> c;
	c.x = "hello world";
	void *pc = &c;

	(*t).push_back(make_pair("int", pa));
	(*t).push_back(make_pair("double", pb));
	(*t).push_back(make_pair("string", pc));

	func_void(t);
	return 0;
}*/