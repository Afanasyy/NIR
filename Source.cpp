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

#define EARTH_RADIUS 6372795 
#define COUNT_CHILDREN 2
#define INPUT false  // true - ручной ввод; false - случайная генерация

using namespace std;

class House {
public:
    House(const int& children, const map<string, int>& dis, const string& cord, const map<string, float>& ratio_dis, const int& id) {
        this->children = children;
        this->dis = dis;
        this->cord = cord;
        this->ratio_dis = ratio_dis;
        this->id = id;
    }
    void operator = (const House&other) {
        this->children = other.children;
        this->dis = other.dis;
        this->cord = other.cord;
        this->ratio_dis = other.ratio_dis;
        this->id = other.id;
    }
    House(){}
    string getCord() {
        return cord;
    }
    int getChildren() {
        return children;
    }
    map<string, int> getDis() {
        return dis;
    }
    map<string, float> getRatio_Dis() {
        return ratio_dis;
    }
    pair<string, float> getMin_dis() {
        pair<string, float> m;
        m.second = INT_MAX;
        for (auto i : dis)
            if (i.second < m.second) m = i;
        return m;
    }
    float getDifference(const string& school_m, const string& school_M) {
        return abs(dis[school_m] - dis[school_M]);
    }
    float getDifference_r(const string& school_m, const string& school_M) {
        return abs(ratio_dis[school_m] - ratio_dis[school_M]);
    }
    int getID() {
        return id;
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
    float getRatioDisToSch(const string& sch) {
        for (auto it : ratio_dis) {
            if (it.first == sch) return it.second;
        }
    }
private:
    map<string, int>dis;
    int children;
    string cord;
    map<string, float>ratio_dis;
    int id;
};

map<int, House>data_data;
map<int, string>addr;
map<string, int>const_count;
bool fl, set_cl = false;
int sum_children = 0;
vector<pair<string, pair<int, int>>>all_houses;
map<string, int>templ_classes;

auto solution_0(map<string, pair<vector<pair<string, pair<int, int>>>, int>>&)->int;
auto calcEf(const map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>&, const map<string, int>&) ->pair<int, float>;
auto calcCountClasses(const int&)->int;
auto setClasses(const int&, map<string, int>)->void;
auto foo(vector<string>&, const string&)->bool;
auto getH(map<string, int>&, pair<int, string>&, string&, map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>&)->bool;
auto smart_search(const vector<pair<int, int>>&, int, int, vector<pair<vector<pair<int, int>>, bool>>, int, int)->vector<pair<vector<pair<int, int>>, bool>>;
auto solution_1(pair<int, float>&)->pair<bool, pair<map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>, map<string, int>>>;
auto getHs(map<string, int>&, int&, vector<int>& , string& , string& , map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>&)->bool;
auto solution_2(map<string, pair<vector<pair<string, pair<int, int>>>, int>>&)->pair<bool, pair<map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>, map<string, int>>>;
auto calculateTheDistance(const float&, const float&, const float&, const float&)->float;
auto readFromTXT()->void;
auto readFromDB()->void;
auto UTF8to1251(string const&)->string;


int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    //srand(time(0));

    readFromDB();

    //readFromTXT(); 

    map<string, pair<vector<pair<string, pair<int, int>>>, int>> ans;
    if (solution_0(ans) > 1) {
        //auto sol_0 = solution_2(ans);
        //auto ef_0 = calcEf(sol_0.second.first, sol_0.second.second);

        pair<int, float> x_ef1_first, x_ef2_first, ef_1, ef_2;

        fl = true;//без учета детей
        auto sol_1 = solution_1(x_ef1_first);
        if (sol_1.first)
            ef_1 = calcEf(sol_1.second.first, sol_1.second.second);
        else cout << "Эффективное решение №1 не найдено\n";
        for (const auto& it : sol_1.second.first) {
            for (const auto& it2 : it.second) {
                if (it2.second == 588) {
                    cout << it.first;
                }
            }
        }


        fl = false;//с учетом детей
        auto sol_2 = solution_1(x_ef2_first);
        if (sol_2.first)
            ef_2 = calcEf(sol_2.second.first, sol_2.second.second);
        else cout << "Эффективное решение №2 не найдено\n";
        for (const auto& it : sol_2.second.first) {
            for (const auto& it2 : it.second) {
                if (it2.second == 588) {
                    cout << it.first;
                }
            }
        }
    }
    
    return 0;
}

