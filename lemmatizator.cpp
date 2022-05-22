#include <thread>

#include "lemmatizator.h"
#include <QFile>
#include <QTextStream>
#include <thread>
#include <QDir>
#include <QCoreApplication>

Lemmatizator::Lemmatizator() {
    QString program = current_lem;
    if (QDir(QCoreApplication::applicationDirPath()).exists(current_lem)) {
        program = QDir(QCoreApplication::applicationDirPath() + current_lem).absolutePath();
    } else if (QDir("./").exists(current_lem)) {
        program = QDir(QString("./") + current_lem).absolutePath();
    } else if (QDir("../").exists(current_lem)) {
        program = QDir(QString("../") + current_lem).absolutePath();
    } else if (QDir("../../").exists(current_lem)) {
        program = QDir(QString("../../") + current_lem).absolutePath();
    } else {
        throw std::exception("lemmatizator not found");
    }

//    program = R"(C:\projects\build-untitled6-Desktop_Qt_6_3_0_MSVC2019_64bit-Debug\debug\mystem_windows.exe)";
    stem.setProgram(program);
//    stem.setArguments(QStringList() << "--format=json" << "-e" <<  "cp866");
    stem.setArguments(QStringList() << "--format=json");
}

Lemmatizator::~Lemmatizator() {
    stem.close();
}

bool Lemmatizator::is_initialized() {
    return initialized;
}

bool Lemmatizator::initialize() {
    stem.start();
    initialized = stem.waitForStarted();
    if (!initialized) {
        LOGGER << "FAILED TO RUN '" << current_lem << "'";
        LOGGER << "ERROR: " << stem.errorString();
    }
    return initialized;
}

QString Lemmatizator::lemmatize(const QString& content) {
    if(!initialized) {
        // throw UninitializedLem();
        return QString{};
    }

    QString return_content{};
//    return_content.resize(content.size() * 2);

    LOGGER << "start write to stem";

//    stem.write(qPrintable(content.toLocal8Bit()));
    stem.write(content.toLocal8Bit().data());
//    stem.write("привет\n");
    LOGGER << content;
    LOGGER << content.size();
    stem.write("\r\n");
    stem.waitForBytesWritten();

    LOGGER << "end write to stem";

    LOGGER << "start read from stem";

    QFile out("D:\\out.txt");
    out.open(QFile::WriteOnly | QFile::Text);
    QTextStream out_stream(&out);

//    while (!stem.canReadLine()) {
//        LOGGER << "wait";

//    }
    LOGGER << "ready read: " << stem.waitForReadyRead();

    using std::chrono_literals::operator""ms;
    std::this_thread::sleep_for(500ms);

    QByteArray arr = stem.readAll();
    return_content.append(QString::fromLocal8Bit(arr));

    LOGGER << "array: " << arr;
    LOGGER << "array length: " << arr.size();
    LOGGER << "read from stem: " << return_content;

    out_stream.flush();
    out.close();

//        if (return_content.count('[') ==
//            return_content.count(']')) {
//            break;
//        }
//    }

    return_content = return_content.trimmed();

    LOGGER << "end read from stem";
    // LOGGER << "content: <" << content.data() << ">  size=" << content.size() << "\n";
    // LOGGER << "sended: " << w << "   recived: " << total_read << "\n";
    // LOGGER << "recived content data: " << return_content.data() << "\n";
    // LOGGER << "recived content: " << return_content << "\n";
    // LOGGER << "recived content size: " << return_content.size() << "\n\n\n";

    return parseRecivedContent(return_content);
}

QString Lemmatizator::lemmatizeb(QByteArray& content) {
    if(!initialized) {
        return QString{};
    }
    stem.readAll();

    content.replace('\n', ' ');

//    LOGGER << "write: " << QString(content);
    stem.write(content);
    stem.write("\r\n");
    stem.waitForBytesWritten();

//    stem.waitForReadyRead();


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
//    qDebug() << "amount [ = " << return_content.count('[');
//    qDebug() << "amount ] = " << return_content.count(']');


    return parseRecivedContent(return_content);
}

QString Lemmatizator::parseRecivedContent(const QString& content) {
//    return content;
//    LOGGER << "start parsing";
//    LOGGER << "content: " << content;

//    QFile out("D:\\out.txt");
//    out.open(QFile::WriteOnly | QFile::Text);
//    QTextStream out_stream(&out);

//    out_stream << "content: " << content << "\n";
//    out_stream << "content length: " << content.length();

//    out_stream.flush();
//    out.close();

    std::string std_content = content.toStdString();
    json json_content;

    try {
        json_content = json::parse(std_content);
    } catch (std::exception e) {
        LOGGER << "ERROR WHILE PARSING: " << e.what();
//        LOGGER << "content: " << std_content.c_str();
    }

    std::string return_content{};
//    LOGGER << "still parsing";

    for (auto& analysis_obj : json_content) {
//        LOGGER << analysis_obj.dump().c_str();
        if (analysis_obj["analysis"].size() != 0) {
            std::string lex = analysis_obj["analysis"][0]["lex"];
            return_content.append(lex);
        } else {
            std::string word = analysis_obj["text"];
            return_content.append(word);
        }
        return_content.push_back(' ');
    }

//    if (*return_content.end() == ' ') {
//        return_content.resize(return_content.size() - 1);
//    }

//    LOGGER << "end parsing";
    return QString(return_content.data());
}
