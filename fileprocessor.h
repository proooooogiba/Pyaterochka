#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include "algorithm.h"

namespace FileProcessor
{
    void collectContents(QDir folder, QFileInfoList &files);
    bool compare_files(QFileInfo our_file_info, QFileInfo another_file_info, int percent);
};

#endif // FILEPROCESSOR_H
