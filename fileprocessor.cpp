#include "fileprocessor.h"
#include "lemmatizator.h"

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

    Algorithm compare;

    QTextStream in_our(&our_file);
    QTextStream in_another(&another_file);

//    std::vector<QString> base_vec;
//    compare.fill_vec(in_our, base_vec);

    in_our.seek(0);
    std::set<QString> base_set;
    compare.fill_set(in_our, base_set);

//    std::vector<QString> compare_vec;
//    compare.fill_vec(in_another, compare_vec);

    in_another.seek(0);
    std::set<QString> compare_set;
    compare.fill_set(in_another, compare_set);

    our_file.close();
    another_file.close();


    std::set<QString> compare_set_new;
    std::set<QString> base_set_new;

    Lemmatizator lem;

    bool success = lem.initialize();
    qDebug() << "lem started successfully: " << success;

    for (QString word : compare_set) {
//        qDebug() << "word: " << word;
        compare_set_new.insert(lem.lemmatize(word));
    }
    qDebug() << "compare_set processed";

    for (QString word : base_set) {
        base_set_new.insert(lem.lemmatize(word));
    }
    qDebug() << "base_set processed";

    qDebug() << "start Jacar alg";
    if (compare.Jacar_alg(base_set_new, compare_set_new))
    {
        return true;
    }

    return false;
}
