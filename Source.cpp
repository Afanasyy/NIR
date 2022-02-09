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
#define EARTH_RADIUS 6372795 
#define COUNT_CHILDREN 20
using namespace std;

map<pair<float, float>, int>const_count;
bool fl;

class House {
public:
    House(const int& children, const map<pair<float, float>, int>& dis, const pair<float, float> cord, const map<pair<float, float>, float>& ratio_dis, const int& id) {
        this->children = children;
        this->dis = dis;
        this->cord = cord;
        this->ratio_dis = ratio_dis;
        this->id = id;
    }
    pair<float, float> getCord() {
        return cord;
    }
    int getChildren() {
        return children;
    }
    map<pair<float, float>, int> getDis() {
        return dis;
    }
    map<pair<float, float>, float> getRatio_Dis() {
        return ratio_dis;
    }
    pair<pair<float, float>, float> getMin_dis() {
        pair<pair<float, float>, float> m;
        m.second = INT_MAX;
        for (auto i : dis)
            if (i.second < m.second) m = i;
        return m;
    }
    float getDifference(const pair<float, float>& school_m, const pair<float, float>& school_M) {
        return abs(dis[school_m] - dis[school_M]);
    }
    float getDifference_r(const pair<float, float>& school_m, const pair<float, float>& school_M) {
        return abs(ratio_dis[school_m] - ratio_dis[school_M]);
    }
    int getID() {
        return id;
    }
    int getDisToSch(const pair<float, float>& sch) {
        for (auto it : dis) {
            if (it.first == sch) return it.second;
        }
    }
    vector<pair<float, float>>getNearDis(const int& cur) {
        vector<pair<float, float>>ans;
        vector<pair<pair<float, float>, int>>tmp;
        for (const auto& it : dis) tmp.push_back(it);
        sort(tmp.begin(), tmp.end(), [](auto& l, auto& r)->bool {return l.second > r.second; });
        for (const auto& it : tmp) if (it.second < cur) ans.push_back(it.first);
        return ans;
    }
    float getRatioDisToSch(const pair<float, float>& sch) {
        for (auto it : ratio_dis) {
            if (it.first == sch) return it.second;
        }
    }
private:
    map<pair<float, float>, int>dis;
    int children;
    pair<float, float>cord;
    map<pair<float, float>, float>ratio_dis;
    int id;
};

auto calcEf(const map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>&, const map<pair<float, float>, int>&) ->pair<int, float>;
auto calcCountClasses(const int&)->int;
auto setClasses(const bool&, const int&, map<pair<float, float>, int>)->void;
auto foo(vector<pair<float, float>>&, const pair<float, float>&)->bool;
auto getH(map<pair<float, float>, int>&, pair<int, pair<float, float>>&, pair<float, float>&, map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>&, vector<pair<House, int>>&)->bool;
auto smart_search(const vector<pair<int, int>>&, int, int, vector<pair<vector<pair<int, int>>, int>>, int, int)->vector<pair<vector<pair<int, int>>, int>>;
auto solution_1(pair<int, float>&, vector<pair<House, int>>&)->pair<bool, pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>>>;
auto getHs(map<pair<float, float>, int>&, int&, vector<int>& , pair<float, float>& , pair<float, float>& , map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>&, vector<pair<House, int>>&)->bool;
auto solution_1(map<pair<float, float>, pair<vector<pair<pair<float, float>, pair<int, int>>>, int>>&, vector<pair<House, int>>&)->pair<bool, pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>>>;
auto calculateTheDistance(const float&, const float&, const float&, const float&)->float;


