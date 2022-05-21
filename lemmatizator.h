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

class Lemmatizator{
    static constexpr std::string_view win_lem = "mystem_windows.exe";
    static constexpr std::string_view linux_lem = "mystem_linux";

#if defined(_WIN32) || defined (__WIN32)
    static constexpr std::string_view current_lem = Lemmatizator::win_lem;
#else
#if defined(__linux__)
    static constexpr std::string_view current_lem = Lemmatizator::linux_lem;
#else
    #error("Unsupported platform!")
#endif
#endif
//   public:
    //TODO: добавить возможность убирать/добавлять аргументы
    QStringList stem_args = { "--format=json", NULL };
    QProcess stem;
    bool initialized = false;

  public:
    Lemmatizator();

    ~Lemmatizator();

    bool is_initialized();

    bool initialize();

    QString lemmatize(const QString& content);

  private:

    QString parseRecivedContent(const QString& content);

    // class UninitializedLem : std::exception {}
};


#endif // LEMMATIZATOR_H
