#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->listWidget->setRowCount(2);
    //ui->listWidget->setColumnCount(1);

    //ui->listWidget->setVerticalHeaderLabels(QStringList());
    //ui->listWidget->setStyle(QStyleFactory::create("Fusion"));

//    for (int i = 0; i < ui->listWidget->rowCount(); ++i) {
//        ui->listWidget->setRowHeight(i, 10);
//    }
//    ui->listWidget->setColumnWidth(0, 241);

    //ui->tableWidget->setColumnWidth(1, 10);
    ui->listWidget->sizeHintForColumn(30);

    ui->pushButton_5->setIcon(QIcon("C:/Users/Komp/Desktop/Icon/Trash_bin.png"));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotFindFiles()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotFindFiles()));
    //connect(ui->listWidget, SIGNAL(), this, SLOT());
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString str;
    str = QFileDialog::getExistingDirectory(this, "Выбрать директорию", "C:\\");
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setText(str);
    ui->lineEdit->setAlignment(Qt::AlignLeft);
    ui->lineEdit->alignment();
}


void MainWindow::on_pushButton_2_clicked()
{
    QString str;
    str = QFileDialog::getOpenFileName(this, "Выбрать файл", "C:\\",
                                       "All Files (*.*);; Текстовые файлы (*.txt);; Формат PDF (*.pdf)");
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_2->setAlignment(Qt::AlignLeft);
    ui->lineEdit_2->setText(str);
}


void MainWindow::on_pushButton_4_clicked()
{
    QString str;
    str = QFileDialog::getExistingDirectory(this, "Выбрать директорию", "C:\\");
    ui->lineEdit_3->setReadOnly(true);
    ui->lineEdit_3->setAlignment(Qt::AlignLeft);
    ui->lineEdit_3->setText(str);
}



void MainWindow::slotFindFiles()
{
    //алгоритм запукается только в том случае, если line едиты оказались не пусты
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        return;
    }
    //ui->listWidget->addItem(new QListWidgetItem())

    ui->listWidget->addItem(ui->lineEdit_2->text());
}


void MainWindow::on_pushButton_3_clicked()
{
    if (ui->lineEdit_4->text().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Название для новой папки не может быть пустым");
        return;
    }
    // по сути копирования всех элементов string grid в список
    if (ui->listWidget->count() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Должен быть хотя бы один файл");
        return;
    }
    // QProgress Dialog
}