int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    srand(time(0));


    vector<pair<int, int>>a = { {4, 7},{7, 14},{8, 6},{8, 3},{9, 11},{9, 8},{11, 4} };
    //for (int i = 0; i < 10; ++i) a.push_back({ rand() % COUNT_CHILDREN + 1,rand() });
    sort(a.begin(), a.end(), [](auto& l, auto& r)->bool {return l.first < r.first; });
    auto y = smart_search(a, 20, 21, vector<pair<vector<pair<int, int>>,int>>(), a.size() - 1, 1);


    bool flag = false; // true - ручной ввод; false - случайная генерация
    ifstream file("cord.txt");
    vector<pair<float, float>>cord_schools;
    vector<pair<pair<float, float>, int>>cord_houses;
    vector<pair<House, int>>houses;
    map<pair<float, float>, int>templ_classes;
    map<pair<float, float>, pair<vector<pair<pair<float, float>, pair<int, int>>>, int>>ans;
    while (!file.eof()) {//считывание из файла координат домов и школ
        string str;
        getline(file, str);
        size_t t = str.find(',');
        if (str[0] == '!') {
            auto tmp1 = stod(str.substr(1, t - 1));
            auto tmp2 = stod(str.substr(t + 2));
            cord_schools.push_back({ tmp1, tmp2 });
            templ_classes[{ tmp1, tmp2 }];
            ans[{tmp1, tmp2}];
        }
        else {
            size_t t2 = str.find(';');
            auto tmp1 = str.substr(0, t);
            auto tmp2 = str.substr(t + 2, t2 - t - 2);
            auto tmp3 = stoi(str.substr(t2 + 2));
            cord_houses.push_back({ { stod(tmp1),  stod(tmp2)}, tmp3 });
        }
    }

    vector<pair<pair<float, float>, pair<int, int>>>numbers;
    int sum = 0;
    int id = 0;
    for (auto cord_house : cord_houses) {
        map<pair<float, float>, float> ratio_dis;//массивы расстояний и коэф расстояний
        map<pair<float, float>, int> dis;
        int children;
        if (flag)
            children = cord_house.second;
        else
            children = rand() % COUNT_CHILDREN + 1;//случайная генерация кол-ва детей от 1 до 20
        sum += children;
        numbers.push_back({ {cord_house.first},{children,id} });
        for (auto cord_school : cord_schools) {//расчет расстояния от дома до каждой школы
            dis[cord_school] = (calculateTheDistance(cord_house.first.first, cord_house.first.second, cord_school.first, cord_school.second));
            ratio_dis[cord_school] = float(children) / dis[cord_school];//расчет коэф расстояния от дома до каждой школы
        }
        houses.push_back({ { children, dis, cord_house.first,ratio_dis, id}, id });//создание объекта одного 
        ++id;
    }


    auto count = calcCountClasses(sum);

    if (count == -1) {
        cout << "Невозможно распределить такое число детей по классам с учетом ограничений\n";
        return 0;
    }
    setClasses(true, count, templ_classes);

    sort(numbers.begin(), numbers.end(), [](auto& l, auto& r)->bool {return l.second.first < r.second.first; });

    vector<int>tmp;
    bool flag2;
    while (!numbers.empty()) {
        flag2 = false;
        for (const auto& it : cord_schools) {
            if (ans[it].second + numbers.back().second.first <= const_count[it] * 28) {
                ans[it].first.push_back(numbers.back());
                ans[it].second += numbers.back().second.first;
                numbers.pop_back();
                flag2 = true;
                break;
            }
        }
        if (!flag2) {
            tmp.push_back(numbers.back().second.first);
            numbers.pop_back();
        }
    }

    if (tmp.empty()) {
        auto sol_0 = solution_1(ans, houses);
        auto ef_0 = calcEf(sol_0.second.first, sol_0.second.second);
    }


    pair<int, float> x_ef1_first, x_ef2_first;

    fl = true;//без учета детей
    auto sol_1 = solution_1(x_ef1_first, houses);
    auto ef_1 = calcEf(sol_1.second.first, sol_1.second.second);

    fl = false;//с учетом детей
    auto sol_2 = solution_1(x_ef2_first, houses);
    auto ef_2 = calcEf(sol_2.second.first, sol_2.second.second);


    //auto sol_2 = solution_2(false, ef2_first, houses);//true - ratio_dis; false - dis 
    //auto ef_2 = calcEf(sol_2.first, sol_2.second);    
    return 0;
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

