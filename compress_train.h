void compress() 
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    ofstream flabel("label.txt"), fdata("data.txt");
    vector<int> labels;
    for (int i = 0; i < N_TRAIN; ++i)
    {
        if (i && i % min(N_TRAIN / 100, 100) == 0) cerr << i << "/" << N_TRAIN << endl;
        ostringstream ss;
        ss << setw(5) << setfill('0') << i;
        string dir_ans = "train/" + ss.str() + ".ans", dir_inp = "train/" + ss.str() + ".inp";
        ifstream in_label(dir_ans.c_str()), in_data(dir_inp.c_str());
        int ans; in_label >> ans; flabel << ans << "\n"; ++cnt[ans];
        for (int line = 0; line < DIM; ++line)
        {
            string s;
            in_data >> s;
            bitset<40> b(s);
            fdata << b.to_ullong() << " ";
        }
        fdata << "\n";
        in_label.close(); in_data.close();
    }
    flabel.flush(); fdata.flush();
    flabel.close(); fdata.close();
    ofstream _("cnt.txt");
    for (int d = 0; d <= 9; ++d) _ << cnt[d] << " ";
    _.flush(); _.close();
}