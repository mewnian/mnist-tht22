#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;
typedef long double ld;

const int N_TEST = 200;
const int DIM = 40;

int cnt[10];
ld avg[10][DIM][DIM];

void compress() 
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    // ofstream flabel("label_test.txt"), fdata("data_test.txt");
    vector<int> labels;
    memset(cnt, 0, sizeof(cnt));
    for (int i = 0; i < N_TEST; ++i)
    {
        if (i && i % min(N_TEST / 100, 100) == 0) cerr << i << "/" << N_TEST << endl;
        string dir_ans, dir_inp;
        if (i < 60000) 
        {
            ostringstream ss;
            ss << setw(5) << setfill('0') << i;
            dir_ans = "test/" + ss.str() + ".ans", dir_inp = "test/" + ss.str() + ".inp";
        }
        else
        {
            ostringstream ss;
            ss << setw(5) << setfill('0') << i - 10'000;
            dir_ans = "train/" + ss.str() + ".ans", dir_inp = "train/" + ss.str() + ".inp";
        }
        ifstream in_label(dir_ans.c_str()), in_data(dir_inp.c_str());
        int ans; 
        in_label >> ans; 
        // flabel << ans << "\n"; 
        ++cnt[ans];
        cerr << dir_inp << endl;
        for (int line = 0; line < DIM; ++line)
        {
            string s;
            in_data >> s;
            cerr << (int)s.size() << " ";
            for(int col = 0; col < DIM; ++col)
                avg[ans][line][col] += s[col] - '0';
        }
        cerr << endl;
        // fdata << "\n";
        in_label.close(); in_data.close();
    }
    // flabel.flush(); fdata.flush();
    // flabel.close(); fdata.close();
    ofstream _("avg_cnt.txt");
    for (int d = 0; d <= 9; ++d) 
    {
        for (int i = 0; i < DIM; ++i)
            for (int j = 0; j < DIM; ++j)
                avg[d][i][j] /= cnt[d];
        // _ << cnt[d] << " ";
    }
    for (int d = 0; d < 9; ++d)
    {
        for (int i = 0; i < DIM; ++i)
        {
            for (int j = 0; j < DIM; ++j)
                _ << fixed << setprecision(6) << avg[d][i][j] << " ";
            cout << "\n";
        }
        cout << "\n";
    }
    _.flush(); _.close();
}

int main(int argc, char** argv)
{
    compress();
}