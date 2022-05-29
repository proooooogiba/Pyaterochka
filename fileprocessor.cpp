#include "fileprocessor.h"
#include "pdftextextractor.h"

#include <QCoreApplication>

extern Lemmatizator global_lem;
extern PdfTextExtractor global_pdf_extractor;

void FileProcessor::collectContents(QDir folder, QFileInfoList &files)
{
    for (QFileInfo info :
         folder.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst))
    {
        if (info.isFile() && (info.suffix() == "txt" || info.suffix() == "pdf"))
        {
            files.append(info);
            continue;
        }

        if (info.isDir() && folder.cd(info.fileName()))
        {
            collectContents(folder, files);
            folder.cdUp();
        }
    }
}

bool FileProcessor::compare_files(QFileInfo our_file_info, QFileInfo another_file_info, int percent)
{
    qDebug() << "{";
    qDebug() << "\"our\": " << our_file_info.fileName() << ",\n\"another\":" <<  another_file_info.fileName() << ",";

    QFile our_file(our_file_info.filePath());
    QFile another_file(another_file_info.filePath());
    our_file.open(QFile::ReadOnly);
    another_file.open(QFile::ReadOnly);

    Lemmatizator& lem = global_lem;
    if (!lem.is_initialized()) {
        bool success = lem.initialize();
        qDebug() << "lematizator started successfully: " << success;
    }

    Algorithm compare;

    std::vector<QString> base_vector;
    if (QFileInfo(our_file).suffix() == "txt") {
            compare.fill_vector_lemmatize(our_file, base_vector, lem);
    } else if (QFileInfo(our_file).suffix() == "pdf") {
            compare.fill_vector_lemmatize(
                        global_pdf_extractor.extractb(QFileInfo(our_file)),
                        base_vector,
                        lem);
    } else {
        throw;
    }

    std::vector<QString> compare_vector;
    if (QFileInfo(another_file).suffix() == "txt") {
            compare.fill_vector_lemmatize(another_file, compare_vector, lem);
    } else if (QFileInfo(another_file).suffix() == "pdf") {
            compare.fill_vector_lemmatize(
                        global_pdf_extractor.extractb(QFileInfo(another_file)),
                        compare_vector,
                        lem);
    } else {
        throw;
    }

    our_file.close();
    another_file.close();

//    qDebug() << "started Jacar alg";
//    bool result = compare.Jacar_alg(base_set, compare_set, percent);
//    qDebug() << "end of Jacar alg, result: " << result;
//    qDebug() << ">>>=====================<<<\n\n";
//    qDebug() << " ";

//    qDebug() << "started Shingl alg";
    bool result = compare.Shingl_alg(base_vector, compare_vector, percent);
//    qDebug() << "end of Shingl alg, result: " << result;
    qDebug() << "},";


    return result;
}
