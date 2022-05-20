#ifndef LEMMATIZATOR_H
#define LEMMATIZATOR_H

#include "json.hpp"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <thread>

using json = nlohmann::json;

#define LOGGER qDebug()

class Lemmatizator{
    static constexpr std::string_view win_lem = "mystem_win.exe";
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
    std::vector<const char *> stem_args = { current_lem.data(), "--format=json", NULL };
    int to_stem[2];
    int from_stem[2];
    bool initialized = false;
    int stem_pid = -1;

  public:
    Lemmatizator();

    ~Lemmatizator();

    bool is_initialized();

    bool initialize();

    std::string lemmatize(const std::string& content);

  private:

    std::string parseRecivedContent(const std::string& content);

    void configFromStemOut();

    // class UninitializedLem : std::exception {}
};


#endif // LEMMATIZATOR_H
