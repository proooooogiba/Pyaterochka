#include <thread>

#include "lemmatizator.h"
#include <QFile>
#include <QTextStream>
#include <thread>

Lemmatizator::Lemmatizator() {
    QString program = R"(C:\projects\build-untitled6-Desktop_Qt_6_3_0_MSVC2019_64bit-Debug\debug\mystem_windows.exe)";
    stem.setProgram(program);
    stem.setArguments(QStringList() << "--format=json" << "-e" <<  "cp866");
}

Lemmatizator::~Lemmatizator() {

}

bool Lemmatizator::is_initialized() {
    return initialized;
}

bool Lemmatizator::initialize() {
    stem.start();
    initialized = stem.waitForStarted();
    LOGGER << "stem started: " << initialized;
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
        using std::chrono_literals::operator""ms;
        std::this_thread::sleep_for(500ms);
//    }
    LOGGER << "ready read: " << stem.waitForReadyRead();

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

QString Lemmatizator::parseRecivedContent(const QString& content) {
//    return content;
    LOGGER << "start parsing";

//    QFile out("D:\\out.txt");
//    out.open(QFile::WriteOnly | QFile::Text);
//    QTextStream out_stream(&out);

//    out_stream << "content: " << content << "\n";
//    out_stream << "content length: " << content.length();

//    out_stream.flush();
//    out.close();

    std::string std_content = content.toStdString();
    json json_content;

//    try {
        json_content = json::parse(std_content);
//    } catch (std::exception e) {
//        LOGGER << e.what();
//    }

    std::string return_content{};
    LOGGER << "still parsing";

    for (auto& analysis_obj : json_content) {
        LOGGER << analysis_obj.dump().c_str();
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

    LOGGER << "end parsing";
    return QString(return_content.data());
}
