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

int layers = 4, moves;
int dim[4] = {1600, 128, 64, 10};
vector< pair<Matrix, Matrix> > param_layer;

int main(int argc, char** argv)
{
    if (argc != 2) return 0;
    ios_base::sync_with_stdio(0); cin.tie(0);
    // get_params
    freopen("jupyter/params.txt", "r", stdin);
    for (int l = 1; l < layers; ++l)
    {
        Matrix w(dim[l - 1], dim[l]), b(1, dim[l]);
        for (int i = 0; i < dim[l - 1]; ++i)
            for (int j = 0; j < dim[l]; ++j)
                cin >> w.vals[i][j];
        for (int j = 0; j < dim[l]; ++j) cin >> b.vals[0][j];
        param_layer.push_back(make_pair(w, b));
    }
    fclose(stdin);
    // query
    int id = atoi(argv[1]);
    vector<int> wrong_ans;
    for (int x = 0; x < 31; ++x)
        for (int y = 0; y < 31; ++y)
        {
            ostringstream ss; ss << setw(5) << setfill('0') << id;
            ifstream f_data("test/" + ss.str() + ".inp"); ifstream f_label("test/" + ss.str() + ".ans");
            moves = layers - 1;
            Matrix img(1, dim[0]);
            for (int i = 0; i < 40; ++i)
            {
                string s; f_data >> s;
                for (int j = 0; j < 40; ++j) img.vals[0][i * 40 + j] = s[j] - '0';
            }
            int answer; f_label >> answer;
            for (int dx = 0; dx < 10; ++dx)
                for (int dy = 0; dy < 10; ++dy)
                    img.vals[0][(x + dx) * 40 + y + dy] = 0;
            // for (int x = 0; x < 40; ++x)
            //     for (int y = 0; y < 40; ++y)
            //         cerr << img.vals[0][x * 40 + y] << " \n"[y == 39];
            for (int i = 0; i < moves - 1; ++i)
            {
                img = forward_pass(img, param_layer[i].first, param_layer[i].second);
                // dropout(img, 0.5l);
            }
            img = final_layer(img, param_layer[moves - 1].first, param_layer[moves - 1].second);
            int output = 0;
            for (int d = 1; d < 10; ++d) 
                if (img.vals[0][d] > img.vals[0][output]) output = d;
            if (answer == output) continue;
            // cout << "\033[0m";
            // for (int x = 0; x < 40; ++x)
            // {
            //     for (int y = 0; y < 40; ++y)
            //         cout << img.vals[0][x * 40 + y];
            //     if (y == 39) cout << "\n";
            // }
            cout << "\033[36m";
            for (int d = 0; d < 10; ++d) cout << fixed << setprecision(9) << d << " | " << img.vals[0][d] * 100.0l << endl;
            cout << "\033[31m" << "Hide " << x << "-" << y << " | Pred = " << output << ", Answer = " << answer << " | " << "FAILURE" << endl;
            f_data.close(); f_label.close();
        }
    cout << "\033[0m";
}