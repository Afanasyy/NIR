#include <iostream>
#include <numbers>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
#include <ctime>
#include <clocale> 
#include <windows.h>
#include "sqlite/sqlite3.h"
#include <time.h>

#define EARTH_RADIUS 6372795 
#define COUNT_CHILDREN 4
//#define CONST_DIS 0
#define INPUT false  // true - ручной ввод; false - случайная генерация

using namespace std;

class House {
public:
    House(const int& children, const map<string, int>& dis, const string& cord, const int& id) {
        this->children = children;
        this->dis = dis;
        this->cord = cord;
        this->id = id;
    }
    void operator = (const House& other) {
        this->children = other.children;
        this->dis = other.dis;
        this->cord = other.cord;
        this->id = other.id;
    }
    House() {}
    int getChildren() {
        return children;
    }
    map<string, int> getDis() {
        return dis;
    }
    pair<string, int> getMin_dis() {
        pair<string, int> m;
        m.second = INT_MAX;
        for (auto i : dis)
            if (i.second < m.second) m = i;
        return m;
    }
    int getDisToSch(const string& sch) {
        for (auto it : dis) {
            if (it.first == sch) return it.second;
        }
    }
    vector<string>getNearDis(const int& cur) {
        vector<string>ans;
        vector<pair<string, int>>tmp;
        for (const auto& it : dis) tmp.push_back(it);
        sort(tmp.begin(), tmp.end(), [](auto& l, auto& r)->bool {return l.second > r.second; });
        for (const auto& it : tmp) if (it.second < cur) ans.push_back(it.first);
        return ans;
    }
private:
    map<string, int>dis;
    int children;
    string cord;
    int id;
};

map<int, House>data_data;
map<string, pair<string, pair<string,string>>>schools;
map<int, string>addr;
map<string, int>const_count;
bool fl, set_cl = false;
int sum_children = 0;
vector<pair<int, int>>all_houses;
map<string, int>templ_classes;
vector<vector<pair<int, string>>> const_h;
string path = "db2.db";

int CONST_DIS;

auto solution_0(map<string, pair<vector<pair<int, int>>, int>>&,int&)->int;
auto calcEf(const map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>&) ->pair<int, double>;
auto calcCountClasses(const int&)->int;
auto setClasses(const int&, map<string, int>)->void;
auto foo(vector<string>&, const string&)->bool;
auto getH(map<string, int>&, pair<int, string>&, string&, map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>&)->bool;
auto smart_search(const vector<pair<int, int>>&, int, int, vector<pair<vector<pair<int, int>>, int>>, int, int, int&)->vector<pair<vector<pair<int, int>>, int>>;
auto solution_1()->pair<bool, pair<map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>, map<string, int>>>;
auto getHs(map<string, int>&, int&, vector<int>& , string& , string& , map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>&)->bool;
auto solution_2(map<string, pair<vector<pair<int, int>>, int>>&, const int&)->pair<bool, pair<map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>, map<string, int>>>;
auto calculateTheDistance(const double&, const double&, const double&, const double&)->double;
auto readFromTXT()->void;
auto readHFromDB()->void;
auto readSFromDB()->void;
auto writeToDB(const string&, map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>&)->void;
auto UTF8to1251(string const&)->string;


