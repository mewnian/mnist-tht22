#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;
typedef long double ld;

const int N_TRAIN = 60000;
const int N_TEST = 10000;
const int DIM = 40;
const int DIGITS = 10;
const int SEED = 20170812;

int cnt_train[DIGITS + 3], label_train[N_TRAIN + 3]; ull data_train[N_TRAIN + 3][DIM + 3];
int cnt_test[DIGITS + 3], label_test[N_TEST + 3]; ull data_test[N_TEST + 3][DIM + 3];
int nTrain = 0, nTest = 0;
vector<int> digitID[DIGITS + 3];

mt19937_64 rng(SEED);

void loadData()
{
    // load training set
    ifstream label_train_file("src/label_train.txt"), data_train_file("src/data_train.txt");
    for (int i = 0; i < N_TRAIN; ++i)
    {
        label_train_file >> label_train[i];
        for (int j = 0; j < DIM; ++j) data_train_file >> data_train[i][j];
        digitID[label_train[i]].emplace_back(i);
        cnt_train[label_train[i]]++;
    }
    label_train_file.close(); data_train_file.close();

    // load testing set
    ifstream label_test_file("src/label_test.txt"), data_test_file("src/data_test.txt");
    for (int i = 0; i < N_TEST; ++i)
    {
        label_test_file >> label_test[i];
        for (int j = 0; j < DIM; ++j) data_test_file >> data_test[i][j];
        cnt_test[label_test[i]]++;
    }
    label_test_file.close(); data_test_file.close();

    // shuffling data
    for (int digit = 0; digit < DIGITS; ++digit)
        shuffle(digitID[digit].begin(), digitID[digit].end(), rng);
}

int distanceRow(ull __r1, ull __r2)
{
    return __builtin_popcountll(__r1 ^ __r2);
}

//int distance(ull arr1[], ull arr2[])
//{
//    int dist = 0;
//    for (int i = 0; i < DIM; ++i) dist += distanceRow(arr1[i], arr2[i]);
//    return dist;
//}

bool compare(const pair<int, int>& __p1, const pair<int, int>& __p2)
{
    if (__p1.first == __p2.first) return uniform_int_distribution<int>(0, 1)(rng);
    return __p1.first < __p2.first;
}

vector< pair<int, int> > KNNcandidates;
int KNNcntTop[DIGITS];
bool KNNconsidered[DIGITS];

int kNearestNeighbors(int queryID, int k = 30, long double ratio_sample = 1.0)
{
    KNNcandidates.clear();
    fill(KNNconsidered, KNNconsidered + DIGITS, 1);
    int n_dUsed = 0;
    for (int digit = 0; digit < DIGITS; ++digit)
    {
        int n_dTrain = cnt_train[digit];
        int n_dSample = (int)(ratio_sample * n_dTrain);
        n_dUsed += n_dSample;
        for (int i = 0; i < n_dSample; ++i)
        {
            int id = digitID[digit][i];
            int dist = 0;
            for (int row = 0; row < DIM; ++row)
                dist += distanceRow(data_train[id][row], data_test[queryID][row]);
            KNNcandidates.emplace_back(make_pair(dist, id));
        }
    }
    sort(KNNcandidates.begin(), KNNcandidates.end(), compare);
    int answer = -1;
    for (int ITER = 0; ITER * k < n_dUsed; ++ITER)
    {
        fill(KNNcntTop, KNNcntTop + DIGITS, 0);
        int cntMax = -1, potentials = 0;
        for (int i = k * ITER; i < min(n_dUsed, k * (ITER + 1)); ++i)
        {
            int id = KNNcandidates[i].second;
            int d = label_train[id];
            KNNcntTop[d] += KNNconsidered[d];
            cntMax = max(cntMax, KNNcntTop[d]);
        }
        for (int digit = 0; digit < DIGITS; ++digit)
        {
            KNNconsidered[digit] = (KNNcntTop[digit] == cntMax);
            potentials += KNNconsidered[digit];
            if (KNNconsidered[digit]) answer = digit;
        }
        if (potentials == 1) break;
    }
    return answer;
}

int main(int argc, char** argv)
{
    // if (argc < 3)
    // {
    //     cout << "Invalid count of arguments - program cannot run";
    //     return 0;
    // }
    ios_base::sync_with_stdio(0); cin.tie(0);
    loadData();
    const int TRIAL_TEST = 10000;
    // const int TRIAL_TEST = atoi(argv[1]);
    assert(TRIAL_TEST <= N_TEST);
    // vector< tuple<int, int, int, ld> > result;
    // for (int _ = 2; _ < argc; ++_)
    {
        // int k = atoi(argv[_]);
        int k = 5;
        int correct = 0;
        for (int test_id = 0; test_id < TRIAL_TEST; ++test_id)
        {
            int output = kNearestNeighbors(test_id, k), answer = label_test[test_id];
            cerr << "run function successfully\n";
            bool verdict = (output == answer);
            correct += verdict;
            ld ratio = 100.0 * (ld)correct / (ld)(test_id + 1);
            if (verdict)
            {
                cout << "\033[32m" << "Image #" << test_id << " | Pred = " << output << ", Answer = " << answer << " | " << "SUCCESS" << endl;
            }
            else
            {
                cout << "\033[31m" << "Image #" << test_id << " | Pred = " << output << ", Answer = " << answer << " | " << "FAILURE" << endl;

            }
            cout << "\033[0m";
            cout << "Current accuracy = " << ratio << "%" << endl;
        }
        cout << "\033[0m";
        cout << "k = " << k << " | Correct = " << correct << " / " << TRIAL_TEST << " | Accuracy = " << 100.0 * correct / (ld)TRIAL_TEST << "%" << endl;
        cout << "=============================" << endl;
        // result.emplace_back(make_tuple(k, correct, TRIAL_TEST, 100.0 * correct / (ld)TRIAL_TEST));
    }
    // cout << "\033[36m";
    // cout << "RESULTS" << endl;
    // for (auto& r : result)
    // {
    //     auto [t1, t2, t3, t4] = r;
    //     cout << "k = " << t1 << " | Correct = " << t2 << " / " << t3 << " | Accuracy = " << t4 << "%" << endl;
    // }
    cout << "\033[0m";
}
