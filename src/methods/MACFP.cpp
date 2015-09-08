#include "../utils/helper.h"
#include "../utils/suffix_array.h"
#include "../utils/my_string.h"
#include "../config/dna.h"
#include <queue>

#include <unordered_map>
#include <unordered_set>

#define MY_MAP unordered_map
#define MY_SET unordered_set

String *s, *rev_s;

vector<int> getOccurrence(int i, int j)
{
    vector<int> ret(1, i);
    int ptr = s->sa->rank[i];
    myAssert(s->sa->sa[ptr] == i, "Wrong SA rank");
    int length = j - i + 1;
    while (ptr > 0 && s->sa->height[ptr] >= length) {
        -- ptr;
        ret.push_back(s->sa->sa[ptr]);
    }
    ptr = s->sa->rank[i];
    while (ptr + 1 < s->size() && s->sa->height[ptr + 1] >= length) {
        ++ ptr;
        ret.push_back(s->sa->sa[ptr]);
    }
    return ret;
}

vector< vector<int> > prefixSum;

int getSum(const vector<int> &prefixSum, int l, int r)
{
    return prefixSum[r + 1] - prefixSum[l];
}

MY_MAP< ULL, vector<int> > bufferLeft, bufferRight;

vector<int> expandLeft(int leftBound, int l, int x, int remainDistance)
{
    ULL key = leftBound;
    key = key * MAGIC + l;
    key = key * MAGIC + x;
    key = key * MAGIC + remainDistance;
    if (bufferLeft.count(key)) {
        return bufferLeft[key];
    }
    vector<int> best(remainDistance + 1, -1);

    deque< pair<int, PII> > q;
    q.push_back(make_pair(0, make_pair(l - 1, x - 1)));

    while (q.size()) {
        int dist = q.front().first;
        int u = q.front().second.first;
        int v = q.front().second.second;
        q.pop_front();

        if (best[dist] != -1 && best[dist] >= v + 1) {
            continue;
        }

        if (u < leftBound) {
            if (best[dist] == -1 || best[dist] < v + 1) {
                best[dist] = v + 1;
            }
            continue;
        }

        int rev_u = (int)rev_s->size() - 1 - u;
        int rev_v = (int)rev_s->size() - 1 - v;
        if (u >= leftBound && v >= 0 && rev_s->s[rev_u] == rev_s->s[rev_v]) {
            int lcp = rev_s->sa->lcp(rev_u, rev_v);
            myAssert(u - lcp >= -1, "u-lcp error");
            myAssert(v - lcp >= -1, "v-lcp error");
            lcp = min(lcp, u - leftBound + 1);
            q.push_front(make_pair(dist, make_pair(u - lcp, v - lcp)));
        } else {
            for (int du = 0; du <= remainDistance - dist; ++ du) {
                if (u - du < leftBound - 1) {
                    break;
                }
                for (int dv = 0; dv <= remainDistance - dist; ++ dv) {
                    if (v - dv < -1) {
                        break;
                    }
                    if (du + dv > 0){
                        if (u - du == leftBound - 1 || v - dv == -1 || rev_s->s[rev_u + du] == rev_s->s[rev_v + dv]) {
                            q.push_back(make_pair(dist + max(du, dv), make_pair(u - du, v - dv)));
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < remainDistance; ++ i) {
        if (best[i] != -1) {
            if (best[i + 1] == -1 || best[i + 1] < best[i] + 1) {
                best[i + 1] = best[i] + 1;
            }
        }
    }
    return bufferLeft[key] = best;
}

vector<int> expandRight(int rightBound, int r, int y, int remainDistance)
{
    ULL key = rightBound;
    key = key * MAGIC + r;
    key = key * MAGIC + y;
    key = key * MAGIC + remainDistance;
    if (bufferRight.count(key)) {
        return bufferRight[key];
    }
    vector<int> best(remainDistance + 1, -1);

    deque< pair<int, PII> > q;
    q.push_front(make_pair(0, make_pair(r + 1, y + 1)));
    while (q.size()) {
        int dist = q.front().first;
        int u = q.front().second.first;
        int v = q.front().second.second;
        q.pop_front();

        if (best[dist] != -1 && best[dist] <= v - 1) {
            continue;
        }

        if (u > rightBound) {
            if (best[dist] == -1 || best[dist] > v - 1) {
                best[dist] = v - 1;
            }
            continue;
        }

        if (u <= rightBound && v < s->size() && s->s[u] == s->s[v]) {
            int lcp = s->sa->lcp(u, v);
            myAssert(u + lcp <= s->size(), "u+lcp error");
            myAssert(v + lcp <= s->size(), "v+lcp error");
            lcp = min(lcp, rightBound - u + 1);
            q.push_front(make_pair(dist, make_pair(u + lcp, v + lcp)));
        } else {
            for (int du = 0; du <= remainDistance - dist; ++ du) {
                if (u + du > rightBound + 1) {
                    break;
                }
                for (int dv = 0; dv <= remainDistance - dist; ++ dv) {
                    if (v + dv > s->size()) {
                        break;
                    }
                    if (du + dv > 0){
                        if (u + du == rightBound + 1 || v + dv == s->size() || s->s[u + du] == s->s[v + dv]) {
                            q.push_back(make_pair(dist + max(du, dv), make_pair(u + du, v + dv)));
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < remainDistance; ++ i) {
        if (best[i] != -1) {
            if (best[i + 1] == -1 || best[i + 1] > best[i] - 1) {
                best[i + 1] = best[i] - 1;
            }
        }
    }
    return bufferRight[key] = best;
}

bool checkByPrefixSum(int leftBound, int rightBound, int l, int r, int x, int y, int remainDistance)
{
    int length = rightBound - r + 1;
    int xMin = min((int)s->size() - 1, y + length + 1 - remainDistance);
    int xMax = min((int)s->size() - 1, y + length + 1 + remainDistance);
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < alphabet.size(); ++ i) {
        int goal = getSum(prefixSum[i], r, rightBound);
        int mini = getSum(prefixSum[i], y, xMin);
        int maxi = getSum(prefixSum[i], y, xMax);
        if (goal < mini) {
            sum1 += mini - goal;
        } else if (goal > maxi) {
            sum2 += goal - maxi;
        }
    }
    int dist = max(sum1, sum2);
    if (dist > remainDistance) {
        return false;
    }

    length = l - leftBound + 1;
    int yMin = max(0, x - length + 1 - remainDistance);
    int yMax = max(0, x - length + 1 + remainDistance);
    sum1 = sum2 = 0;
    for (int i = 0; i < alphabet.size(); ++ i) {
        int goal = getSum(prefixSum[i], leftBound, l);
        int mini = getSum(prefixSum[i], yMax, x);
        int maxi = getSum(prefixSum[i], yMin, x);
        if (goal < mini) {
            sum1 += mini - goal;
        } else if (goal > maxi) {
            sum2 += goal - maxi;
        }
    }
    dist += max(sum1, sum2);
    return dist <= remainDistance;
}

int counter = 0;

int CHUNK_SIZE;
MY_SET<ULL> explored;

void dfs(int l, int r, int firstPosition, int lastPosition, int firstChunkStart, int lastChunkEnd, int remainDistance, vector<PII> &supports)
{
    ULL key = firstChunkStart;
    key = key * MAGIC + lastChunkEnd;
    key = key * MAGIC + firstPosition;
    key = key * MAGIC + lastPosition;

    if (explored.count(key)) {
        return;
    }
    explored.insert(key);

    if (!checkByPrefixSum(l, r, firstChunkStart, lastChunkEnd, firstPosition, lastPosition, remainDistance)) {
        return;
    }

    counter ++;

    vector<int> left = expandLeft(l, firstChunkStart, firstPosition, remainDistance);
    int leftStart = remainDistance + 1, leftEnd = 0;
    for (int i = 0; i <= remainDistance; ++ i) {
        if (left[i] != -1) {
            leftStart = min(leftStart, i);
            leftEnd = i;
        }
    }
    if (leftStart > leftEnd) {
        return;
    }

    vector<int> right = expandRight(r, lastChunkEnd, lastPosition, remainDistance);
    int rightStart = remainDistance + 1, rightEnd = 0;
    for (int i = 0; i <= remainDistance; ++ i) {
        if (right[i] != -1) {
            rightStart = min(rightStart, i);
            rightEnd = i;
        }
    }
    if (rightStart > rightEnd) {
        return;
    }

    for (int leftK = leftStart; leftK <= leftEnd; ++ leftK) {
        if (left[leftK] == -1) {
            continue;
        }
        for (int rightK = rightStart; rightK <= rightEnd && rightK <= remainDistance - leftK; ++ rightK) {
            if (right[rightK] == -1) {
                continue;
            }
            if (max(l, left[leftK]) <= min(r, right[rightK])) {
                continue;
            }
            supports.push_back(make_pair(left[leftK], right[rightK]));
        }
    }
}

vector<PII> getSup(int l, int r)
{
    int length = r - l + 1;
    myAssert(length >= LEN_THRES, "Length OK!");

    CHUNK_SIZE = (r - l + 1) / (EDIT_DIS_THRES + 1);
    //CHUNK_SIZE = LEN_THRES / (EDIT_DIS_THRES + 1);
    int chunkN = (r - l + 1) / CHUNK_SIZE;

    myAssert(chunkN >= EDIT_DIS_THRES + 1, "Not Enough Chunks!");

    explored.clear();
    vector<PII> ret(1, make_pair(l, r));
    for (int i = 0, start = l; i < chunkN; ++ i) {
        int end = start + CHUNK_SIZE + (i < (r - l + 1) % CHUNK_SIZE) - 1;
        vector<int> positions = getOccurrence(start, end);
        FOR (match, positions) {
            if ((*match) == start) {
                continue;
            }
            dfs(l, r, (*match), (*match) + end - start, start, end, EDIT_DIS_THRES, ret);
        }
        vector<PII> temp = makeMaximal(ret);
        if (temp.size() >= MIN_SUP) {
            return temp;
        }
        start = end + 1;
    }
    return makeMaximal(ret);
}

unordered_set<ULL> checked;

bool enumerateEdit(int ptr, int length, int l, int r, int left, int right, int remainDist, vector<PII> &ret)
{
    if (remainDist == 0 && left == right) {
        return false;
    }
    if (ptr == r + 1) {
        ULL key = (left * MAGIC + right) * MAGIC + length;
        if (checked.count(key)) {
            return false;
        }
        checked.insert(key);
        for (int i = left; i <= right; ++ i) {
            int start = s->sa->sa[i];
            int end = s->sa->sa[i] + length - 1;
            if (max(start, l) > min(end, r)) {
                ret.push_back(make_pair(start, end));
            }
        }
        if (makeMaximal(ret).size() >= MIN_SUP) {
            return true;
        }
    }
    int last[alphabet.size()];
    for (int i = 0; i < alphabet.size(); ++ i) {
        int l = (i == 0) ? left - 1 : last[i - 1];
        int r = right + 1;
        while (l + 1 < r) {
            int mid = l + r >> 1;
            int index = s->sa->sa[mid] + length;
            if (index < s->size() && alphabet[i] < s->s[index]) {
                r = mid;
            } else {
                l = mid;
            }
        }
        last[i] = l;
    }
    for (int i = 0; i < alphabet.size(); ++ i) {
        int new_left = (i == 0) ? left : last[i - 1] + 1;
        while (s->sa->sa[new_left] + length == s->size()) {
            ++ new_left;
        }
        int new_right = last[i];

        if (new_left > new_right) {
            continue;
        }
        myAssert(s->s[s->sa->sa[new_left] + length] == alphabet[i], "index error");
        myAssert(s->s[s->sa->sa[new_right] + length] == alphabet[i], "index error");

        // replace
        if (ptr <= r && remainDist >= (s->s[ptr] != alphabet[i])) {
            if (enumerateEdit(ptr + 1, length + 1, l, r, new_left, new_right, remainDist - (s->s[ptr] != alphabet[i]), ret)) {
                return true;
            }
        }
        //insert
        if (remainDist > 0) {
            if (enumerateEdit(ptr, length + 1, l, r, new_left, new_right, remainDist - 1, ret)) {
                return true;
            }
        }
    }
    // delete
    if (remainDist > 0 && ptr <= r) {
        if (enumerateEdit(ptr + 1, length, l, r, left, right, remainDist - 1, ret)) {
            return true;
        }
    }
    return false;
}

const int SMALL_PATCH = EDIT_DIS_THRES * 3 + 1;

vector<PII> checkSmallPatch(int l, int r)
{
    int length = r - l + 1;
    //myAssert(length <= SMALL_PATCH, "only for small patch!");
    vector<PII> ret(1, make_pair(l, r));
    enumerateEdit(l, 0, l, r, 1, s->size(), EDIT_DIS_THRES, ret);
    return makeMaximal(ret);
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

    cerr << "n = " << dna.size() << endl;

    size_t n = dna.size();

    s = new String(dna);
    reverse(dna.begin(), dna.end());
    rev_s = new String(dna);
    reverse(dna.begin(), dna.end());

    prefixSum.resize(alphabet.size(), vector<int>(dna.size() + 1, 0));
    for (int i = 0; i < dna.size(); ++ i) {
        for (int j = 0; j < alphabet.size(); ++ j) {
            prefixSum[j][i + 1] = prefixSum[j][i];
        }
        int index = alphabet.find(dna[i]);
        prefixSum[index][i + 1] += 1;
    }

    cerr << "initialization done" << endl;

    FILE* out = tryOpen(OUTPUT_FILENAME, "w");
    int lastJ = -1;
    int cnt = 0;
    for (int i = 0, j = 0; i + LEN_THRES - 1 < n; ++ i) {
        j = max(j, i + LEN_THRES - 1);

        bufferLeft.clear();
        vector<PII> sup = getSup(i, j);
        while (j < n && sup.size() >= MIN_SUP) {
            bufferRight.clear();
            vector<PII> newSup = getSup(i, j);
            if (newSup.size() >= MIN_SUP) {
                sup = newSup;
                ++ j;
            } else {
                break;
            }
        }

        if (j > lastJ && sup.size() >= MIN_SUP) {
            //cerr << i << " " << j - 1 << endl;
            lastJ = j;
            fprintf(out, "%d %d ", i, j - 1);
            for (int k = i; k < j; ++ k) {
                fprintf(out, "%c", dna[k]);
            }
            fprintf(out, "\n");
            fprintf(out, "supports: %d\n", sup.size());
            FOR (pair, sup) {
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
    cerr << "counter = " << counter << endl;
    fclose(out);

    cout << "running time: " << (clock() - start) / CLOCKS_PER_SEC << endl;
    cout << "# of patterns: " << cnt << endl;

    return 0;
}
