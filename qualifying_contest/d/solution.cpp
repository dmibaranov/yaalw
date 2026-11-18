#include <bits/stdc++.h>
using namespace std;
// D = q1 + reverse(q2) as one doubly linked list. Closing or opening a counter
// never changes D; it only moves the split cursor (to an end or to the middle).
// A pointer to the middle gap is maintained in O(1) per event.
int main() {
    size_t cap = 1 << 16, sz = 0;
    char *buf = (char*)malloc(cap);
    for (;;) {
        if (sz == cap) { cap <<= 1; buf = (char*)realloc(buf, cap); }
        size_t r = fread(buf + sz, 1, cap - sz, stdin);
        if (!r) break;
        sz += r;
    }
    size_t i = 0;
    while (i < sz && (buf[i] < '0' || buf[i] > '9')) ++i;
    long long n = 0;
    while (i < sz && buf[i] >= '0' && buf[i] <= '9') n = n * 10 + (buf[i++] - '0');
    while (i < sz && (buf[i] == ' ' || buf[i] == '\n' || buf[i] == '\r' || buf[i] == '\t')) ++i;
    const char *s = buf + i;
    size_t m = 0;
    while (i + m < sz && m < (size_t)n && buf[i + m] > ' ') ++m;

    const int H = 0, TL = (int)n + 1;
    int *nx = (int*)malloc(sizeof(int) * (n + 2));
    int *pv = (int*)malloc(sizeof(int) * (n + 2));
    nx[H] = TL; pv[TL] = H;
    char *out = (char*)malloc(m + 2);
    size_t on = 0;
    int cnt = 0, len = 0, p = 0, C = H, gm = 0, Mn = H;

    for (size_t k = 0; k < m; ++k) {
        char ch = s[k];
        switch (ch) {
        case 'a': case 'b': {
            int v = ++cnt, X = C, Y = nx[X];
            nx[v] = Y; pv[v] = X; pv[Y] = v; nx[X] = v;
            if (p < gm) ++gm;
            if (ch == 'a') { C = v; ++p; }
            ++len;
            break;
        }
        case 'A': {
            int v = nx[H];
            out[on++] = char('0' + v % 10);
            if (gm >= 1) { --gm; if (Mn == v) Mn = H; }
            if (C == v) C = H;
            --p;
            int Y = nx[v]; nx[H] = Y; pv[Y] = H;
            --len;
            break;
        }
        case 'B': {
            int v = pv[TL];
            out[on++] = char('0' + v % 10);
            if (Mn == v) { Mn = pv[v]; --gm; }
            int X = pv[v]; nx[X] = TL; pv[TL] = X;
            --len;
            break;
        }
        case '>': p = 0; C = H; break;
        case ']': p = len; C = pv[TL]; break;
        case '<': p = gm; C = Mn; break;
        case '[':
            if (len & 1) { p = gm + 1; C = nx[Mn]; } else { p = gm; C = Mn; }
            break;
        }
        int t = len >> 1;
        while (gm < t) { Mn = nx[Mn]; ++gm; }
        while (gm > t) { Mn = pv[Mn]; --gm; }
    }
    out[on++] = '\n';
    fwrite(out, 1, on, stdout);
    return 0;
}
