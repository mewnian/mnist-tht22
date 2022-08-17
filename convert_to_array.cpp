#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;
typedef long double ld;

const int N_TEST = 10000;
const int DIM = 8;

int cnt[10];
ld avg[10][DIM][DIM];

void compress() 
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    ifstream inp("avg_cnt.txt"); ofstream _("avg_arr.txt");
    for (int d = 0; d <= 9; ++d) 
    {
        for (int i = 0; i < DIM; ++i)
            for (int j = 0; j < DIM; ++j)
                inp >> avg[d][i][j];
    }
    _ << "{";
    for (int d = 0; d < 10; ++d)
    {
        _ << "{";
        for (int i = 0; i < DIM; ++i)
        {
            _ << "{";
            for (int j = 0; j < DIM; ++j)
            {
                _ << fixed << setprecision(9) << avg[d][i][j] << ",}"[j == 7];
            }
            _ << ",}"[i == 7];
        }
        _ << ",}"[d == 9];
    }
    inp.close();
    _.flush(); _.close();
}

int main(int argc, char** argv)
{
    compress();
}