int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    //srand(time(0));

    clock_t start, end;
    vector<double>seconds;
    start = clock();
    readSFromDB();
    readHFromDB();
    end = clock();
    seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);


    //readFromTXT();



    map<string, pair<vector<pair<int, int>>, int>> ans;
    int l;

    start = clock();
    int t = solution_0(ans, l);
    end = clock();
    seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);

    switch (t) {
    case 2: {
        auto f = [](const auto& sol, const int& id)->string {
            for (const auto& it : sol.second.first)
                for (const auto& it2 : it.second)
                    if (it2.second == id) return it.first;
        };
        auto output = [](const auto& sol) {
            for (const auto& it : sol.second.first) {
                cout << schools[it.first].second.first << " / " << schools[it.first].second.second << '\n';
                for (const auto& it2 : it.second)
                    cout << "\t" << addr[it2.second] << '\t' << it2.first.first.second << "\n";
            }};

        CONST_DIS = 0;
        start = clock();
        auto sol_0 = solution_2(ans, l);
        auto ef_0 = calcEf(sol_0.second.first);
        cout << endl << endl << ef_0.first << " / " << ef_0.second << endl << endl;
        //output(sol_0);
        writeToDB("ans_0", sol_0.second.first);
        end = clock();
        seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);

        cout << "\n###################\n\n";

        start = clock();
        fl = true;//без учета детей
        auto sol_1 = solution_1();
        auto ef_1 = calcEf(sol_1.second.first);
        cout << endl << endl << ef_1.first << " / " << ef_1.second << endl << endl;
        //output(sol_1);
        writeToDB("ans_1", sol_1.second.first);
        end = clock();
        seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);

        cout << "\n###################\n\n";

        start = clock();
        fl = false;//с учетом детей
        auto sol_2 = solution_1();
        auto ef_2 = calcEf(sol_2.second.first);
        cout << endl << endl << ef_2.first << " / " << ef_2.second << endl << endl;
        //output(sol_2);
        writeToDB("ans_2", sol_2.second.first);
        end = clock();
        seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);


        cout << "\n###################\n###################\n\n";


        CONST_DIS = 300;
        start = clock();
        auto sol_3 = solution_2(ans, l);
        auto ef_3 = calcEf(sol_3.second.first);
        cout << endl << endl << ef_3.first << " / " << ef_3.second << endl << endl;
        //output(sol_0);
        writeToDB("ans_3", sol_3.second.first);
        end = clock();
        seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);

        cout << "\n###################\n\n";

        start = clock();
        fl = true;//без учета детей
        auto sol_4 = solution_1();
        auto ef_4 = calcEf(sol_4.second.first);
        cout << endl << endl << ef_4.first << " / " << ef_4.second << endl << endl;
        //output(sol_1);
        writeToDB("ans_4", sol_4.second.first);
        end = clock();
        seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);

        cout << "\n###################\n\n";

        start = clock();
        fl = false;//с учетом детей
        auto sol_5 = solution_1();
        auto ef_5 = calcEf(sol_5.second.first);
        cout << endl << endl << ef_5.first << " / " << ef_5.second << endl << endl;
        //output(sol_2);
        writeToDB("ans_5", sol_5.second.first);
        end = clock();
        seconds.push_back((double)(end - start) / CLOCKS_PER_SEC);


        cout << "\n###################\n\n";
        break;
    }
    case 1: {
        cout << "Нулевой алгоритм не нашел решение\n";
        break;
    }
    case 0: {
        cout << "Невозможно распределить при таком кол-ве детей\n";
        break;
    }
    }

    return 0;
}

int callback_1(void* data, int argc, char** argv, char** azColName) {
    int id;
    map<string, int>dis;
    int children;
    string cord;
    for (int i = 0; i < argc; i++) {
        string tmp = azColName[i];
        if (tmp == "id")
            id = atoi(argv[i]);
        else if (tmp == "lat") {
            cord = argv[i];
            cord += ", ";
        }
        else if (tmp == "cord")
            cord = argv[i];
        else if (tmp == "lon")
            cord += argv[i];
        else if (tmp == "child") {
            if (INPUT)
                children = atoi(argv[i]);
            else
                children = rand() % COUNT_CHILDREN;
            sum_children += children;
        }
        else if (tmp != "addr") {
            if (!set_cl) templ_classes[tmp];
            int t = atoi(argv[i]);
            if (t == 0) ++t;
            dis[tmp] = t;
        }
        else if (tmp == "addr")
            addr[id] = (UTF8to1251(argv[i]));
    }
    if (id == 2) {
        cout << "Csac";
    }
    if (children != 0) all_houses.push_back({ children,id });
    data_data[id] = { children,dis,cord,id };
    set_cl = true;
    return 0;
}

int callback_2(void* data, int argc, char** argv, char** azColName) {
    string id, addr, cord, name;
    for (int i = 0; i < argc; i++) {
        string tmp = azColName[i];
        if (tmp == "id")
            id = argv[i];
        else if (tmp == "addr")
            addr = UTF8to1251(argv[i]);
        else if (tmp == "cord")
            cord = argv[i];
        else if (tmp == "name")
            name = UTF8to1251(argv[i]);
    }
    schools[id] = { cord,{addr,name} };
    return 0;
}

