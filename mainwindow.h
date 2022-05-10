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

#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();
    void slotFindFiles();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
