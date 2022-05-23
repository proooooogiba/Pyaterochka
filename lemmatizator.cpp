#include <thread>

#include "lemmatizator.h"
#include <QFile>
#include <QTextStream>
#include <thread>
#include <QDir>
#include <QCoreApplication>
#include <QDirIterator>

Lemmatizator::Lemmatizator() {
    QString program = find_lem();
    if (program == QString{""}) {
        qDebug() << "can not find lemmatizator =(";
        throw std::exception("lemmatizator not found");
    } else {
        qDebug() << "lemmatizator found: " << program;
    }
    stem.setProgram(program);
    stem.setArguments(QStringList() << "--format=json");
}

Lemmatizator::~Lemmatizator() {
    stem.close();
}

QString Lemmatizator::find_lem(const QString where) {
    int max_current_processed = 3000;
    int max_parent_processed = 10000;
    QString program = current_lem;
    QDir current_path(where);

    int processed = 0;
    QDirIterator it(current_path.absolutePath(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo info = it.nextFileInfo();
        if (info.fileName() == program) {
            program = info.absoluteFilePath();
            break;
        }
        ++processed;
        if (processed > max_current_processed) {
            break;
        }
    }

    if (program != QString(current_lem)) {
        return program;
    }

    current_path.cdUp();
    processed = 0;
    QDirIterator it_parent(current_path.absolutePath(), QDir::Files, QDirIterator::Subdirectories);
    while (it_parent.hasNext()) {
        QFileInfo info = it_parent.nextFileInfo();
        if (info.fileName() == program) {
            program = info.absoluteFilePath();
            break;
        }
        ++processed;
        if (processed > max_parent_processed) {
            break;
        }
    }

    if (program != QString(current_lem)) {
        return program;
    }

    return QString{""};
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
