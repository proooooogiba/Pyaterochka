#ifndef LEMMATIZATOR_H
#define LEMMATIZATOR_H

#include "json.hpp"
#include <iostream>
#include <thread>
#include <QStringList>
#include <QProcess>
#include <QDebug>

using json = nlohmann::json;

#define LOGGER qDebug()

template<typename Type = QString>
class Lemmatizator{
    static const constexpr char* win_lem = "mystem_windows.exe";
    static const constexpr char* linux_lem = "mystem_linux";


#if defined(Q_OS_WIN) || defined (Q_OS_WIN32)
    static const constexpr char* current_lem = Lemmatizator::win_lem;
#else
#if defined(Q_OS_LINUX)
    static const constexpr char* current_lem = Lemmatizator::linux_lem;
#else
    #error("Unsupported platform!")
#endif
#endif
    QProcess stem;
    bool initialized = false;

  public:

    Lemmatizator();

    ~Lemmatizator();

    bool is_initialized();

    bool initialize();

    Type lemmatizeString(const Type& content);

    Type lemmatizeBinary(const QByteArray& content_);

  private:

    Type parseRecivedContent(const Type& content);

    static Type findLemmatizator(const Type where = ".");

    // class UninitializedLem : std::exception {}
};


#endif // LEMMATIZATOR_H
