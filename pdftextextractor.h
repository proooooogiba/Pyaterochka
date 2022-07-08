#ifndef PDFTEXTEXTRACTOR_H
#define PDFTEXTEXTRACTOR_H

#include <QString>
#include <QProcess>
#include <QFile>
#include <QFileInfo>

class PdfTextExtractor
{
    static const constexpr char* win_ext = "text_extraction_win.exe";
    static const constexpr char* linux_ext = "text_extraction_linux";

#if defined(Q_OS_WIN) || defined (Q_OS_WIN32)
    static const constexpr char* current_ext = win_ext;
#else
#if defined(Q_OS_LINUX)
    static const constexpr char* current_ext = linux_ext;
#else
    #error("Unsupported platform!")
#endif
#endif
//   public:
    //TODO: добавить возможность убирать/добавлять аргументы
//    QStringList stem_args = { "--format=json", NULL };
    QProcess extractor;

public:

    PdfTextExtractor();

    ~PdfTextExtractor();

    QString extract(const QString path);

    QString extract(const QFileInfo file);

    QByteArray extractb(const QString path);

    QByteArray extractb(const QFileInfo file);

private:
    static QString find_ext(const QString where = ".");
};

#endif // PDFTEXTEXTRACTOR_H
