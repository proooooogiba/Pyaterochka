#include "fileprocessor.h"
#include "pdftextextractor.h"

#include <QCoreApplication>

extern Lemmatizator<> global_lem;
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

void FileProcessor::fill_vector(std::vector<QString> &vector, QFile &file, Lemmatizator<> &lem, Algorithm<QString> &compare) {
    if (QFileInfo(file).suffix() == "txt") {
            compare.fillVectorLemmatize(file, vector, lem);
    } else if (QFileInfo(file).suffix() == "pdf") {
            compare.fillVectorLemmatize(
                        global_pdf_extractor.extractb(QFileInfo(file)),
                        vector,
                        lem);
    } else {
        throw;
    }
}

bool FileProcessor::compare_files(QFileInfo our_file_info, QFileInfo another_file_info, float percent)
{
    qDebug() << "{";
    qDebug() << "\"our\": " << our_file_info.fileName() << ",\n\"another\":" <<  another_file_info.fileName() << ",";

    QFile our_file(our_file_info.filePath());
    QFile another_file(another_file_info.filePath());
    our_file.open(QFile::ReadOnly);
    another_file.open(QFile::ReadOnly);

    Lemmatizator<>& lem = global_lem;
    if (!lem.is_initialized()) {
        bool success = lem.initialize();
        qDebug() << "lematizator started successfully: " << success;
    }

    Algorithm<QString> compare;

    std::vector<QString> base_vector;
    std::vector<QString> compare_vector;

    fill_vector(base_vector, our_file, lem, compare);
    fill_vector(compare_vector, another_file, lem, compare);

    our_file.close();
    another_file.close();

    bool result = compare.shinglAlg(base_vector, compare_vector, percent);
    qDebug() << "},";

    return result;
}
