#include <bits/stdc++.h>
#define pb push_back
#define fi first
#define se second

using namespace std;
typedef pair<int, int> pi;

const int MAXN = 1003;

int n, k, a[MAXN][MAXN], visited[MAXN][MAXN];
// int pa[1200000], res;
vector<pi> one;

void dfs(int x, int y)
{
    visited[x][y] = 1;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    for (int d = 0; d < 4; ++d)
    {
        int tx = x + dx[d], ty = y + dy[d];
        if (tx < 0 || tx >= n || ty < 0 || ty >= n || visited[tx][ty] || a[tx][ty] == 1) continue;
        dfs(tx, ty);
    }
}

int sub1()
{
    set<int> rows, cols;
    int mx_r = -1, mx_c = -1;
    for (auto& v : one) 
    {
        mx_r = max(mx_r, v.fi);
        mx_c = max(mx_c, v.se);
        rows.insert(v.fi);
        cols.insert(v.se);
    }
    vector<int> _r, _c;
    rows.insert(n); cols.insert(n);
    int cur = 0;
    for (auto& v : rows)
    {
        if (v - cur > 1) _r.pb(cur + 1);
        _r.pb(v);
    }
    for (auto& v : cols)
    {
        if (v - cur > 1) _c.pb(cur + 1);
        _c.pb(v);
    }
    if (n - mx_r > 1) _r.pb(mx_r + 1);
    if (n - mx_c > 1) _c.pb(mx_c + 1);
    for (auto& v : one)
    {

    }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            a[i][j] = visited[i][j] = 0;
    for (auto& v : one) a[v.fi][v.se] = 1;
    int zones = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (!a[i][j] && !visited[i][j]) 
            {
                ++zones;
                dfs(i, j);
            }
    return zones;
}

// set<int> s;
// set<pi> S;
// vector<pi> interval_col; 
// vector<pi> interval_row; 


// bool valid(int x, int y) 
// { 
//     return (x >= 0 && x < n) && (y >= 0 && y < n); 
// }

// bool is_one(int x, int y) 
// {
//     pi T = *lower_bound(one.begin(), one.end(), pi(x, y));
//     return T == pi(x, y); 
// }

// bool cmp(pi X, pi Y) 
// {
//     return X.se < Y.se; 
// }

// int root(int x) 
// {
//     return pa[x] == x ? x : pa[x] = root(pa[x]); 
// }

// void connect(int x, int y) 
// {
//     x = root(x), y = root(y);
//     if (x == y) return;
//     if (x > y) swap(x, y);
//     pa[x] = y;
//     --res;
// }

// void connect() 
// {
//     vector<pi> SV(S.begin(), S.end());
//     int N = S.size() + interval_col.size() + interval_row.size();
//     for (int i = 0; i < N; ++i) pa[i] = i + 1; 
//     for (int i = 0; i < S.size(); ++i) pa[i] = i; 
//     for (int i = S.size(); i < S.size() + interval_col.size(); ++i) pa[i] = S.size(); 
//     res = S.size() + 1; 

//     // Check in special interval 
//     for (int i = 0; i < SV.size(); ++i) 
//     { 
//         pi inter_row = *lower_bound(interval_row.begin(), interval_row.end(), pi(SV[i].fi, SV[i].fi));
//         pi inter_col = *lower_bound(interval_col.begin(), interval_col.end(), pi(SV[i].se, SV[i].se));
//         if ((inter_row.fi <= SV[i].fi && SV[i].fi <= inter_row.se) || (inter_col.fi <= SV[i].se && SV[i].se <= inter_col.se))
//             pa[i] = S.size(), --res;
//     }

//     const int dx[4] = {1, -1, 0, 0};
//     const int dy[4] = {0, 0, 1, -1};
//     // Check next_to each other 
//     for (int i = 0; i < SV.size(); ++i) 
//     {
//         for (int j = 0; j < 4; ++j) 
//         {
//             int tx = SV[i].fi + dx[j], ty = SV[i].se + dy[j];
//             int k = lower_bound(SV.begin(), SV.end(), pi(tx, ty)) - SV.begin();
//             if (SV[k] == pi(tx, ty)) connect(i, k); 
//         }    
//     }
// }

// int ac()
// {
//     const int dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
//     const int dy[] = {1, 1, 1, 0, -1, -1, -1, 0};

//     for (int i = 0; i < n; ++i) 
//     { 
//         for (int j = 0; j < 8; ++j) 
//         { 
//             int x = one[i].fi + dx[j], y = one[i].se + dy[j];
//             if (valid(x, y) && !is_one(x, y)) S.insert({x, y}); 
//         }
//     }

//     if (n <= k) 
//     {
//         for (int i = 0; i < n; ++i) 
//         {
//             int x = 0, y = i; 
//             if (valid(x, y) && !is_one(x, y)) S.insert({x, y}); 
//             if (valid(y, x) && !is_one(y, x)) S.insert({y, x}); 
//             if (valid(n - 1 - x, y) && !is_one(n - 1 - x, y)) S.insert({n - 1 - x, y});
//             if (valid(n - 1 - y, x) && !is_one(n - 1 - y, x)) S.insert({n - 1 - y, x});
//         }
//     }
//     else 
//     {
//         for (int i = 0; i < n; ++i) {
//             int L = one[i].fi + 1, R = one[i+1].fi - 1; 
//             if (L <= R) interval_row.pb({L, R}); 
//         }
//         sort(one.begin(), one.end(), cmp); 
//         for (int i = 0; i < n; ++i) {
//             int L = one[i].se + 1, R = one[i+1].se - 1; 
//             if (L <= R) interval_col.pb({L, R}); 
//         }
//     }

//     connect();
//     return res;  
// }

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    cin >> n >> k;
    for (int i = 0; i < k; ++i)
    {
        int x, y;
        cin >> x >> y;
        one.push_back({x, y});
    }
    one.push_back({-1, -1});
    one.push_back({n, n}); 
    sort(one.begin(), one.end());

    cout << ac();
    // cerr << "success";
    // if (n <= 1000) cout << sub1();
    // else cout << "1";
}