#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "lemmatizator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <QString>
#include <locale>
#include <QTextStream>
#include <QFile>

//#include "unicode/utypes.h"
//#include "unicode/coll.h"
//#include "unicode/unistr.h"
//#include "unicode/ustream.h"

using namespace std;


class Algorithm
{
public:
    bool check_familiar_words(const QString& a);
    void delete_symbol(QString& a);
    bool clean_word(QString & tmp);
    void fill_vec(QTextStream& in, vector <QString>& key_words_vec);

    void fill_set(QTextStream& in, set<QString>& set_of_keywords);
    void fill_set_lemmatize(QFile& in, set<QString>& set_of_keywords, Lemmatizator& lem);
    bool Jacar_alg(set<QString>& A, set<QString>& B);
    bool Shingl_alg(vector<QString>& A, vector<QString>& B);

};

#endif // ALGORITHM_H
