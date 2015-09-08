#include "../utils/helper.h"
#include "../config/dna.h"

void update(int &a, int b)
{
    if (a > b) {
        a = b;
    }
}

vector< pair<int, int> > getSup(string &dna, int n, int l, int r)
{
    vector< pair<int, int> > Q(1, make_pair(l, r));
    for (int i = 0; i < n; ++ i) {
        if (l <= i && i <= r) {
            continue;
        }
        vector< vector<int> > f(r - l + 2, vector<int>(EDIT_DIS_THRES * 2 + 1, EDIT_DIS_THRES + 1));
        f[0][0 + EDIT_DIS_THRES] = 0;
        for (int x = 0; x <= r - l + 1; ++ x) {
            for (int delta = -EDIT_DIS_THRES; delta <= EDIT_DIS_THRES; ++ delta) {
                int j = i + x + delta;
                if (j < i || j > n + 1) {
                    continue;
                }
                int dist = f[x][delta + EDIT_DIS_THRES];
                if (dist > EDIT_DIS_THRES) {
                    continue;
                }

                if (x < r - l + 1 && j < n) {
                    update(f[x + 1][EDIT_DIS_THRES + delta], dist + (dna[l + x] != dna[j]));
                }
                if (delta + 1 <= EDIT_DIS_THRES) {
                    update(f[x][EDIT_DIS_THRES + delta + 1], dist + 1);
                }
                if (x < r - l + 1 && delta - 1 >= -EDIT_DIS_THRES) {
                    update(f[x + 1][EDIT_DIS_THRES + delta - 1], dist + 1);
                }
            }
        }
        for (int delta = -EDIT_DIS_THRES; delta <= EDIT_DIS_THRES; ++ delta) {
            int j = i + r - l + 1 + delta;
            if (f[r - l + 1][delta + EDIT_DIS_THRES] <= EDIT_DIS_THRES) {
                if (max(l, i) <= min(j - 1, r)) {
                    continue;
                }
                Q.push_back(make_pair(i, j - 1));
            }
        }
    }
    return makeMaximal(Q);
}

int main(int argc, char* argv[])
{
    loadConfiguration(argc, argv);

    cout << "settings:" << endl;
    cout << "\tfilename = " << DNA_FILENAME << endl;
    cout << "\tmin_sup = " << MIN_SUP << endl;
    cout << "\tlen_thres = " << LEN_THRES << endl;
    cout << "\tedit_thres = " << EDIT_DIS_THRES << endl;

    double start = clock();

    string dna = loadDNASequence();
    //dna = "acgtacgt";

    int n = dna.size();
    cerr << "length of DNA sequence = " << n << endl;

    //cerr << getSup(dna, n, 1958, 1970).size() << endl; return 0;
    /*cerr << "valid = " << valid(dna, n, 0, 0) << endl;
    cerr << "valid = " << valid(dna, n, 0, 1) << endl;
    return 0;*/

    FILE* out = tryOpen(OUTPUT_FILENAME, "w");
    int lastJ = -1;
    int cnt = 0;
    for (int i = 0, j = 0; i < n; ++ i) {
        j = max(j, i);
        while (j < n && getSup(dna, n, i, j).size() >= MIN_SUP) {
            ++ j;
        }
        if (j > lastJ) {
            lastJ = j;
            if (j - i < LEN_THRES) {
                continue;
            }
            vector< pair<int, int> > sups = getSup(dna, n, i, j - 1);
            //cerr << i << " " << j << endl;
            fprintf(out, "%d %d ", i, j - 1);
            for (int k = i; k < j; ++ k) {
                fprintf(out, "%c", dna[k]);
            }
            fprintf(out, "\n");
            fprintf(out, "supports: %d\n", sups.size());
            FOR (pair, sups) {
                fprintf(out, "\t%d %d ", pair->first, pair->second);
                for (int k = pair->first; k <= pair->second; ++ k) {
                    fprintf(out, "%c", dna[k]);
                }
                fprintf(out, "\n");
            }
            cnt ++;
        }
    }
    cerr << "# approximate patterns = " << cnt << endl;
    fclose(out);

    cout << "running time: " << (clock() - start) / CLOCKS_PER_SEC << endl;
    cout << "# of patterns: " << cnt << endl;

    return 0;
}
