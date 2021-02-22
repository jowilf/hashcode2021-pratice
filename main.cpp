#include "main.h"

#define MAX_M 100001
#define MAX_I
#define N 501
#define A_IN "/Users/mac/CLionProjects/hashcode2021-pratice/inputs/a.in"
#define A_OUT "/Users/mac/CLionProjects/hashcode2021-pratice/outputs/a.out"
#define B_IN "/Users/mac/CLionProjects/hashcode2021-pratice/inputs/b.in"

struct PizzaScore {
    int p1, p2, s;

    PizzaScore(int p1, int p2, int s) : p1(p1), p2(p2), s(s) {}
};

int M, t2, t3, t4;
map<int, vector<string>> pizzas;
map <string, vector<int>> ingredientPizza;
int pizzaTeam[MAX_M];
int pizzaScores[N][N];
vector <PizzaScore> allPizzaScore;
vector<int> team2;
vector<int> team3;
vector<int> team4;

void addIngredientPizza(string ingredient, int pizza) {
    vector<int> v;
    if (ingredientPizza.find(ingredient) != ingredientPizza.end())
        v = ingredientPizza[ingredient];
    v.push_back(pizza);
    ingredientPizza[ingredient] = v;
}

void readInput() {
    freopen(B_IN, "r", stdin);
    cin >> M >> t2 >> t3 >> t4;
    for (int i = 0; i < M; ++i) {
        int k;
        string ingredient;
        vector <string> ingredients;
        cin >> k;
        for (int j = 0; j < k; ++j) {
            cin >> ingredient;
            ingredients.push_back(ingredient);
            addIngredientPizza(ingredient, i);
        }
        sort(ingredient.begin(), ingredient.end());
        pizzas.insert({i, ingredients});
    }
}

int intersection(vector <string> &nums1, vector <string> &nums2) {
    unordered_set <string> m(nums1.begin(), nums1.end());
    vector <string> res;
    for (auto a : nums2)
        if (m.count(a)) {
            res.push_back(a);
            m.erase(a);
        }
    return res.size();
}

int getScore(int p1, int p2) {
    int i = pizzas[p1].size() > pizzas[p2].size() ? intersection(pizzas[p1], pizzas[p2]) : intersection(pizzas[p2],
                                                                                                        pizzas[p1]);
    return pizzas[p1].size() + pizzas[p2].size() - i;
}

int getScore(int p1, int p2, int p3) {
    set <string> st;
    for (string x:pizzas[p1])
        st.insert(x);
    for (string x:pizzas[p2])
        st.insert(x);
    for (string x:pizzas[p3])
        st.insert(x);
    return st.size();
}

void computeScoreArray() {
    for (int i = 0; i < M - 1; ++i) {
        for (int j = i + 1; j < M; ++j) {
            int s = getScore(i, j);
            pizzaScores[i][j] = s;
            pizzaScores[j][i] = s;
            allPizzaScore.push_back(PizzaScore(i, j, s));
        }
    }
}

void pick(int p, int team) {
    pizzaTeam[p] = team;
    if (team == 2)
        team2.push_back(p);
    else if (team == 3)
        team3.push_back(p);
}

int findP3(int p1, int p2) {
    int p = -1, ms = -1;
    for (int i = 0; i < M; ++i) {
        if (pizzaTeam[i] == -1) {
            int s = getScore(p1, p2, i);
            if (s > ms)
                p = i;
        }
    }
    return p;
}

void solve() {
    sort(allPizzaScore.begin(), allPizzaScore.end(), [](PizzaScore p1, PizzaScore p2) { return p1.s < p2.s; });
    int i = allPizzaScore.size() - 1;
    PizzaScore pizzaScore = allPizzaScore[i];
    //t3
    while (t3 > 0 && i > -1) {
        if (pizzaTeam[pizzaScore.p1] == -1 && pizzaTeam[pizzaScore.p2] == -1) {
            int p3 = findP3(pizzaScore.p1, pizzaScore.p2);
            if (p3 != -1) {
                pick(pizzaScore.p1, 3);
                pick(pizzaScore.p2, 3);
                pick(p3, 3);
                t3--;
            }
        }
        i -= 1;
        pizzaScore = allPizzaScore[i];
    }

    //t2
    i = allPizzaScore.size() - 1;
    while (t2 > 0 && i > -1) {
        //        printf("i: %d\n",i);
        if (pizzaTeam[pizzaScore.p1] == -1 && pizzaTeam[pizzaScore.p2] == -1) {
            pick(pizzaScore.p1, 2);
            pick(pizzaScore.p2, 2);
            t2--;
        }
        i -= 1;
        pizzaScore = allPizzaScore[i];
    }


}

void output() {
    freopen(A_OUT, "w", stdout);
    double s = 0;
    int d = (int) (team2.size() / 2 + team3.size() / 3);
    printf("%d\n", d);
    for (int i = 0; i < team2.size() / 2; ++i) {
        printf("%d %d %d\n", 2, team2[i * 2], team2[i * 2 + 1]);
        s += pow(getScore(team2[i * 2], team2[i * 2 + 1]), 2);
    }
    for (int i = 0; i < team3.size() / 3; ++i) {
        printf("%d %d %d %d\n", 3, team3[i * 3], team3[i * 3 + 1], team3[i * 3 + 2]);
        s += pow(getScore(team3[i * 3], team3[i * 3 + 1], team3[i * 3 + 2]), 2);
    }
    printf("score: %f\n", s);
}

int main() {
    memset(pizzaTeam, -1, sizeof pizzaTeam);
    readInput();
    computeScoreArray();
    solve();
    output();


}