int callback_3(void* data, int argc, char** argv, char** azColName) {
    return 0;
}

int solution_0(map<string, pair<vector<pair<int, int>>, int>>& ans,int&last) {
    last = const_h.size() == 0 ? 0 : const_h.size() - 1;
    const_h.push_back({});
    for (const auto& it : templ_classes) ans[it.first];
    auto count = calcCountClasses(sum_children);
    if (count == -1)
        return 0;

    setClasses(count, templ_classes);

    sort(all_houses.begin(), all_houses.end(), [](auto& l, auto& r)->bool {return l.first < r.first; });

    vector<int>tmp;
    bool flag2;
    while (!all_houses.empty()) {
        flag2 = false;                
        for (const auto& it : templ_classes) {
            if (all_houses.empty()) break;
            auto t = data_data[all_houses.back().second].getMin_dis();
            if (t.second <= CONST_DIS) {
                ans[t.first].second += data_data[all_houses.back().second].getChildren();
                const_h.back().push_back({ all_houses.back().second, t.first });
                all_houses.pop_back();
                continue;
            }
            else
                if (ans[it.first].second + all_houses.back().first <= const_count[it.first] * 28) {
                    ans[it.first].first.push_back(all_houses.back());
                    ans[it.first].second += all_houses.back().first;
                    all_houses.pop_back();
                    flag2 = true;
                    continue;
                }
        }
        if (!flag2 && !all_houses.empty()) {
            tmp.push_back(all_houses.back().first);
            all_houses.pop_back();
        }
    }
    if (tmp.empty()) return 2;
    else return 1;
}

void readHFromDB() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    string sql;
    string data = "Callback function called";
    /* Open database */
    rc = sqlite3_open(path.c_str(), &db);

    if (rc) {
        cout << "Can't open database:\n" << sqlite3_errmsg(db);
        return;
    }
    else
        cout << "Opened database successfully\n";


    /* Create SQL statement */
    sql = "SELECT * from final";
    /* Execute SQL statement */

    rc = sqlite3_exec(db, sql.c_str(), callback_1, (void*)data.c_str(), &zErrMsg);

    if (rc != SQLITE_OK) {
        cout << "SQL error:\n" << zErrMsg;
        sqlite3_free(zErrMsg);
    }
    else
        cout << "Operation done successfully\n";
    sqlite3_close(db);
}

void readSFromDB() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    string sql;
    string data = "Callback function called";
    /* Open database */
    rc = sqlite3_open(path.c_str(), &db);

    if (rc) {
        cout << "Can't open database:\n" << sqlite3_errmsg(db);
        return;
    }
    else
        cout << "Opened database successfully\n";


    /* Create SQL statement */
    sql = "SELECT * from schools";
    /* Execute SQL statement */

    rc = sqlite3_exec(db, sql.c_str(), callback_2, (void*)data.c_str(), &zErrMsg);

    if (rc != SQLITE_OK) {
        cout << "SQL error:\n" << zErrMsg;
        sqlite3_free(zErrMsg);
    }
    else
        cout << "Operation done successfully\n";
    sqlite3_close(db);
}

void readFromTXT() {
    ifstream file("cord.txt");
    vector<string>cord_schools;
    vector<pair<string, int>>cord_houses;
    while (!file.eof()) {//считывание из файла координат домов и школ
        string str;
        getline(file, str);
        if (str[0] == '!') {
            str.erase(str.begin());
            cord_schools.push_back(str);
            templ_classes[str];
        }
        else {
            size_t t2 = str.find(';');
            auto tmp1 = str.substr(0, t2);
            auto tmp2 = stoi(str.substr(t2 + 2));
            cord_houses.push_back({ tmp1, tmp2 });
        }
    }

    int id = 0;
    for (auto cord_house : cord_houses) {
        map<string, int> dis;
        int children;
        if (INPUT)
            children = cord_house.second;
        else
            children = rand() % COUNT_CHILDREN;//случайная генерация кол-ва детей от 1 до 20
        sum_children += children;
        if (children != 0) all_houses.push_back({ children,id });
        for (auto cord_school : cord_schools) {//расчет расстояния от дома до каждой школы
            int t = cord_house.first.find(',');
            auto tmp1 = stod(cord_house.first.substr(0, t)), tmp2 = stod(cord_house.first.substr(t + 2)), tmp3 = stod(cord_school.substr(0, t)), tmp4 = stod(cord_school.substr(t + 2));
            dis[cord_school] = calculateTheDistance(tmp1, tmp2, tmp3, tmp4);
        }
        data_data[id] = { children,dis,cord_house.first,id }; //создание объекта одного 
        ++id;
    }
}

