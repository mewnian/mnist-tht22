#include <bits/stdc++.h>

using namespace std;
typedef long double ld;

const int SEEDS = 20220820;
// mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng(SEEDS);

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

int layers = 3, moves;
int dim[3] = {1600, 256, 10};
vector< pair<Matrix, Matrix> > param_layer;

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    // get_params
    ifstream f("params.inp");
    for (int l = 1; l < layers; ++l)
    {
        Matrix w(dim[l - 1], dim[l]), b(1, dim[l]);
        for (int i = 0; i < dim[l - 1]; ++i)
            for (int j = 0; j < dim[l]; ++j)
                f >> w.vals[i][j];
        for (int j = 0; j < dim[l]; ++j) f >> b.vals[0][j];
        param_layer.push_back(make_pair(w, b));
    }
    f.close();
    // query
    vector<int> wrong_ans;

    moves = layers - 1;
    Matrix img(1, dim[0]);
    for (int i = 0; i < 40; ++i)
    {
        string s; cin >> s;
        for (int j = 0; j < 40; ++j) img.vals[0][i * 40 + j] = s[j] - '0';
    }
    for (int i = 0; i < moves - 1; ++i)
    {
        img = forward_pass(img, param_layer[i].first, param_layer[i].second);
        dropout(img, 0.5l);
    }
    img = final_layer(img, param_layer[moves - 1].first, param_layer[moves - 1].second);
    int output = 0;
    for (int d = 1; d < 10; ++d) 
        if (img.vals[0][d] > img.vals[0][output]) output = d;
    cout << output;
}
