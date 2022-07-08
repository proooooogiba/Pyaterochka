#include <thread>

#include "lemmatizator.h"
#include <QFile>
#include <QTextStream>
#include <thread>
#include <QDir>
#include <QCoreApplication>
#include <QDirIterator>

Lemmatizator<QString> global_lem;

template <typename Type>
Lemmatizator<Type>::Lemmatizator() {
    Type program = findLemmatizator();
    if (program == Type{""}) {
        qDebug() << "can not find lemmatizator =(";
        throw;
    } else {
        qDebug() << "lemmatizator found: " << program;
    }
    stem.setProgram(program);
    stem.setArguments(QStringList() << "--format=json");
    bool success = initialize();
    qDebug() << "lematizator started successfully: " << success;
}

template <typename Type>
Lemmatizator<Type>::~Lemmatizator() {
    stem.close();
}

template <typename Type>
Type Lemmatizator<Type>::findLemmatizator(const Type where) {
    int max_current_processed = 3000;
    int max_parent_processed = 10000;
    Type program = current_lem;
    QDir current_path(where);

    int processed = 0;
    QDirIterator it(current_path.absolutePath(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo info = QFileInfo(it.next());
        if (info.fileName() == program) {
            program = info.absoluteFilePath();
            break;
        }
        ++processed;
        if (processed > max_current_processed) {
            break;
        }
    }

    if (program != Type(current_lem)) {
        return program;
    }

    current_path.cdUp();
    processed = 0;
    QDirIterator it_parent(current_path.absolutePath(), QDir::Files, QDirIterator::Subdirectories);
    while (it_parent.hasNext()) {
        QFileInfo info = QFileInfo(it_parent.next());
        if (info.fileName() == program) {
            program = info.absoluteFilePath();
            break;
        }
        ++processed;
        if (processed > max_parent_processed) {
            break;
        }
    }

    if (program != Type(current_lem)) {
        return program;
    }

    return QString{""};
}

template <typename Type>
bool Lemmatizator<Type>::is_initialized() {
    return initialized;
}

template <typename Type>
bool Lemmatizator<Type>::initialize() {
    stem.start();
    initialized = stem.waitForStarted();
    if (!initialized) {
        LOGGER << "FAILED TO RUN '" << current_lem << "'";
        LOGGER << "ERROR: " << stem.errorString();
    }
    return initialized;
}

template <typename Type>
Type Lemmatizator<Type>::lemmatizeString(const Type& content) {
    return lemmatizeBinary(content.toUtf8());
}

template <typename Type>
Type Lemmatizator<Type>::lemmatizeBinary(const QByteArray& content_) {
    if(!initialized) {
        return Type{};
    }
    stem.readAll();

    QByteArray content(content_.data(), content_.size());
    content.replace('\n', ' ');

    stem.write(content);
    stem.write("\r\n");
    stem.waitForBytesWritten();
    QByteArray return_content;

    while (return_content.size() == 0 || return_content.count('[') != return_content.count(']')) {

//=========================================================
// САМОЕ ВАЖНОЕ ˅˅˅˅˅˅˅˅˅˅˅˅˅˅˅˅˅˅˅
        stem.waitForReadyRead(); // <<<<<<<<<<<<<<<<<<<<<<<
// САМОЕ ВАЖНОЕ ^^^^^^^^^^^^^^^^^^
//=========================================================

        QByteArray bts = stem.readAll();
        return_content.append(bts);
    }

    return parseRecivedContent(return_content);
}

template <typename Type>
Type Lemmatizator<Type>::parseRecivedContent(const Type& content) {
    std::string std_content = content.toStdString();
    json json_content;

    try {
        json_content = json::parse(std_content);
    } catch (std::exception e) {
        LOGGER << "ERROR WHILE PARSING: " << e.what();
    }

    std::string return_content{};

    for (auto& analysis_obj : json_content) {
        if (analysis_obj["analysis"].size() != 0) {
            std::string lex = analysis_obj["analysis"][0]["lex"];
            return_content.append(lex);
        } else {
            std::string word = analysis_obj["text"];
            return_content.append(word);
        }
        return_content.push_back(' ');
    }

    return Type(return_content.data());
}

template class Lemmatizator<QString>;