int callback(void* data, int argc, char** argv, char** azColName) {
    int id;
    map<string, int>dis;
    map<string, float>ratio_dis;
    int children;    
    string cord;
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << '\n';
        string tmp = azColName[i];
        if (tmp == "id")
            id = atoi(argv[i]);
        else if (tmp == "lat") {
            cord = argv[i];
            cord += ", ";
        }
        else if (tmp == "lon")
            cord += argv[i];
        else if (tmp == "child") {
            if (INPUT)
                children = atoi(argv[i]);
            else
                children = rand() % COUNT_CHILDREN + 1;
            if (id == 588) children = 1;
            sum_children += children;
        }
        else if (tmp != "addr") {
            if (!set_cl) templ_classes[tmp];
            int t = atoi(argv[i]);
            if (t == 0) ++t;
            dis[tmp] = t;
            ratio_dis[tmp] = float(children) / t;
        }
        else if (tmp == "addr")
            addr[id] = (UTF8to1251(argv[i]));
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    all_houses.push_back({ cord,{children,id} });
    data_data[id]={children,dis,cord,ratio_dis,id };
    set_cl = true;
    //printf("\n");
    return 0;
}

int solution_0(map<string, pair<vector<pair<string, pair<int, int>>>, int>>& ans) {
    for (const auto& it : templ_classes) ans[it.first];
    auto count = calcCountClasses(sum_children);
    if (count == -1) {
        cout << "Невозможно распределить при таком кол-ве детей\n";
        return 0;
    }
    setClasses(count, templ_classes);

    sort(all_houses.begin(), all_houses.end(), [](auto& l, auto& r)->bool {return l.second.first < r.second.first; });

    vector<int>tmp;
    bool flag2;
    while (!all_houses.empty()) {
        flag2 = false;
        for (const auto& it : templ_classes) {
            if (ans[it.first].second + all_houses.back().second.first <= const_count[it.first] * 28) {
                ans[it.first].first.push_back(all_houses.back());
                ans[it.first].second += all_houses.back().second.first;
                all_houses.pop_back();
                flag2 = true;
                break;
            }
        }
        if (!flag2) {
            tmp.push_back(all_houses.back().second.first);
            all_houses.pop_back();
        }
    }
    if (tmp.empty()) return 2;
    else return 1;
}

void readFromDB() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    string sql;
    string data = "Callback function called";
    /* Open database */
    rc = sqlite3_open("db.db", &db);

    if (rc) {
        cout << "Can't open database:\n" << sqlite3_errmsg(db);
        return;
    }
    else
        cout << "Opened database successfully\n";


    /* Create SQL statement */
    sql = "SELECT * from final";
    /* Execute SQL statement */

    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data.c_str(), &zErrMsg);

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
        map<string, float> ratio_dis;//массивы расстояний и коэф расстояний
        map<string, int> dis;
        int children;
        if (INPUT)
            children = cord_house.second;
        else
            children = rand() % COUNT_CHILDREN + 1;//случайная генерация кол-ва детей от 1 до 20
        sum_children += children;
        all_houses.push_back({ {cord_house.first},{children,id} });
        for (auto cord_school : cord_schools) {//расчет расстояния от дома до каждой школы
            int t = cord_house.first.find(',');
            auto tmp1 = stod(cord_house.first.substr(0, t)), tmp2 = stod(cord_house.first.substr(t + 2)), tmp3 = stod(cord_school.substr(0, t)), tmp4 = stod(cord_school.substr(t + 2));
            dis[cord_school] = calculateTheDistance(tmp1, tmp2, tmp3, tmp4);
            ratio_dis[cord_school] = float(children) / dis[cord_school];//расчет коэф расстояния от дома до каждой школы
        }
        data_data[id] = { children,dis,cord_house.first,ratio_dis,id }; //создание объекта одного 
        ++id;
    }
}

float calculateTheDistance(const float& a1, const float& b1, const float& a2, const float& b2) {
    float cl1 = cos(a1 * numbers::pi / 180);
    float cl2 = cos(a2 * numbers::pi / 180);
    float sl1 = sin(a1 * numbers::pi / 180);
    float sl2 = sin(a2 * numbers::pi / 180);
    float cdelta = cos(b2 * numbers::pi / 180 - b1 * numbers::pi / 180);
    float sdelta = sin(b2 * numbers::pi / 180 - b1 * numbers::pi / 180);
    float y = sqrt(pow(cl2 * sdelta, 2) + pow(cl1 * sl2 - sl1 * cl2 * cdelta, 2));
    float x = sl1 * sl2 + cl1 * cl2 * cdelta;
    return atan2(y, x) * EARTH_RADIUS;
}

