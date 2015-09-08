#ifndef __Suffix_Array_H__
#define __Suffix_Array_H__

#include "helper.h"

struct SuffixArray
{
    static void selfTest() {
        vector<int> a;
        a.push_back(0);
        a.push_back(1);
        a.push_back(1);
        a.push_back(0);
        a.push_back(0);
        a.push_back(1);
        a.push_back(1);
        SuffixArray sa(a);
        sa.show();
    }

    void show() {
        cerr << "alphabet size = " << m << endl;
        for (int i = 0; i < n; ++ i) {
            //printf("%d ", sa[i]);
            for (int j = sa[i]; j < n; ++ j) {
                cerr << s[j];
            }
            cerr << endl;
            cerr << "height = " << height[i] << endl;
        }

        for (int i = 0; i < n; ++ i) {
            cerr << rank[i] << endl;
        }
    }

    long long n;
    int m;
    #define F(x) ((x)/3+((x)%3==1?0:tb))
    #define G(x) ((x)<tb?(x)*3+1:((x)-tb)*3+2)

    vector<int> s, sa, wa, wb, wv, wss;

    vector<long long> rank, height;
    vector<long long> segment_tree;

    #define ID(l,r) (((l) + (r)) | ((l) != (r)))

    void build_tree(long long l, long long r) {
        if (l == r) {
            segment_tree[ID(l, r)] = height[l];
            return;
        }
        long long mid = l + r >> 1;
        build_tree(l, mid);
        build_tree(mid + 1, r);
        segment_tree[ID(l, r)] = min(segment_tree[ID(l, mid)], segment_tree[ID(mid + 1, r)]);
    }

    static const long long INF_LL = 1000000000000000000LL;

    long long getMin(long long l, long long r, long long ll, long long rr) {
        if (r < ll || l > rr) {
            return INF_LL;
        }
        if (ll <= l && r <= rr) {
            return segment_tree[ID(l, r)];
        }
        long long mid = l + r >> 1;
        return min(getMin(l, mid, ll, rr), getMin(mid + 1, r, ll, rr));
    }

    long long lcp(long long i, long long j) {
        long long ret = 0;
        if (i == j) {
            ret = n - 1 - i;
        } else {
            long long l = rank[i], r = rank[j];
            if (l > r) {
                swap(l, r);
            }
            l += 1;
            ret = getMin(0, n - 1, l, r);
        }
        return ret;
    }

    SuffixArray(const vector<int> &str) : s(str) {
        n = s.size();
        m = 0;
        for (int i = 0; i < n; ++ i) {
            ++ s[i];
            m = max(m, s[i] + 1);
        }
        s.push_back(0);
        ++ n;

        s.resize(n * 3, 0);
        sa.resize(n * 3, 0);
        wa.resize(n, 0);
        wb.resize(n, 0);
        wv.resize(n, 0);
        wss.resize(n, 0);

        dc3(&s[0], &sa[0], n, m);

        wa.clear(); wa.shrink_to_fit();
        wb.clear(); wb.shrink_to_fit();
        wv.clear(); wv.shrink_to_fit();
        wss.clear(); wss.shrink_to_fit();

        sa.resize(n); sa.shrink_to_fit();

        rank.resize(n);
        height.resize(n);
        for (long long i = 0; i < n; ++ i) {
            rank[sa[i]] = i;
        }
        for (long long i = 0, j, k = 0; i < n; height[rank[i ++]] = k) {
            if (rank[i] == 0) {
                continue;
            }
            for (k ? -- k : 0, j = sa[rank[i] - 1]; s[i + k] == s[j + k]; ++ k);
        }

        segment_tree.resize(n * 2 + 5, 0);
        build_tree(0, n - 1);
    }

    int c0(int *r, long long a, long long b) {
        return r[a] == r[b] && r[a+1] == r[b+1] && r[a+2] == r[b+2];
    }

    int c12(int k, int *r, long long a, long long b) {
        if (k == 2) {
            return r[a] < r[b] || r[a] == r[b] && c12(1, r, a + 1, b + 1);
        } else {
            return r[a] < r[b] || r[a] == r[b] && wv[a + 1] < wv[b + 1];
        }
    }

    void sort(int *r, int *a, int *b, long long n, int m) {
        for (long long i = 0; i < n; ++ i) {
            wv[i] = r[a[i]];
        }
        for (int i = 0; i < m; ++ i) {
            wss[i]=0;
        }
        for (long long i = 0; i < n; ++ i) {
            ++ wss[wv[i]];
        }
        for (int i = 1; i < m; ++ i) {
            wss[i] += wss[i - 1];
        }
        for (long long i = n - 1; i >= 0; -- i) {
            b[-- wss[wv[i]]] = a[i];
        }
    }

    void dc3(int *r, int *sa, long long n, int m) {
        int *rn = r + n, *san = sa + n, ta = 0, tb = (n + 1) / 3;
        long long i, j, p, tbc = 0;
        r[n] = r[n + 1] = 0;
        for (i = 0; i < n; ++ i) {
            if (i % 3 != 0) {
                wa[tbc ++]=i;
            }
        }
        sort(r+2, &wa[0], &wb[0], tbc, m);
        sort(r+1, &wb[0], &wa[0], tbc, m);
        sort(r, &wa[0], &wb[0], tbc, m);
        for(p = 1, rn[F(wb[0])] = 0, i = 1; i < tbc; ++ i) {
            rn[F(wb[i])] = c0(r, wb[i - 1], wb[i]) ? p - 1 : p ++;
        }
        if (p < tbc) {
            dc3(rn, san, tbc, p);
        } else {
            for (i = 0; i < tbc; ++ i) {
                san[rn[i]] = i;
            }
        }
        for (i = 0; i < tbc; ++ i) {
            if (san[i] < tb) {
                wb[ta ++] = san[i] * 3;
            }
        }
        if (n % 3 == 1) {
            wb[ta ++]=n - 1;
        }
        sort(r, &wb[0], &wa[0], ta, m);
        for (i = 0; i < tbc; ++ i) {
            wv[wb[i] = G(san[i])] = i;
        }
        for (i = 0, j = 0, p = 0; i < ta && j < tbc; ++ p) {
            sa[p] = c12(wb[j] % 3, r, wa[i], wb[j]) ? wa[i ++] : wb[j++];
        }
        for (; i < ta; ++ p) {
            sa[p] = wa[i ++];
        }
        for (; j < tbc; ++ p) {
            sa[p] = wb[j ++];
        }
    }
};

#endif
