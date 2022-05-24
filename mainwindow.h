#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStyle>
#include <QTableWidgetItem>
#include <QColor>
#include <QStyleFactory>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QDir>
#include <QProgressDialog>
#include <QThread>
#include <QTextStream>
#include <QProcess>
#include <QListWidgetItem>

#include "fileprocessor.h"


//#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddToFolder(QString file_path, QString file_name);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();
    void slotFindFiles();
    void slotSelectFile();
    void slotDeleteItem();

    void on_pushButton_6_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);


    void on_horizontalSlider_valueChanged(int value);

signals:
    void collect_contents();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
