#include "algorithm.h"
#include "lemmatizator.h"
#include <QDebug>
#include <QFile>

template<typename Type>
void Algorithm<Type>::fillSetLemmatize(QFile& in, set<Type>& set_of_keywords, Lemmatizator<>& lem) {
    QByteArray bytes = in.readAll();
    Type lemmatized_text = lem.lemmatizeBinary(bytes);
    for (Type word : lemmatized_text.split(" ")) {
        set_of_keywords.insert(word);
    }
}

template<typename Type>
void Algorithm<Type>::fillVectorLemmatize(QFile& in, vector<Type>& vector_of_keywords, Lemmatizator<>& lem) {
    QByteArray bytes = in.readAll();
    Type lemmatized_text = lem.lemmatizeBinary(bytes);
    for (Type word : lemmatized_text.split(" ")) {
        vector_of_keywords.emplace_back(word);
    }
}

template<typename Type>
void Algorithm<Type>::fillSetLemmatize(const Type& in, set<Type>& set_of_keywords, Lemmatizator<>& lem) {
    QByteArray bytes = in.toUtf8();
    Type lemmatized_text = lem.lemmatizeBinary(bytes);
    for (QString word : lemmatized_text.split(" ")) {
        set_of_keywords.insert(word);
        qDebug() << word << '\n';
    }
}

template<typename Type>
void Algorithm<Type>::fillVectorLemmatize(const Type& in, vector<Type>& vector_of_keywords, Lemmatizator<>& lem) {
    QByteArray bytes = in.toUtf8();
    Type lemmatized_text = lem.lemmatizeBinary(bytes);
    for (Type word : lemmatized_text.split(" ")) {
        vector_of_keywords.emplace_back(word);
        qDebug() << word << '\n';
    }
}

template<typename Type>
void Algorithm<Type>::fillSetLemmatize(const QByteArray& in_bytes, set<Type>& set_of_keywords, Lemmatizator<>& lem) {
    Type lemmatized_text = lem.lemmatizeBinary(in_bytes);
    for (Type word : lemmatized_text.split(" ")) {
        set_of_keywords.insert(word);
        qDebug() << word << '\n';
    }
}

template<typename Type>
void Algorithm<Type>::fillVectorLemmatize(const QByteArray& in_bytes, vector<Type>& vector_of_keywords, Lemmatizator<>& lem) {
    Type lemmatized_text = lem.lemmatizeBinary(in_bytes);
    for (Type word : lemmatized_text.split(" ")) {
        vector_of_keywords.emplace_back(word);
        qDebug() << word << '\n';
    }
}

template<typename Type>
bool Algorithm<Type>::jacarAlg(set<Type>& A, set<Type>& B, float percent) {
    vector<Type> intersection_vec;
    vector<Type> union_vec;

    std::set_intersection(A.begin(), A.end(),
                          B.begin(), B.end(),
                          std::back_inserter(intersection_vec));
    std::set_union(A.begin(),A.end(),
                   B.begin(), B.end(),
                   std::back_inserter(union_vec));

    if (union_vec.size() == 0) {
        return false;
    }

    double coefficent = (double)intersection_vec.size() / union_vec.size();

    qDebug() << "\"coef\": " << 100 * coefficent << ",";
    qDebug() << "\"percent\": " << percent << ",";
    if (100 * coefficent < percent) {
        return false;
    }
    return true;
}

template<typename Type>
bool Algorithm<Type>::shinglAlg(vector <Type>& A, vector <Type>& B, float percent) {
    set <Type> set_A;
    set <Type> set_B;
    for (size_t i = 0; i < A.size() - 1; ++i) {
        Type tmp;
        tmp+= A[i] + ' ' + A[i + 1];
        set_A.insert(tmp);
    }
    for (size_t i = 0; i < B.size() - 1; ++i) {
        Type tmp;
        tmp += B[i] + ' ' + B[i + 1];
        set_B.insert(tmp);
    }
    bool result = jacarAlg(set_A, set_B, percent);
    return result;
}

template class Algorithm<QString>;

