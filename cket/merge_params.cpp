#include <bits/stdc++.h>
using namespace std;
int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    ifstream code("play_template.cpp"), params("../jupyter/text.txt");
    ofstream code_new("play.cpp");
    string s;
    while (getline(code, s))
    {
        code_new << s << "\n";
        if (s == "vector< pair<Matrix, Matrix> > param_layer;") 
        {
            code_new << "\n";
            string t;
            while (getline(params, t))
            {
                code_new << t << "\n";
            }
            code_new << "\n";
        }
    }
    code.close();
    code_new.close();
}