string quotesql(const string& s) {
    return string("'") + s + string("'");
}

void writeToDB(const string&name, map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>&ans) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    string sql;
    string data = "Callback function called";
    string idh, ids;
    /* Open database */
    rc = sqlite3_open(path.c_str(), &db);

    if (rc) {
        cout << "Can't open database:\n" << sqlite3_errmsg(db);
        return;
    }
    else
        cout << "Opened database successfully\n";
    sql = "DELETE FROM "+name;
    rc = sqlite3_exec(db, sql.c_str(), callback_3, (void*)data.c_str(), &zErrMsg);

    sql = "CREATE TABLE if not exists " + name + " (idh TEXT, ids TEXT, child INTEGER)";
    rc = sqlite3_exec(db, sql.c_str(), callback_3, (void*)data.c_str(), &zErrMsg);

    for (const auto& it : ans) {
        for (const auto& it2 : it.second) {
            sql = "INSERT INTO " + name + " (idh, ids, child) VALUES (" + quotesql(to_string(it2.second)) + ',' + quotesql(it.first) + ',' + quotesql(to_string(it2.first.first.second)) + ");";
            rc = sqlite3_exec(db, sql.c_str(), callback_3, (void*)data.c_str(), &zErrMsg);
        }
    }    
    sqlite3_close(db);
}

double calculateTheDistance(const double& a1, const double& b1, const double& a2, const double& b2) {
    double cl1 = cos(a1 * numbers::pi / 180);
    double cl2 = cos(a2 * numbers::pi / 180);
    double sl1 = sin(a1 * numbers::pi / 180);
    double sl2 = sin(a2 * numbers::pi / 180);
    double cdelta = cos(b2 * numbers::pi / 180 - b1 * numbers::pi / 180);
    double sdelta = sin(b2 * numbers::pi / 180 - b1 * numbers::pi / 180);
    double y = sqrt(pow(cl2 * sdelta, 2) + pow(cl1 * sl2 - sl1 * cl2 * cdelta, 2));
    double x = sl1 * sl2 + cl1 * cl2 * cdelta;
    return atan2(y, x) * EARTH_RADIUS;
}

pair<int, double> calcEf(const map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>& B) {
    pair<int, double>ef;
    for (auto it : B) {
        for (auto it2 : it.second) {
            ef.first += it2.first.first.first * it2.first.first.second;
            ef.second += double(it2.first.first.second) / it2.first.first.first;            
        }
    }
    return ef;
}

int calcCountClasses(const int& s) {
    double upper = s / 25.0, lower = s / 28.0;
    vector<int>ans;
    for (int i = ceil(lower); i <= floor(upper); ++i)
        ans.push_back(i);
    pair<int, int>mmin = { 0,0 };
    for (int i = 0; i < ans.size(); ++i) {
        int l = ans[i] * 25, u = ans[i] * 28;
        int m = min(s - l, u - s);
        if (mmin.second < m)
            mmin = { i,m };
    }
    if (ans.empty())
        return -1;
    return ans[mmin.first];
}

void setClasses(const int& count_classes, map<string, int>templ_class) {
    int min_c = floor(double(count_classes) / templ_class.size());
    int remains = count_classes - min_c * templ_class.size();//остаток
    for (auto& it : templ_class) it.second += min_c;
    while (remains != 0) {
        int tmp = rand() % templ_class.size();
        auto it = templ_class.begin();
        advance(it, tmp);
        it->second++;
        --remains;
    }
    for (auto it : templ_class) const_count[it.first] = it.second;
}

bool foo(vector<string>& v, const string& t) {
    for (auto it : v)        if (it == t)            return false;
    return true;
}

