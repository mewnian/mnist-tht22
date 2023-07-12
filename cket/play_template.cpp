#include <bits/stdc++.h>
#define fi first
#define se second

using namespace std;
typedef long double ld;
typedef pair<int, int> pi;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

class Matrix 
{
    public: 
        int m, n;
        vector< vector<ld> > vals;
    public: 
        Matrix(int _m, int _n, bool rand = false): m(_m), n(_n)
        {
            vals.resize(m, vector<ld>(n, 0.0l));
            if (rand)
            {
                for (int i = 0; i < m; ++i)
                    for (int j = 0; j < n; ++j)
                        vals[i][j] = uniform_real_distribution<ld>(0.0l, 1.0l)(rng);
            }
        }

        Matrix(vector< vector<ld> > _vals)
        {
            m = vals.size(); n = vals[0].size();
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j)
                    vals[i][j] = _vals[i][j];
        }

        Matrix operator*(const ld& scalar)
        {
            Matrix prod(m, n);
            for (int x = 0; x < m; ++x)
                for (int y = 0; y < n; ++y)
                    prod.vals[x][y] = vals[x][y] * scalar;
            return prod;
        }

        Matrix operator*(const Matrix& rhs)
        {
            assert(n == rhs.m);
            Matrix prod(m, rhs.n);
            for (int x = 0; x < m; ++x)
                for (int y = 0; y < rhs.n; ++y)
                {
                    ld &sum = prod.vals[x][y];
                    sum = 0.0l;
                    for (int k = 0; k < n; ++k)
                        sum += vals[x][k] * rhs.vals[k][y];
                }
            return prod;
        }

        Matrix operator+(const ld& scalar)
        {
            Matrix sum(m, n);
            for (int x = 0; x < m; ++x)
                for (int y = 0; y < n; ++y)
                    sum.vals[x][y] = vals[x][y] + scalar;
            return sum;
        }

        Matrix operator+(const Matrix& rhs)
        {
            assert(m == rhs.m && n == rhs.n);
            Matrix sum(m, n);
            for (int x = 0; x < m; ++x)
                for (int y = 0; y < n; ++y)
                    sum.vals[x][y] = vals[x][y] + rhs.vals[x][y];
            return sum;
        }

        Matrix T()
        {
            Matrix mtr(n, m);
            for (int x = 0; x < m; ++x)
                for (int y = 0; y < n; ++y)
                    mtr.vals[x][y] = vals[y][x];
            return mtr;
        }
};

// sigmoid = 1 / (1 + exp(-z)
// relu = max(0, z)
// softmax = exp(z) / sum(exp(z))

Matrix sigmoid(const Matrix& z)
{
    Matrix res(z.m, z.n);
    for (int x = 0; x < z.m; ++x)
        for (int y = 0; y < z.n; ++y)
            res.vals[x][y] = 1.0l / (1.0l + exp(-z.vals[x][y]));
    return res;
}

Matrix relu(const Matrix& z)
{
    Matrix res(z.m, z.n);
    for (int x = 0; x < z.m; ++x)
        for (int y = 0; y < z.n; ++y)
            res.vals[x][y] = max(0.0l, z.vals[x][y]);
    return res;
}

Matrix softmax(const Matrix& z)
{
    Matrix res(z.m, z.n);
    for (int x = 0; x < z.m; ++x)
    {
        ld sum = 0.0l;
        for (int y = 0; y < z.n; ++y)
            sum += exp(z.vals[x][y]);
        for (int y = 0; y < z.n; ++y)
            res.vals[x][y] = exp(z.vals[x][y]) / sum;
    }
    return res;
}

Matrix forward_pass(Matrix& val, Matrix& w, Matrix& b)
{
    Matrix z = val * w + b;
    return relu(z);
}

void dropout(Matrix& val, ld keep_rate)
{
    for (int x = 0; x < val.m; ++x)
        for (int y = 0; y < val.n; ++y)
            if (uniform_real_distribution<ld>(0.0l, 1.0l)(rng) > keep_rate) val.vals[x][y] = 0.0l;
            else val.vals[x][y] /= keep_rate;
}

Matrix final_layer(Matrix& val, Matrix& w, Matrix& b)
{
    Matrix z = val * w + b;
    return softmax(z);
}

int layers = 4, moves = 3;
vector< pair<Matrix, Matrix> > param_layer;


pair<int,ld> predict(Matrix& img)
{
    for (int i = 0; i < moves - 1; ++i)
    {
        img = forward_pass(img, param_layer[i].first, param_layer[i].second);
        // dropout(img, 0.5l);
    }
    img = final_layer(img, param_layer[moves - 1].first, param_layer[moves - 1].second);
    int output = 0;
    for (int d = 1; d < 10; ++d) 
        if (img.vals[0][d] > img.vals[0][output]) output = d;
    return make_pair(output, img.vals[0][output]);
}

pair<ld,ld> predict_good(Matrix& img1, Matrix& img2)
{
    int answer = predict(img1).fi;
    for (int i = 0; i < moves - 1; ++i)
    {
        img2 = forward_pass(img2, param_layer[i].first, param_layer[i].second);
        // dropout(img, 0.5l);
    }
    img2 = final_layer(img2, param_layer[moves - 1].first, param_layer[moves - 1].second);
    int output = 0;
    for (int d = 1; d < 10; ++d) 
        if (img2.vals[0][d] > img2.vals[0][output]) output = d;
    return make_pair(img2.vals[0][answer] / img2.vals[0][output], img2.vals[0][output]);
}

