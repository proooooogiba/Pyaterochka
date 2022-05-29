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

    void fill_vec(QFile& in, vector <QString>& key_words_vec);

    void fill_vecb(const QByteArray& in, vector<QString>& key_words_vec);

    void fill_set(QFile& in, set<QString>& set_of_keywords);

    void fill_setb(const QByteArray& in, set<QString>& set_of_keywords);

    void fill_set_lemmatize(QFile& in, set<QString>& set_of_keywords, Lemmatizator& lem);

    void fill_set_lemmatize(const QString& in, set<QString>& set_of_keywords, Lemmatizator& lem);

    void fill_set_lemmatize(const QByteArray& in_bytes, set<QString>& set_of_keywords, Lemmatizator& lem);

    void fill_vector_lemmatize(QFile& in, vector<QString>& vector_of_keywords, Lemmatizator& lem);

    void fill_vector_lemmatize(const QString& in, vector<QString>& vector_of_keywords, Lemmatizator& lem);

    void fill_vector_lemmatize(const QByteArray& in_bytes, vector<QString>& vector_of_keywords, Lemmatizator& lem);

    bool Jacar_alg(set<QString>& A, set<QString>& B, float percent);

    bool Shingl_alg(vector<QString>& A, vector<QString>& B, float percent);

};

#endif // ALGORITHM_H