bool getH(map<string, int>& s_B, pair<int, string>& h, string& sch, map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>& A) {
    int c = 0;
    bool flag2 = false;
    for (auto it : s_B) {
        ++c;
        if (it.second > 28 * const_count[it.first]) {
            pair<pair<int, int>, string>min;
            min.first.second = INT_MAX;
            int count = 0;
            for (auto it2 : A[it.first]) {
                pair<string, int>m = { {0,0},INT_MAX };
                int current = it2.first.first.first;
                int children;
                if (fl)
                    children = 1;
                else
                    children = it2.first.first.second;
                auto dis = data_data[it2.second].getDis();
                for (auto it3 : dis)
                    if (m.second > abs((current - it3.second) * children) && current != it3.second && foo(it2.first.second, it3.first))
                        m = { it3.first,abs((current - it3.second) * children) };
                if (min.first.second > m.second)
                    min = { {count, m.second}, m.first };
                ++count;
            }
            if (min.first.second == INT_MAX) h.first = -1;
            else
                h = { min.first.first, it.first };
            sch = min.second;
            return false;
        }
        else if (it.second < 25 * const_count[it.first]) {
            pair<pair<int, int>, string>min;
            min.first.second = INT_MAX;
            for (auto it2 : A) {
                if (it2.first != it.first) {
                    pair<int, int>m = { 0,INT_MAX };
                    int count = 0;
                    for (auto it3 : it2.second) {
                        int current = it3.first.first.first;
                        int children;
                        if (fl)
                            children = 1;
                        else
                            children = it3.first.first.second;
                        int new_dis = data_data[it3.second].getDisToSch(it.first);
                        if (m.second > abs((current - new_dis) * children) && current != new_dis && foo(it3.first.second, it.first) && (s_B[it2.first] - A[it2.first][count].first.first.second > 25 * const_count[it2.first]))
                            m = { count,abs((current - new_dis) * children) }, flag2 = true;
                        ++count;
                    }
                    if (min.first.second > m.second) min = { {m.first, m.second},it2.first };
                }
            }
            if (!flag2) continue;
            h = { min.first.first, min.second };
            sch = it.first;
            return false;
        }
    }
    if (c == const_count.size()) return true;
}

pair<bool, pair<map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>, map<string, int>>> solution_1() {
    int last = const_h.size() == 0 ? 0 : const_h.size() - 1;
    const_h.push_back({});
    map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>> A, B;
    //массив: школа -> дом -> характеристики, B_2 - отсортированный по коэф
    map<string, int> s_A, s_B, count_class;//сумма детей в каждой школе, кол-во классов
    int s = 0;
    for (auto house : data_data) {
        pair<string, int> tmp = house.second.getMin_dis();//минимальное расстояние
        auto t = vector<string>(0);
        int c = house.second.getChildren();
        if (c != 0) {
            if (tmp.second <= CONST_DIS)
                const_h.back().push_back({ house.first,tmp.first });
            else
                A[tmp.first].push_back({ {{tmp.second,c },t}, house.first });
        }
        //распределение домов к школе с максимальным коэф
        s_A[tmp.first] += c;//подсчет суммы детей в школе
        count_class[tmp.first] = 0;
    }


    int count_iteration = 0;
    if(CONST_DIS==0)
    writeToDB("ans_6", A);
    else
        writeToDB("ans_7", A);
    B = A;
    s_B = s_A;
    pair<int, string>h;
    string sch;
    int l = 0;
    while (!getH(s_B, h, sch, B)) {//поиск дома для перекидывания
        if (h.first == -1)
            return { false, {B,s_B} };
        int* ptr = &(B[h.second][h.first].first.first.second);
        s_B[h.second] -= *ptr;
        s_B[sch] += *ptr;
        B[h.second][h.first].first.first.first = data_data[B[h.second][h.first].second].getDisToSch(sch);
        B[h.second][h.first].first.second.push_back(h.second);
        if (B[h.second][h.first].first.second.size() == 6)
            B[h.second][h.first].first.second.erase(B[h.second][h.first].first.second.begin());
        B[sch].push_back(B[h.second][h.first]);
        B[h.second].erase(B[h.second].begin() + h.first);
        auto t = calcEf(B);
        if (t.first < l)
            cout << "dasdasd";       
        l = t.first;
        //cout << "EF = " << t.first << '\n';
        ++count_iteration;
    }
    cout << "count_iteration = " << count_iteration << '\n';
    for (const auto& it : const_h[last])
        B[it.second].push_back({ {{data_data[it.first].getDisToSch(it.second),data_data[it.first].getChildren()},vector<string>(0)},it.first });
    return { true, {B,s_B} };
}

