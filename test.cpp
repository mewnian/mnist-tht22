#include <bits/stdc++.h>

using namespace std;

vector< vector<int> > a { {1, 2}, { 3, 4} };

int main()
{
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            cout << a[i][j] << " ";
}