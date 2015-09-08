#ifndef __MY_HELPER__
#define __MY_HELPER__

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

#define FOR(i,a) for (__typeof((a).begin()) i = (a).begin(); i != (a).end(); ++ i)
typedef pair<int, int> PII;

bool bySecond(const PII &a, const PII &b)
{
    return a.second < b.second || a.second == b.second && a.first < b.first;
}

vector<PII> makeMaximal(vector<PII> &maximal)
{
    sort(maximal.begin(), maximal.end(), bySecond);
    maximal.erase(unique(maximal.begin(), maximal.end()), maximal.end());
    vector<PII> ret;
    int maxi = -1;
    for (size_t i = 0; i < maximal.size(); ++ i) {
        if (maximal[i].first <= maxi) {
            continue;
        }
        ret.push_back(maximal[i]);
        maxi = maximal[i].second;
    }
    return ret;
}

void myAssert(bool flg, string msg)
{
    if (!flg) {
        cerr << "[Fatal Error] " + msg << endl;
        exit(-1);
    }
}

void writeBinary(FILE* out, const string &s)
{
	int n = s.size();
	fwrite(&n, sizeof(n), 1, out);
	if (n > 0) {
		fwrite(&s[0], sizeof(s[0]), n, out);
	}
}

void readBinary(FILE* in, string &s)
{
	int n;
	fread(&n, sizeof(n), 1, in);
	s.resize(n);
	if (n > 0) {
		fread(&s[0], sizeof(s[0]), n, in);
	}
}

template<class T>
void writeBinary(FILE* out, const T &n)
{
	fwrite(&n, sizeof(n), 1, out);
}

template<class T>
void readBinary(FILE* in, T &n)
{
	fread(&n, sizeof(n), 1, in);
}

const int maxlength = 100000000;

char line[maxlength + 1];

bool getLine(FILE* in)
{
	bool ret = fgets(line, maxlength, in);
	int len = strlen(line);
	while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
		line[len - 1] = 0;
		-- len;
	}
	return ret;
}

FILE* tryOpen(string filename, string param)
{
	FILE* ret = fopen(filename.c_str(), param.c_str());
	if (ret == NULL) {
		fprintf(stderr, "error while opening %s\n", filename.c_str());
	}
	return ret;
}

vector<string> splitBy(const string &line, char comma)
{
	vector<string> tokens;
	string token = "";
	bool inside = false;
	for (size_t i = 0; i < line.size(); ++ i) {
		if (line[i] == '"') {
			inside ^= 1;
		} else if (line[i] == comma && !inside) {
			tokens.push_back(token);
			token = "";
		} else {
			token += line[i];
		}
	}
	tokens.push_back(token);
	return tokens;
}

bool fromString(const string &s, string &x)
{
	x = s;
	return true;
}

template<class T>
bool fromString(const string &s, T &x)
{
	stringstream in(s);
	return (in >> x);
}

#endif