pair<int, float> calcEf(const map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>& B, const map<string, int>& s_B) {
    pair<int, float>ef;
    for (auto it : B) {
        for (auto it2 : it.second) {
            ef.first += it2.first.first.second.first * it2.first.first.second.second;
            ef.second += float(it2.first.first.second.second) / it2.first.first.second.first;
            if (ef.second == INFINITY) {
                cout << "STOP\n";
            }
        }
    }
    return ef;
}

int calcCountClasses(const int& s) {
    float upper = s / 25.0, lower = s / 28.0;
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
    int min_c = floor(float(count_classes) / templ_class.size());
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
    for (auto it : v)
        if (it == t)
            return false;
    if (v.size() == 2) v.erase(v.begin());
    return true;
}

bool getH(map<string, int>& s_B, pair<int, string>& h, string& sch, map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>& A) {
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
                int current = it2.first.first.second.first;
                int children;
                if (fl)
                    children = 1;
                else
                    children = it2.first.first.second.second;
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
                        int current = it3.first.first.second.first;
                        int children;
                        if (fl)
                            children = 1;
                        else
                            children = it3.first.first.second.second;
                        int new_dis = data_data[it3.second].getDisToSch(it.first);
                        if (m.second > abs((current - new_dis) * children) && current != new_dis && foo(it3.first.second, it.first) && (s_B[it2.first] - A[it2.first][count].first.first.second.second > 25 * const_count[it2.first]))
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

pair<bool, pair<map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>, map<string, int>>> solution_1(pair<int, float>& ef) {
    map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>> A, B;
    //массив: школа -> дом -> характеристики, B_2 - отсортированный по коэф
    map<string, int> s_A, s_B, count_class;//сумма детей в каждой школе, кол-во классов
    int s = 0;
    for (auto house : data_data) {
        pair<string, float> tmp = house.second.getMin_dis();//минимальное расстояние
        auto t = vector<string>(0);
        A[tmp.first].push_back({ {{ house.second.getCord(),{tmp.second,house.second.getChildren()} },t}, house.second.getID() });
        //распределение домов к школе с максимальным коэф
        s_A[tmp.first] += house.second.getChildren();//подсчет суммы детей в школе
        count_class[tmp.first] = 0;
        s += house.second.getChildren();
    }

    bool flag = false;

    auto EF = calcEf(A, s_A);
    ef = EF;

    auto count = calcCountClasses(s);

    int count_iteration = 0;
    bool flag2 = true;
    while (!flag) {
        B = A;
        s_B = s_A;
        pair<int, string>h;
        string sch;
        while (!(flag = getH(s_B, h, sch, B))) {//поиск дома для перекидывания
            if (h.first == -1) {
                cout << "Кол-во классов увеличенно на 1\n";
                count++;
                flag2 = false;
                setClasses(count, count_class);//true - перераспределение; false - копирование
                break;
            }
            int* ptr = &(B[h.second][h.first].first.first.second.second);
            s_B[h.second] -= *ptr;
            s_B[sch] += *ptr;
            B[h.second][h.first].first.first.second.first = data_data[B[h.second][h.first].second].getDisToSch(sch);
            B[h.second][h.first].first.second.push_back(h.second);
            B[sch].push_back(B[h.second][h.first]);
            B[h.second].erase(B[h.second].begin() + h.first);
            ++count_iteration;
        }
    }
    return { flag2, {B,s_B} };
}

vector<pair<vector<pair<int, int>>, bool>> smart_search(const vector<pair<int, int>>& list, int lower, int upper, vector<pair<vector<pair<int, int>>, bool>>last_list, int last_ind, int lvl) {
    if (lower <= 0 && upper >= 0) {
        last_list.back().second = true;
        return last_list;
    }
    if (last_ind == -1) {
        last_list.back().second = false;
        return last_list;
    }
    int l = lower, u = upper;
    for (int i = last_ind; i >= 0; --i)
        if (u - list[i].first >= 0) {
            !last_list.empty() ? last_list.back().first.push_back(list[i]) : last_list.push_back({ { list[i] },false });
            l -= list[i].first;
            u -= list[i].first;
            auto y = smart_search(list, l, u, last_list, i - 1, lvl + 1);
            if (y.empty())
                return vector<pair<vector<pair<int, int>>, bool>>();
            if (!y.back().first.empty() && y.back().first[0].first == -1 && !y.back().second)
                return smart_search(list, lower, upper, vector<pair<vector<pair<int, int>>, bool>>(), last_ind - 1, lvl + 1);
            else if ((y.back().first.empty() && y.back().second) || (!y.back().first.empty() && y.back().first[0].first != -1 && y.back().second)) {
                if (lvl == 1) {
                    y.push_back({});
                    auto x = smart_search(list, lower, upper, y, i - 1, lvl);
                    if (x.back().first.empty())
                        return x;
                }
                return y;
            }
        }
    return  { {{{-1,-1}},false} };
}

bool getHs(map<string, int>& s_B, int& h_o, vector<int>& h_in, string& s_o, string& s_in, map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>& A) {
    for (const auto& it : A)
        for (const auto& it2 : it.second) {
            h_in = vector<int>();
            int children = it2.first.first.second.second;
            auto dis = data_data[it2.second].getNearDis(it2.first.first.second.first);
            if (dis.empty()) continue;
            int best = 0;
            for (const auto& it4 : dis) {
                vector<pair<int, int>>list;
                for (const auto& it3 : A[it4])
                    if (data_data[it3.second].getDisToSch(it.first) < it3.first.first.second.first)
                        list.push_back({ it3.first.first.second.second,it3.second });
                int lower = s_B[it4] - (28 * const_count[it4]) + children;
                int upper = (28 * const_count[it.first]) - s_B[it.first] + children;
                vector<int>tmp2;
                if (list.empty()) continue;
                sort(list.begin(), list.end(), [](auto& l, auto& r)->bool {return l.first < r.first; });
                auto ans = smart_search(list, lower, upper, vector<pair<vector<pair<int, int>>, bool>>(), list.size() - 1, 1);
                /*if (ans.) {
                    int newb = abs(data_data[it2.second].getDisToSch(it4) - it2.first.first.second.first) * children;
                    for (const auto& it5 : ans.first) newb += (abs(data_data[it5.second].getDisToSch(it4) - data_data[it5.second].getDisToSch(it.first)) * data_data[it5.second].getChildren());
                    if (newb > best) {
                        for (auto it : ans.first) tmp2.push_back(it.second);
                        h_o = it2.second;
                        h_in = tmp2;
                        s_o = it.first;
                        s_in = it4;
                    }
                }*/
            }
            if (!h_in.empty()) return false;
        }
    return true;
}

pair<bool, pair<map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>>, map<string, int>>> solution_2(map<string, pair<vector<pair<string, pair<int, int>>>, int>>& ans) {
    map<const string, vector<pair<pair<pair<string, pair<int, int>>, vector<string>>, int>>> A, B;
    map<string, int> s_A, s_B;//сумма детей в каждой школе
    for (const auto& it : ans) {
        s_A[it.first] = it.second.second;
        for (const auto& it2 : it.second.first)
            A[it.first].push_back({ {{{it2.first},{data_data[it2.second.second].getDisToSch(it.first),it2.second.first}},vector<string>(0)},it2.second.second });
    }

    auto t = calcEf(A, s_A);

    B = A;
    s_B = s_A;

    int h_o, count_iteration = 0;
    vector<int>h_in;
    string s_o, s_in;
    while (!getHs(s_B, h_o, h_in, s_o, s_in, B)) {
        int tmp;
        for (int i = 0; i < B[s_o].size(); ++i) if (B[s_o][i].second == h_o) { tmp = i; break; }
        int* ptr = &(B[s_o][tmp].first.first.second.second);
        s_B[s_o] -= *ptr;
        s_B[s_in] += *ptr;
        B[s_o][tmp].first.first.second.first = data_data[B[s_o][tmp].second].getDisToSch(s_in);
        B[s_in].push_back(B[s_o][tmp]);
        B[s_o].erase(B[s_o].begin() + tmp);
        for (const auto& it : h_in) {
            for (int i = 0; i < B[s_in].size(); ++i) if (B[s_in][i].second == it) { tmp = i; break; }
            int* ptr = &(B[s_in][tmp].first.first.second.second);
            s_B[s_in] -= *ptr;
            s_B[s_o] += *ptr;
            B[s_in][tmp].first.first.second.first = data_data[B[s_in][tmp].second].getDisToSch(s_o);
            B[s_o].push_back(B[s_in][tmp]);
            B[s_in].erase(B[s_in].begin() + tmp);
        }
        ++count_iteration;
    }
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

//void sorting(const bool& flag, const pair<float, float>& school_m, const pair<float, float>& school_M, map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& map, vector<pair<House, int>>& houses) {
//    sort(map[school_M].begin(), map[school_M].end(),
//        [&school_M, &school_m, &houses, &flag]
//    (pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>& l, pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>& r)->bool
//        {
//            int id_1 = l.second, id_2 = r.second;
//            float dif_1 = 0, dif_2 = 0;
//            if (flag)
//                dif_1 = houses[id_1].first.getDifference_r(school_m, school_M), dif_2 = houses[id_2].first.getDifference_r(school_m, school_M);
//            else
//                dif_1 = houses[id_1].first.getDifference(school_m, school_M), dif_2 = houses[id_2].first.getDifference(school_m, school_M);
//            return dif_1 > dif_2;
//        });
//}
// 
//void getMin_Max(const map<pair<float, float>, int>& s_B, pair<pair<float, float>, int>& min_S, pair<pair<float, float>, int>& max_S, map<pair<float, float>, int>& count_classes) {
//    pair<pair<float, float>, int> min, min_2, max;
//    min.second = INT_MIN;
//    max.second = INT_MIN;
//    min_2.second = INT_MAX;
//    for (const auto& it : s_B) {
//        if (it.second - count_classes[it.first] * 28 > max.second)
//            max = { it.first,  it.second - count_classes[it.first] * 28 };
//        if (count_classes[it.first] * 25 - it.second > min.second)
//            min = { it.first,  count_classes[it.first] * 25 - it.second };
//        if (it.second - count_classes[it.first] * 25 < min_2.second)
//            min_2 = { it.first,  it.second - count_classes[it.first] * 25 };
//        else if (it.second - count_classes[it.first] * 25 == min_2.second) {
//            if (float(it.second - count_classes[it.first] * 25) / (count_classes[it.first] * 28 - count_classes[it.first] * 25) < (float(min_2.second) / (count_classes[min_2.first] * 28 - count_classes[min_2.first] * 25)))
//                min_2 = { it.first,  it.second - count_classes[it.first] * 25 };
//        }
//    }
//    if (min.second > 0)
//        min_S = min;// min
//    else
//        min_S = min_2;
//    max_S = max;// max
//}
//
//pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>> solution_2(const bool& flag, map<pair<float, float>, pair<int, float>>& ef, vector<pair<House, int>>& houses) {
// true - ratio_dis; false - dis   
// map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>> A, B;
//    //массив: школа -> дом -> характеристики, B_2 - отсортированный по коэф
//    map<pair<float, float>, int> s_A, s_B, count_class;//сумма детей в каждой школе, кол-во классов
//    int s = 0;
//    for (auto house : houses) {
//        auto tmp = house.first.getMin_dis();//минимальное расстояние
//        auto t = vector<pair<float, float>>(0);
//        A[tmp.first].push_back({ {{ house.first.getCord(),{tmp.second,house.first.getChildren()} },t}, house.first.getID() });
//        //распределение домов к школе с максимальным коэф
//        s_A[tmp.first] += house.first.getChildren();//подсчет суммы детей в школе
//        count_class[tmp.first] = 0;
//        s += house.first.getChildren();
//    }
// 
//    auto count = calcCountClasses(s);
//    setClasses(false, count, count_class);//true - перераспределение; false - копирование
//
//    auto EF = calcEf(A, s_A);
//    ef = EF;
//
//    B = A;
//    s_B = s_A;
//
//    pair<pair<float, float>, int>min_S, max_S;
//
//    int count_iteration = 0;
//
//    for (;;) {
//        getMin_Max(s_B, min_S, max_S, const_count);
//        if (max_S.second == INT_MIN) break;
//        sorting(flag, min_S.first, max_S.first, B, houses);
//        auto it = --B[max_S.first].end();
//        int* ptr = &((*it).first.first.second.second);
//        if ((min(abs(s_B[min_S.first] - 25 * const_count[min_S.first]), abs(s_B[min_S.first] - 28 * const_count[min_S.first])) + min(abs(s_B[max_S.first] - 25 * const_count[max_S.first]), abs(s_B[max_S.first] - 28 * const_count[max_S.first]))) < (min(abs(s_B[min_S.first] + *ptr - 25 * const_count[min_S.first]), abs(s_B[min_S.first] + *ptr - 28 * const_count[min_S.first])) + min(abs(s_B[max_S.first] - *ptr - 25 * const_count[max_S.first]), abs(s_B[max_S.first] - *ptr - 28 * const_count[max_S.first])))) break;
//        it->first.first.second.first = houses[it->second].first.getDisToSch(min_S.first);
//        s_B[max_S.first] -= *ptr;
//        s_B[min_S.first] += *ptr;
//        B[min_S.first].push_back(*it);
//        B[max_S.first].pop_back();
//        ++count_iteration;
//    }
//    return { B,s_B };
//}
