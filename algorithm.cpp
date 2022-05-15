#include "algorithm.h"

bool Algorithm::check_familiar_words(const QString& a) {
    vector <QString> prepositions = { "безо", "близ",  "вместо", "из-за", "из-под", "кроме", "между",
                                    "перед", "передо","пред", "пред", "подо", "ради",  "сквозь", "среди", "через", "чрез"};
    if (find(prepositions.begin(), prepositions.end(),a) != prepositions.end()) {
        return true;
    }
    vector <QString> unions = {"вдобавок", "именно", "также", "благодаря", "тому", "благо", "буде", "будто",
                                    "если", "из-за", "дабы", "даже", "едва", "лишь", "ежели", "затем",
                                    "зато", "зачем", "значит", "следовательно", "тогда", "кабы", "как-то",
                                    "коли", "либо", "ради", "нежели", "только", "невзирая", "независимо", "несмотря", "однако",
                                    "особенно", "оттого", "отчего", "пока", "покамест", "покуда", "поскольку",
                                    "потому", "почему", "прежде", "притом", "причем", "пускай", "пусть", "ради", "раньше",
                                    "словно", "также", "тогда", "тоже", "только", "точно", "хотя", "чтоб", "чтобы"
                                    };
    if (find(unions.begin(), unions.end(),a) != unions.end()) {
        return true;
    }

    return false;
}

void Algorithm::delete_symbol(QString& a) {

    vector <char16_t> symbols = {',', '.', '?', '!', ';', ':', ')', '(', '}', '{', '\"', '\''};

    while (find(symbols.begin(), symbols.end(),a[a.length() - 1]) != symbols.end()) {
        a.truncate(a.length() -1);
    }

    while (find(symbols.begin(), symbols.end(),a[0]) != symbols.end()) {
        a.remove(0, 1);
    }
}

bool Algorithm::clean_word(QString & tmp) {
    delete_symbol(tmp);// убираем знаки препинания вначале и конце слова, если например: работа,  или .Цель
    tolower(tmp, std::locale("Russian")); // приводим к нижнему регистру
    tolower(tmp, std::locale("US"));

    if (!check_familiar_words(tmp)) {
        if (tmp.length() > 3) {
            return true;
        }
    }
    return false;
}

void Algorithm::fill_vec (QTextStream in, vector<QString>& key_words_vec) {
    while (!in.atEnd()) {
        QString tmp;
        in >> tmp;
        if (clean_word(tmp)) {
            key_words_vec.push_back(tmp);
        }
    }
}


void Algorithm::fill_set(QTextStream in, set<QString>& set_of_keywords) {
    while (!in.atEnd()) {
        QString tmp;
        in >> tmp;
        if (clean_word(tmp)) {
        set_of_keywords.insert(tmp);
        }
    }
}

//Сходство Жаккара
bool Algorithm::Jacar_alg(set<QString>& A, set<QString>& B) {
    vector <QString> dest1;
    vector <QString> dest2;

    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),
        std::back_inserter(dest1));
    std::set_union(A.begin(), A.end(),
        B.begin(), B.end(), std::back_inserter(dest2));

    if ((dest1.size() / dest2.size()) > 0,1) {
        return true;
    }

    return false;
}

bool Algorithm::Shingl_alg(vector <QString>& A, vector <QString>& B) {
    set <QString> set_A;
    set <QString> set_B;
    for (size_t i = 0; i < A.size() - 1; ++i) {
        QString tmp;
        tmp+= A[i] + ' ' + A[i + 1];
        set_A.insert(tmp);
    }
    for (size_t i = 0; i < B.size() - 1; ++i) {
        QString tmp;
        tmp+= B[i] + ' ' + B[i + 1];
        set_B.insert(tmp);
    }
    if (Jacar_alg(set_A, set_B)) {
        return true;
    }
    return false;
}


/*
int main() {

    ifstream in("cmake-build-debug/file1.txt");// базовый файл

    vector <icu::UnicodeString> base_vec;
    fill_vec(in, base_vec);

    set <icu::UnicodeString> base_set;
    fill_set(in, base_set);


    // берём файл из директории
    ifstream in_compare("cmake-build-debug/file2.txt");

    vector <icu::UnicodeString> compare_vec;
    fill_vec(in_compare, compare_vec);

    set <icu::UnicodeString> compare_set;
    fill_set(in_compare, compare_set);

    in.close();
    in_compare.close();

    // вызов функции для анализа схожести
    if (Jacar_alg(base_set, compare_set)) {
        cout << "Файлы схожи";
    }

    return 0;
}
*/
