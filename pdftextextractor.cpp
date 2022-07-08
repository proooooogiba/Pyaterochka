#include "pdftextextractor.h"
#include <QDebug>
#include <QDir>
#include <QDirIterator>

PdfTextExtractor global_pdf_extractor;

PdfTextExtractor::PdfTextExtractor() {
    QString program = find_ext();
    if (program == QString{""}) {
        qDebug() << "can not find pdf text expractor =(";
        throw;
    } else {
        qDebug() << "pdf text expractor found: " << program;
    }
    extractor.setProgram(program);
}

PdfTextExtractor::~PdfTextExtractor() {
    extractor.close();
}

QString PdfTextExtractor::find_ext(const QString where) {
    int max_current_processed = 3000;
    int max_parent_processed = 10000;
    QString program = current_ext;
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

    if (program != QString(current_ext)) {
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

    if (program != QString(current_ext)) {
        return program;
    }

    return QString{""};
}

QString PdfTextExtractor::extract(const QString path) {
    return extract(QFileInfo(path));
}

QString PdfTextExtractor::extract(const QFileInfo file_) {

    QString file;
    file = file_.absoluteFilePath();


    extractor.setArguments(QStringList() << file);
    extractor.start();

    extractor.waitForStarted();
    extractor.waitForReadyRead();
    extractor.waitForFinished();

    QByteArray return_bytes = extractor.readAll();
    extractor.close();
    return QString::fromUtf8(return_bytes);
}

QByteArray PdfTextExtractor::extractb(const QString path) {
    return extractb(QFileInfo(path));
}

QByteArray PdfTextExtractor::extractb(const QFileInfo file) {
    extractor.setArguments(QStringList() << file.absoluteFilePath());

    extractor.start();

    extractor.waitForStarted();
    extractor.waitForReadyRead();
    extractor.waitForFinished();

    QByteArray return_bytes = extractor.readAll();
    extractor.close();
    return return_bytes;
}