pair<int, float> calcEf(const map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& B, const map<pair<float, float>, int>& s_B) {
    pair<int, float>ef;
    for (auto it : B)
        for (auto it2 : it.second) {
            ef.first += it2.first.first.second.first * it2.first.first.second.second;
            ef.second += float(it2.first.first.second.second) / it2.first.first.second.first;
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

void setClasses(const bool& flag, const int& count_classes, map<pair<float, float>, int>templ_classes) {
    if (flag) {
        int min_c = floor(float(count_classes) / templ_classes.size());
        int remains = count_classes - min_c * templ_classes.size();//остаток
        for (auto& it : templ_classes) it.second += min_c;
        vector<int>mem;
        while (remains != 0) {
            int tmp = rand() % templ_classes.size();
            for (auto it : mem)
                if (it == tmp) continue;
            mem.push_back(tmp);
            auto it = templ_classes.begin();
            advance(it, tmp);
            it->second++;
            --remains;
        }
        for (auto it : templ_classes) const_count[it.first] = it.second;
    }
}

bool foo(vector<pair<float, float>>& v, const pair<float, float>& t) {
    for (auto it : v)
        if (it == t)
            return false;
    if (v.size() == 2) v.erase(v.begin());
    return true;
}

bool getH(map<pair<float, float>, int>& s_B, pair<int, pair<float, float>>& h, pair<float, float>& sch, map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& A, vector<pair<House, int>>& houses) {
    int c = 0;
    bool flag2 = false;
    for (auto it : s_B) {
        ++c;
        if (it.second > 28 * const_count[it.first]) {
            pair<pair<int, int>, pair<float, float>>min;
            min.first.second = INT_MAX;
            int count = 0;
            for (auto it2 : A[it.first]) {
                pair<pair<float, float>, int>m = { {0,0},INT_MAX };
                int current = it2.first.first.second.first;
                int children;
                if (fl)
                    children = 1;
                else
                    children = it2.first.first.second.second;
                auto dis = houses[it2.second].first.getDis();
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
            pair<pair<int, int>, pair<float, float>>min;
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
                        int new_dis = houses[it3.second].first.getDisToSch(it.first);
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

pair<bool, pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>>> solution_1(pair<int, float>& ef, vector<pair<House, int>>& houses) {
    map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>> A, B;
    //массив: школа -> дом -> характеристики, B_2 - отсортированный по коэф
    map<pair<float, float>, int> s_A, s_B, count_class;//сумма детей в каждой школе, кол-во классов
    int s = 0;
    for (auto house : houses) {
        pair<pair<float, float>, float> tmp = house.first.getMin_dis();//минимальное расстояние
        auto t = vector<pair<float, float>>(0);
        A[tmp.first].push_back({ {{ house.first.getCord(),{tmp.second,house.first.getChildren()} },t}, house.first.getID() });
        //распределение домов к школе с максимальным коэф
        s_A[tmp.first] += house.first.getChildren();//подсчет суммы детей в школе
        count_class[tmp.first] = 0;
        s += house.first.getChildren();
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
        pair<int, pair<float, float>>h;
        pair<float, float>sch;
        while (!(flag = getH(s_B, h, sch, B, houses))) {//поиск дома для перекидывания
            if (h.first == -1) {
                cout << "Кол-во классов увеличенно на 1\n";
                count++;
                flag2 = false;
                setClasses(true, count, count_class);//true - перераспределение; false - копирование
                break;
            }
            int* ptr = &(B[h.second][h.first].first.first.second.second);
            s_B[h.second] -= *ptr;
            s_B[sch] += *ptr;
            B[h.second][h.first].first.first.second.first = houses[B[h.second][h.first].second].first.getDisToSch(sch);
            B[h.second][h.first].first.second.push_back(h.second);
            B[sch].push_back(B[h.second][h.first]);
            B[h.second].erase(B[h.second].begin() + h.first);
            ++count_iteration;
        }
    }
    return { flag2, {B,s_B} };
}

vector<pair<vector<pair<int, int>>, int>> smart_search(const vector<pair<int, int>>& list, int lower, int upper, vector<pair<vector<pair<int, int>>, int>>last_list, int last_ind, int lvl) {
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
            auto y = smart_search(list, l, u, last_list, i - 1, lvl + 1);
            auto f = [&](auto& tmp, int&x, int&y)->auto {
                auto t = tmp.back();
                for (int j = t.first.size() - 1; j > lvl - 1; --j) {
                    t.first.erase(t.first.begin() + j, t.first.end());
                    t.second = -4;
                    auto z = smart_search(list, x, y, { t }, i - 1, lvl + 1);
                    for (auto& it : z)
                        if (it.second == -5) {
                            it.second = 2;
                            tmp.push_back(it);
                        }
                }
                return tmp;
            };
            if (y.back().second == -5)
                f(y, l, u);
            else if (y.back().second == -1 && !last_list.back().first.empty())
                f(last_list, l, u);
            else if (!y.back().first.empty() && y.back().second == -1)
                return smart_search(list, lower, upper, vector<pair<vector<pair<int, int>>, int>>(), last_ind - 1, lvl);
            else if ((y.back().first.empty() && y.back().second == -5) || (!y.back().first.empty() && (y.back().second == -5 || y.back().second == 2))) {
                if (lvl == 1) {
                    y.push_back({});
                    auto x = smart_search(list, lower, upper, y, i - 1, lvl);
                    if (x.back().first.empty())
                        return x;
                }
                return y;
            }
        }
    return  { {{{}},-1} };
}

bool getHs(map<pair<float, float>, int>& s_B, int& h_o, vector<int>& h_in, pair<float, float>& s_o, pair<float, float>& s_in, map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>& A, vector<pair<House, int>>& houses) {
    for (const auto& it : A)
        for (const auto& it2 : it.second) {
            h_in = vector<int>();
            int children = it2.first.first.second.second;
            auto dis = houses[it2.second].first.getNearDis(it2.first.first.second.first);
            if (dis.empty()) continue;
            int best = 0;
            for (const auto& it4 : dis) {
                vector<pair<int, int>>list;
                for (const auto& it3 : A[it4])
                    list.push_back({ it3.first.first.second.second,it3.second });
                int lower = s_B[it4] - (28 * const_count[it4]) + children;
                int upper = (28 * const_count[it.first]) - s_B[it.first] + children;
                if (list.empty()) continue;
                sort(list.begin(), list.end(), [](auto& l, auto& r)->bool {return l.first < r.first; });
                auto ans = smart_search(list, lower, upper, { {{},false} }, list.size() - 1, 1);
                if (ans[0].second) {
                    pair<int, int> max_ef = { INT_MIN,-1 };
                    for (int i = 0; i < ans.size() - 1; ++i) {
                        int cur_ef = 0;
                        for (const auto& it5 : ans[i].first) 
                            cur_ef += (houses[it5.second].first.getDisToSch(it4) - houses[it5.second].first.getDisToSch(it.first));
                        if (cur_ef > max_ef.first) max_ef = { cur_ef,i };
                    }
                        /*h_o = it2.second;
                        h_in = tmp2;
                        s_o = it.first;
                        s_in = it4;*/
                    
                }
            }
            if (!h_in.empty()) return false;
        }
    return true;
}

pair<bool, pair<map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>>, map<pair<float, float>, int>>> solution_1(map<pair<float, float>, pair<vector<pair<pair<float, float>, pair<int, int>>>, int>>& ans, vector<pair<House, int>>& houses) {
    map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>> A, B;
    map<pair<float, float>, int> s_A, s_B;//сумма детей в каждой школе
    for (const auto& it : ans) {
        s_A[it.first] = it.second.second;
        for (const auto& it2 : it.second.first)
            A[it.first].push_back({ {{{it2.first},{houses[it2.second.second].first.getDisToSch(it.first),it2.second.first}},vector<pair<float, float>>(0)},it2.second.second });
    }

    auto t = calcEf(A, s_A);

    B = A;
    s_B = s_A;

    int h_o, count_iteration = 0;
    vector<int>h_in;
    pair<float, float>s_o, s_in;
    while (!getHs(s_B, h_o, h_in, s_o, s_in, B, houses)) {
        int tmp;
        for (int i = 0; i < B[s_o].size(); ++i) if (B[s_o][i].second == h_o) { tmp = i; break; }
        int* ptr = &(B[s_o][tmp].first.first.second.second);
        s_B[s_o] -= *ptr;
        s_B[s_in] += *ptr;
        B[s_o][tmp].first.first.second.first = houses[B[s_o][tmp].second].first.getDisToSch(s_in);
        B[s_in].push_back(B[s_o][tmp]);
        B[s_o].erase(B[s_o].begin() + tmp);
        for (const auto& it : h_in) {
            for (int i = 0; i < B[s_in].size(); ++i) if (B[s_in][i].second == it) { tmp = i; break; }
            int* ptr = &(B[s_in][tmp].first.first.second.second);
            s_B[s_in] -= *ptr;
            s_B[s_o] += *ptr;
            B[s_in][tmp].first.first.second.first = houses[B[s_in][tmp].second].first.getDisToSch(s_o);
            B[s_o].push_back(B[s_in][tmp]);
            B[s_in].erase(B[s_in].begin() + tmp);
        }
        ++count_iteration;
    }
    return{ true,{B,s_B} };
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
//    map<const pair<float, float>, vector<pair<pair<pair<pair<float, float>, pair<int, int>>, vector<pair<float, float>>>, int>>> A, B;
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
