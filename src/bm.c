/**
 * @file bm.c
 * @author hutusi (hutusi@outlook.com)
 * @brief Refer to bm.h
 * @date 2019-08-10
 *
 * @copyright Copyright (c) 2019, hutusi.com
 *
 */

#include "bm.h"
#include "def.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

STATIC void bm_calculate_bad_chars(const char *pattern, int len, int *bad_chars)
{
    for (int i = 0; i < 256; ++i) {
        bad_chars[i] = -1;
    }

    for (int i = len - 1; i >= 0; --i) {
        int ch = pattern[i];
        if (bad_chars[ch] < 0) {
            bad_chars[ch] = i;
        }
    }
}

STATIC void bm_calculate_good_suffixes(const char *pattern,
                                int len,
                                int *good_suffixes)
{
    for (int i = 0; i <= len; ++i) {
        good_suffixes[i] = -1;
    }

    for (int i = 0; i < len - 1; ++i) {
        int j = i;
        int k = 0;
        while (j >= 0 && pattern[j] == pattern[len - 1 + j - i]) {
            ++k;
            good_suffixes[k] = j;
            --j;
        }
    }
}

STATIC int bm_move_by_good_suffixes(int *good_suffixes, int len, int good_len)
{
    if (good_len == 0) {
        return 1;
    } else {
        return len - good_suffixes[good_len] - good_len;
    }
}

#define MAX(a, b) ((a) < (b) ? (b) : (a))

int bm_string_match(const char *text, const char *pattern)
{
    int text_len = strlen(text);
    int pat_len = strlen(pattern);

    int bad_chars[256];
    bm_calculate_bad_chars(pattern, pat_len, bad_chars);

    int *good_suffixes = (int *)malloc((pat_len + 1) * sizeof(int));
    bool *prefixes = (bool *)malloc(pat_len * sizeof(bool));
    bm_calculate_good_suffixes(pattern, pat_len, good_suffixes);

    int index = -1;
    int cursor = 0;
    while (cursor <= text_len - pat_len) {
        int i = 0;
        for (; i < pat_len; ++i) {
            if (text[cursor + pat_len - 1 - i] == pattern[pat_len - 1 - i]) {

            } else {
                int bad =
                    pat_len - bad_chars[(int)text[cursor + pat_len - 1 - i]] - 1;
                int good = bm_move_by_good_suffixes(good_suffixes, pat_len, i);
                cursor += MAX(bad, good);
                break;
            }
        }

        if (i == pat_len) { // found it!
            index = cursor;
            break;
        }
    }

    free(good_suffixes);
    free(prefixes);
    return index;
}