vector<pair<vector<pair<int, int>>, int>> smart_search(const vector<pair<int, int>>& list, int lower, int upper, vector<pair<vector<pair<int, int>>, int>>last_list, int last_ind, int lvl, int& count_iteration) {
    ++count_iteration;
    if (lower <= 0 && upper >= 0) {
        last_list.back().second = -5;
        return last_list;
    }
    if (last_ind == -1) {
        last_list.back().second = -4;
        return last_list;
    }
    int l = lower, u = upper;
    for (int i = last_ind; i >= 0; --i)
        if (u - list[i].first >= 0) {
            !last_list.empty() ? last_list.back().first.push_back(list[i]) : last_list.push_back({ { list[i] },false });
            l -= list[i].first;
            u -= list[i].first;
            auto y = smart_search(list, l, u, last_list, i - 1, lvl + 1, count_iteration);
            switch (y.back().second)
            {
            case -5: {
                auto t = y.back();
                t.first.erase(t.first.begin() + (lvl - 1), t.first.end());
                t.second = -3;
                auto z = smart_search(list, lower, upper, { t }, i - 1, lvl, count_iteration);
                for (auto& it : z)
                    if (it.second == -5 || it.second == 2) {
                        it.second = 2;
                        y.push_back(it);
                    }
                return y;
            }
            case -4: {
                return y;
            }
            case -1: {
                auto t = last_list.back();
                last_list.pop_back();
                t.first.erase(t.first.begin() + (lvl - 1), t.first.end());
                t.second = -3;
                auto z = smart_search(list, lower, upper, { t }, i - 1, lvl, count_iteration);
                for (auto& it : z)
                    if (it.second == -5 || it.second == 2) {
                        it.second == 2;
                        last_list.push_back(it);
                    }
                return last_list.empty() ? z : last_list;
            }
            default:
                break;
            }
            if (!y.back().first.empty() && (y.back().second == -5 || y.back().second == 2)) {
                if (lvl == 1) {
                    y.push_back({ {},-4 });
                    auto x = smart_search(list, lower, upper, y, i - 1, lvl, count_iteration);
                    return x;
                }
                return y;
            }
        }
    return  { {{{}},-1} };
}

bool getHs(map<string, int>& s_B, int& h_o, vector<int>& h_in, string& s_o, string& s_in, map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>& A) {
    int count_iteration = 0;
    for (const auto& it : A)
        for (const auto& it2 : it.second) {
            h_in = vector<int>();
            int children = it2.first.first.second;
            int cur = it2.first.first.first;
            auto dis = data_data[it2.second].getNearDis(it2.first.first.first);
            if (dis.empty()) continue;
            pair<pair<int, string>, vector<pair<int, int>>>best = { {0,""},{} };
            for (const auto& it4 : dis) {
                int new_dis = data_data[it2.second].getDisToSch(it4);
                vector<pair<int, int>>list;
                for (const auto& it3 : A[it4])
                    list.push_back({ it3.first.first.second,it3.second });
                int lower = s_B[it4] - (28 * const_count[it4]) + children;
                int upper = (28 * const_count[it.first]) - s_B[it.first] + children;
                vector<int>tmp2;
                if (list.empty()) continue;
                sort(list.begin(), list.end(), [](auto& l, auto& r)->bool {return l.first < r.first; });
                auto ans = smart_search(list, lower, upper, { {{},-4} }, list.size() - 1, 1, count_iteration);
                pair<int, int>best_2 = { 0,-1 };
                int c = 0;
                for (const auto& it5 : ans) {
                    if (it5.second != -5 && it5.second != 2) continue;
                    int tmp = 0;
                    for (const auto& it6 : it5.first)
                        tmp += (data_data[it6.second].getDisToSch(it4) - data_data[it6.second].getDisToSch(it.first)) * data_data[it6.second].getChildren();
                    if ((cur - new_dis) * children + tmp > best_2.first) best_2 = { (cur - new_dis) * children + tmp ,c };
                    ++c;
                }
                if (best_2.first > best.first.first) best = { {best_2.first,it4},ans[best_2.second].first };
            }
            if (best.first.first != 0) {
                h_o = it2.second;
                for (const auto& it : best.second) h_in.push_back(it.second);
                s_o = it.first;
                s_in = best.first.second;
                //cout << "count_iteration in rec = " << count_iteration << '\n';
                return false;
            }
        }
    //cout << "count_iteration in rec = " << count_iteration << '\n';
    return true;
}

