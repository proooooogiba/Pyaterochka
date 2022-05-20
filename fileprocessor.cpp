#include "fileprocessor.h"

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

    our_file.open(QIODevice::ReadOnly | QFile::Truncate);
    another_file.open(QIODevice::ReadOnly | QFile::Truncate);

    Algorithm compare;

    QTextStream in_our(&our_file);
    QTextStream in_another(&another_file);
    //in_our --> in_our_lemmitize

    std::vector<QString> base_vec;
    compare.fill_vec(in_our, base_vec);

    in_our.seek(0);
    std::set<QString> base_set;
    compare.fill_set(in_our, base_set);

    std::vector<QString> compare_vec;
    compare.fill_vec(in_another, compare_vec);



    in_another.seek(0);
    std::set<QString> compare_set;
    compare.fill_set(in_another, compare_set);

    our_file.close();
    another_file.close();

    if (compare.Jacar_alg(base_set, compare_set))
    {

        return true;
    }

    return false;
}
