#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include "algorithm.h"
#include "lemmatizator.h"

namespace FileProcessor
{
    void collectContents(QDir folder, QFileInfoList &files);
    void fill_vector(std::vector<QString> &vector, QFile &file, Lemmatizator<> &lem, Algorithm<QString> &compare);
    bool compare_files(QFileInfo our_file_info, QFileInfo another_file_info, float percent);
};

#endif // FILEPROCESSOR_H