int pixels[103][43][43];
int pref_sum[103][43][43];

int get_pixels(int id_img, int x, int y)
{
    int rx = min(40, x + 10), ry = min(40, y + 10);
    return pref_sum[id_img][rx][ry] - pref_sum[id_img][rx][y] - pref_sum[id_img][x][ry] + pref_sum[id_img][x][y];
}

ld prob_chosen(int x)
{
    return exp((ld)x / (ld)10.0l);
}

void hide_task()
{
    vector< tuple<int, int, int, int> > rs;
    int q; cin >> q;
    for (int _ = 0; _ < q; ++_)
    {
        for (int i = 0; i < 40; ++i)
        {
            for (int j = 0; j < 40; ++j)
            {
                int u; cin >> u;
                pixels[_][i][j] = u;
                pref_sum[_][i + 1][j + 1] = pref_sum[_][i + 1][j] + pref_sum[_][i][j + 1] - pref_sum[_][i][j] + u;
            }
        }
    }
    for (int _ = 0; _ < q; ++_)
    {
        for (int x = 0; x < 40; ++x)
            for (int y = 0; y < 40; ++y)
            {
                if (get_pixels(_,x,y) < 10) continue;
                rs.push_back({get_pixels(_,x,y), _, x, y});
            }
    }
    sort(rs.begin(), rs.end());
    vector<ld> range;
    ld sum = 0;
    for (int i = 0; i < rs.size(); ++i) sum += prob_chosen(get<0>(rs[i]));
    ld cur = 0.0l;
    for (int i = 0; i < rs.size(); ++i)
    {
        range.push_back(cur);
        cur += prob_chosen(get<0>(rs[i])) / sum;
    }
    pair<ld, ld> ratio = {1, 1};
    tuple<int, int, int> chose_tp = {-1, -1, -1};
    for (int ITER = 0; ITER < 200; ++ITER)
    {
        ld rnd = uniform_real_distribution<ld>(0,1)(rng);
        int cid = lower_bound(range.begin(), range.end(), rnd) - range.begin();
        Matrix _img1(1, 1600), _img2(1, 1600);
        for (int x = 0; x < 40; ++x)
            for (int y = 0; y < 40; ++y)
                _img1.vals[0][x * 40 + y] = _img2.vals[0][x * 40 + y] = pixels[get<1>(rs[cid])][x][y];
        for (int dx = 0; dx < 10; ++dx)
            for (int dy = 0; dy < 10; ++dy)
            {
                int tx = get<2>(rs[cid]) + dx, ty = get<3>(rs[cid]) + dy;
                if (tx > 40 || ty > 40) continue;
                _img2.vals[0][tx * 40 + ty] = 0.25l;
            }
        pair<ld, ld> res = predict_good(_img1, _img2);
        if (res < ratio)
        {
            ratio = res;
            chose_tp = {get<1>(rs[cid]), get<2>(rs[cid]), get<3>(rs[cid])};
        }
    }
    cout << get<0>(chose_tp) << " " << get<1>(chose_tp) << " " << get<2>(chose_tp) << endl;
}

void predict_task(ld confidence = 0.9)
{
    Matrix _img(1, 1600);
    for (int i = 0; i < 40; ++i)
    {
        for (int j = 0; j < 40; ++j)
        {
            int u; cin >> u;
            if (u == 0) _img.vals[0][i * 40 + j] = 0;
            else if (u == 1) _img.vals[0][i * 40 + j] = 1;
            else _img.vals[0][i * 40 + j] = 0.25l;
        }
    }
    pair<int, ld> res = predict(_img);
    cout << (res.se > confidence ? res.fi : -1);
}

Matrix img(1, 1600);
int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    
    for (int i = 0; i < w1.m; ++i)
        for (int j = 0; j < w1.n; ++j)
            w1.vals[i][j] = vw1[i][j];
    for (int i = 0; i < b1.m; ++i)
        for (int j = 0; j < b1.n; ++j)
            b1.vals[i][j] = vb1[i][j];
    for (int i = 0; i < w2.m; ++i)
        for (int j = 0; j < w2.n; ++j)
            w2.vals[i][j] = vw2[i][j];  
    for (int i = 0; i < b2.m; ++i)
        for (int j = 0; j < b2.n; ++j)
            b2.vals[i][j] = vb2[i][j];
    for (int i = 0; i < w3.m; ++i)
        for (int j = 0; j < w3.n; ++j)
            w3.vals[i][j] = vw3[i][j];
    for (int i = 0; i < b3.m; ++i)
        for (int j = 0; j < b3.n; ++j)
            b3.vals[i][j] = vb3[i][j];
    // get_params
    param_layer.push_back(make_pair(w1, b1));
    param_layer.push_back(make_pair(w2, b2));
    param_layer.push_back(make_pair(w3, b3));    

    int type; cin >> type;
    if (type == 1) hide_task();
    else predict_task();
}
