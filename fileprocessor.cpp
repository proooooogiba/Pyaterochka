#include "fileprocessor.h"
#include "lemmatizator.h"

#include <QCoreApplication>

void FileProcessor::collectContents(QDir folder, QFileInfoList &files)
{
    for (QFileInfo info :
         folder.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst))
    {
        if (info.isFile() && info.suffix() == "txt")
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

bool FileProcessor::compare_files(QFileInfo our_file_info, QFileInfo another_file_info)
{
    QFile our_file(our_file_info.filePath());
    QFile another_file(another_file_info.filePath());

    our_file.open(QIODevice::ReadOnly);
    another_file.open(QIODevice::ReadOnly);

    Lemmatizator lem;
    bool success = lem.initialize();
    qDebug() << "lematizator started successfully: " << success;
//    qDebug() << QCoreApplication::applicationDirPath();

    Algorithm compare;

//    std::vector<QString> base_vec;
//    compare.fill_vec(in_our, base_vec);

    std::set<QString> base_set;
    compare.fill_set_lemmatize(our_file, base_set, lem);

//    std::vector<QString> compare_vec;
//    compare.fill_vec(in_another, compare_vec);

    std::set<QString> compare_set;
    compare.fill_set_lemmatize(another_file, compare_set, lem);

    our_file.close();
    another_file.close();

    qDebug() << "started Jacar alg";
    bool result = compare.Jacar_alg(base_set, compare_set);
    qDebug() << "end of Jacar alg, result: " << result;
    qDebug() << ">>>=====================<<<\n\n";

    return result;
}
