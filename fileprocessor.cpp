#include "fileprocessor.h"
//#include "algorithm.h"

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

    //read_file

    QByteArray our_data, another_data;
        if (!our_file.open(QIODevice::ReadOnly) || !another_file.open(QIODevice::ReadOnly))
            //MessageBos::informatopn()
            //QMessageBox::warning(this, "Уведомление", "Файл" + our_file_info.filePath() + "не смог считаться");
            return false;

    our_data = our_file.readAll();
    another_data = our_file.readAll();
    //алгоритма Жакара и еще кого-то
    //
    if (false)
    {
        return true;
    }
    return true;
}
