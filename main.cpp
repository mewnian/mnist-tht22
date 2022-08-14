#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;
typedef long double ld;

const int N_TRAIN = 60'000;
const int DIM = 40;
const int DIGITS = 10;
const int SEED = 0;

int cnt[DIGITS], label[N_TRAIN];
ull dataPoint[N_TRAIN][DIM], nTrain = 0, nTest = 0;
vector<int> digitID[DIGITS], trainID, testID;

mt19937_64 rng(SEED);

void loadData()
{
    ifstream flabel("label.txt"), fdata("data.txt");
    for (int i = 0; i < N_TRAIN; ++i)
    {
        flabel >> label[i];
        for (int j = 0; j < DIM; ++j) fdata >> dataPoint[i][j];
        digitID[label[i]].emplace_back(i);
    }
}

void splitData(ld ratio = 0.75)
{
    for (int digit = 0; digit <= 9; ++digit)
    {
        vector<int> shuffleID(digitID[digit].size());
        copy(digitID[digit].begin(), digitID[digit].end(), shuffleID.begin());
        // cerr << digit << " = " << shuffleID.size() << endl;
        shuffle(shuffleID.begin(), shuffleID.end(), rng);
        int dTrain = round(ratio * shuffleID.size());
        nTrain += dTrain;
        nTest += (int)shuffleID.size() - dTrain;
        for (int i = 0; i < dTrain; ++i) trainID.emplace_back(shuffleID[i]);
        for (int i = dTrain; i < (int)shuffleID.size(); ++i) testID.emplace_back(shuffleID[i]);
    }
    shuffle(trainID.begin(), trainID.end(), rng);
    shuffle(testID.begin(), testID.end(), rng);
}

int distanceRow(int __r1, int __r2)
{
    return __builtin_popcountll(__r1 ^ __r2);
}

int kNearestNeighbors(int queryID, int k = 30)
{
    vector< pair<int, int> > candidates;
    for (int& id : trainID)
    {
        int dist = 0;
        for (int row = 0; row < DIM; ++row)
            dist += distanceRow(dataPoint[queryID][row], dataPoint[id][row]);
        candidates.emplace_back(make_pair(dist, id));
    }
    sort(candidates.begin(), candidates.end(), [](const pair<int, int>& a, const pair<int, int>& b) -> bool
    {
        return (a.first == b.first ? uniform_int_distribution<int>(0, 1)(rng) : a.first < b.first);
    });
    int cntTop[10]; 
    memset(cntTop, 0, sizeof(cntTop));
    for (int i = 0; i < k; ++i)
    {
        int id = candidates[i].second;
        ++cntTop[label[id]];
    }
    vector<int> answer = {0};
    for (int i = 1; i <= 9; ++i)
    {
        if (cntTop[i] >= cntTop[answer[0]]) 
        {
            if (cntTop[i] > cntTop[answer[0]]) answer.clear();
            answer.emplace_back(i);
        }
    }
    return answer[uniform_int_distribution<int>(0, answer.size() - 1)(rng)];
}

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    loadData();
    splitData();
    vector<int> tryK = {20};
    const int TRIAL_TEST = 400;
    for (int k : tryK)
    {
        int correct = 0;
        for (int i = 0; i < TRIAL_TEST; ++i)
        {
            int id = testID[i];
            // cerr << i << endl;
            int ans = kNearestNeighbors(id, k);
            correct += (ans == label[id]);
        }
        // cerr << "done\n";
        cerr << k << " | " << correct << " " << TRIAL_TEST << " | " << correct / (ld)TRIAL_TEST << endl;
    }
}