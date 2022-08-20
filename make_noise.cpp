#include <bits/stdc++.h>

using namespace std;

vector<string> s(40);

int main(int argc, char** argv)
{
    if (argc != 4) return 0;
    ios_base::sync_with_stdio(0); cin.tie(0);
    int id = atoi(argv[1]), x = atoi(argv[2]), y = atoi(argv[3]);
    ostringstream ss;
    ss << setw(5) << setfill('0') << id;
    string dir = "test/" + ss.str() + ".inp";
    ifstream f_data(dir.c_str());
    for (int i = 0; i < 40; ++i) 
    {
        f_data >> s[i];
        // cerr << s[i].size();
    }
    for (int dx = 0; dx < 10; ++dx)
        for (int dy = 0; dy < 10; ++dy)
            s[x + dx][y + dy] = '0';
    ofstream noised("noised.txt");
    for (int i = 0; i < 40; ++i) noised << s[i] << "\n";
    noised.flush();
    f_data.close(); noised.close();
}