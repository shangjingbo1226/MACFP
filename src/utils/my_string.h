#ifndef __STRING_H__
#define __STRING_H__

#include "suffix_array.h"

typedef unsigned long long ULL;
const ULL MAGIC = 0xabcdef;

struct String
{
    string s;
    SuffixArray *sa;
    ULL *h, *pw;

    size_t size() {
        return s.size();
    }

    struct SubString
    {
        String* parent;
        long long l, r;

        SubString(String *parent, long long l, long long r) : parent(parent), l(l), r(r) {
        }

        long long lcp(long long x, long long y) {
            long long ret = parent->sa->lcp(l, x);
            ret = min(ret, r - l + 1);
            ret = min(ret, y - x + 1);
            return ret;
        }
    };

    String(string s) : s(s) {
        vector<int> a;
        map<char, int> remap;
        vector<bool> occur(256, false);
        for (size_t i = 0; i < s.size(); ++ i) {
            occur[s[i] + 128] = true;
        }
        for (int ch = -128; ch < 128; ++ ch) {
            if (occur[ch + 128]) {
                int new_id = remap.size();
                remap[ch] = new_id;
            }
        }
        for (size_t i = 0; i < s.size(); ++ i) {
            a.push_back(remap[s[i]]);
        }
        sa = new SuffixArray(a);

        h = new ULL[s.size() + 1];
        pw = new ULL[s.size() + 1];
        h[0] = 0;
        pw[0] = 1;
        for (size_t i = 0; i < s.size(); ++ i) {
            h[i + 1] = h[i] * MAGIC + (a[i] + 1);
            pw[i + 1] = pw[i] * MAGIC;
        }
    }

    ~String() {
        delete [] h;
        delete [] pw;
        delete sa;
    }

    ULL getHash(long long l, long long r) {
        return h[r + 1] - h[l] * pw[r - l + 1];
    }

    SubString substr(long long i, long long len) {
        return SubString(this, i, i + len - 1);
    }
};

#endif