pair<bool, pair<map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>>, map<string, int>>> solution_2(map<string, pair<vector<pair<int, int>>, int>>& ans,const int&last) {
    map<const string, vector<pair<pair<pair<int, int>, vector<string>>, int>>> A, B;
    map<string, int> s_A, s_B;//сумма детей в каждой школе
    for (const auto& it : ans) {
        s_A[it.first] = it.second.second;
        for (const auto& it2 : it.second.first) {
            auto dis = data_data[it2.second].getDisToSch(it.first);
            A[it.first].push_back({ {{dis,it2.first},vector<string>(0)},it2.second });
        }
    }
    auto t = calcEf(A);
    cout << "EF = " << t.first << '\n';


    if (CONST_DIS == 0)
        writeToDB("ans_8", A);
    else
        writeToDB("ans_9", A);


    B = A;
    s_B = s_A;

    int h_o, count_iteration = 0;
    vector<int>h_in;
    string s_o, s_in;
    while (!getHs(s_B, h_o, h_in, s_o, s_in, B)) {
        int tmp;
        for (int i = 0; i < B[s_o].size(); ++i) if (B[s_o][i].second == h_o) { tmp = i; break; }
        int* ptr = &(B[s_o][tmp].first.first.second);
        s_B[s_o] -= *ptr;
        s_B[s_in] += *ptr;
        B[s_o][tmp].first.first.first = data_data[B[s_o][tmp].second].getDisToSch(s_in);
        B[s_in].push_back(B[s_o][tmp]);
        B[s_o].erase(B[s_o].begin() + tmp);
        for (const auto& it : h_in) {
            for (int i = 0; i < B[s_in].size(); ++i) if (B[s_in][i].second == it) { tmp = i; break; }
            int* ptr = &(B[s_in][tmp].first.first.second);
            s_B[s_in] -= *ptr;
            s_B[s_o] += *ptr;
            B[s_in][tmp].first.first.first = data_data[B[s_in][tmp].second].getDisToSch(s_o);
            B[s_o].push_back(B[s_in][tmp]);
            B[s_in].erase(B[s_in].begin() + tmp);
        }
        auto t = calcEf(B);
       // cout << "EF = " << t.first << '\n';
        ++count_iteration;
    }
    cout << "count_iteration = " << count_iteration << '\n';
    for (const auto& it : const_h[last]) 
        B[it.second].push_back({ {{data_data[it.first].getDisToSch(it.second),data_data[it.first].getChildren()},vector<string>(0)},it.first });    
    return{ true,{B,s_B} };
}

string UTF8to1251(string const& utf8) {
    string s1251;
    for (int i = 0; i < utf8.size(); ++i) {
        int b1 = (unsigned char)utf8[i];
        if ((b1 >> 5) == 6) {
            int b2 = (unsigned char)utf8[i + 1];
            if ((b1 == 208) && (b2 >= 144 && b2 <= 191))
                s1251 += (char)(b2 + 48);
            else if ((b1 == 209) && (b2 >= 128 && b2 <= 143))
                s1251 += (char)(b2 + 112);
            else if ((b1 == 208) && (b2 == 129))
                s1251 += (char)(b2 + 39);
            else if ((b1 == 209) && (b2 == 145))
                s1251 += (char)(b2 + 39);
            ++i;
        }
        else if ((b1 >> 7) == 0)
            s1251 += b1;
    }
    return s1251;
}
