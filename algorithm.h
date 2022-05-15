#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include "unicode/utypes.h"
#include "unicode/coll.h"
#include "unicode/unistr.h"
#include "unicode/ustream.h"

using namespace std;


class Algorithm
{
    bool check_familiar_words(const icu::UnicodeString& a);
    void delete_symbol(icu::UnicodeString& a);
    bool clean_word(icu::UnicodeString & tmp);
    void fill_vec (ifstream in, vector <icu::UnicodeString>& key_words_vec);
    void fill_set(ifstream in, set<icu::UnicodeString>& set_of_keywords);
    bool Jacar_alg(set <icu::UnicodeString>& A, set <icu::UnicodeString>& B);
    bool Shingl_alg(vector <icu::UnicodeString>& A, vector <icu::UnicodeString>& B);

};

#endif // ALGORITHM_H
