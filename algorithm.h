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

using std::vector;
using std::set;

template<typename Type = QString>
class Algorithm
{
public:
    void fillSetLemmatize(QFile& in, set<Type>& set_of_keywords, Lemmatizator<>& lem);

    void fillSetLemmatize(const Type& in, set<Type>& set_of_keywords, Lemmatizator<>& lem);

    void fillSetLemmatize(const QByteArray& in_bytes, set<Type>& set_of_keywords, Lemmatizator<>& lem);

    void fillVectorLemmatize(QFile& in, vector<Type>& vector_of_keywords, Lemmatizator<>& lem);

    void fillVectorLemmatize(const Type& in, vector<Type>& vector_of_keywords, Lemmatizator<>& lem);

    void fillVectorLemmatize(const QByteArray& in_bytes, vector<Type>& vector_of_keywords, Lemmatizator<>& lem);

    bool jacarAlg(set<Type>& A, set<Type>& B, float percent);

    bool shinglAlg(vector<Type>& A, vector<Type>& B, float percent);
};

#endif // ALGORITHM_H
