#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileprocessor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont("C:/Users/Komp/Desktop/fonts/OpenSans-VariableFont_wdth,wght.ttf");
    ui->listWidget->setItemAlignment(Qt::AlignmentFlag::AlignRight);
    ui->listWidget->sizeHintForColumn(30);

    ui->horizontalSlider->setValue(9);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setMinimum(1);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotFindFiles()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotFindFiles()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectFile()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(slotDeleteItem()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged()), this, SLOT(setNum()));
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
    if (str.isEmpty()) {
        return;
    }
    ui->lineEdit->setText(str);
    ui->lineEdit->setAlignment(Qt::AlignLeft);
    ui->lineEdit->alignment();
}


void MainWindow::on_pushButton_2_clicked()
{
    QString str;

    str = QFileDialog::getOpenFileName(this, "Выбрать файл", ui->lineEdit->text(),
                                       "Все поддерживаемые форматы (*.pdf;*.txt);;"
                                       "Текстовые файлы (*.txt);;"
                                       "Формат PDF (*.pdf)");

    ui->lineEdit_2->setReadOnly(true);
    if (str.isEmpty()) {
        return;
    }
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
    ui->listWidget->clear();
    ui->label_9->setNum(0);

    QDir search_directory(ui->lineEdit->text());
    QFileInfoList file_list;

    //поисковый алгоритм
    FileProcessor::collectContents(search_directory, file_list);
    QFileInfo our_file(ui->lineEdit_2->text());


    QProgressDialog progress("Поиск схожих файлов по содержанию", "Прервать поиск", 0,  file_list.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle("Окно прогресса");
    for (int i = 0; i < file_list.size(); ++i) {
        progress.setValue(i);
        if (file_list.at(i) == our_file) {
            continue;
        }
        if (FileProcessor::compare_files(our_file, file_list.at(i), ui->horizontalSlider->value())) {
            ui->listWidget->addItem(file_list.at(i).filePath());
        }
    }
    if (progress.wasCanceled()) {
        QMessageBox::warning(this, "Ошибка", "Что-то пошло не так - процесс был прерван");
    } else if (ui->listWidget->count() == 0) {
        QMessageBox::information(this, "Уведомление", "Не найдены похожие файлы, попробуйте уменьшить коеффициент схожести");
        ui->label_9->setNum(0);
    } else {
        QMessageBox::information(this, "Уведомление", "Алгоритм успешно нашел схожие файлы");
        //ui->label_9->setNum(ui->listWidget->size());
        ui->label_9->setNum(ui->listWidget->count());
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    if (ui->listWidget->count() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Должен быть хотя бы один файл");
        return;
    }

    QProgressDialog progress("Формирование папки", "Прервать формирование", 0,  ui->listWidget->count(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle("Окно прогресса");
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        AddToFolder(ui->lineEdit_3->text(), ui->listWidget->item(i)->text());
        progress.setValue(i);
    }
    if (progress.wasCanceled()) {
        QMessageBox::warning(this, "Ошибка", "Что-то пошло не так - процесс был прерван");
    } else {
        QMessageBox::information(this, "Уведомление", "Формирование папки прошло успешно");
    }
}

void MainWindow::slotSelectFile()
{
    ui->lineEdit_5->setText(ui->listWidget->currentItem()->text());
}

void MainWindow::slotDeleteItem()
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }

    if (ui->listWidget->count() == 1)
    {
        ui->listWidget->clear();
        ui->lineEdit_5->clear();
        return;
    }

    QListWidgetItem *it = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete it;
}

void MainWindow::AddToFolder(QString file_path, QString file_name)
{
    QDir dir(file_path);
    // если такой папки в указанной директории не существует, то создает новую папку с указанным именем
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    QFileInfo fileInfo(file_name);
    QString filename(fileInfo.fileName());

    QString dstPath = file_path + "/" + filename;

    if (QFile::exists(dstPath)) {
        QFile::remove(dstPath);
    }

    QFile::copy(file_name, dstPath);
    return;
}



void MainWindow::on_pushButton_6_clicked()
{
    ui->listWidget->clear();
    ui->lineEdit_5->clear();
    ui->label_9->setNum(0);
    return;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "dclick: " << item->text();
    QProcess proc(this);
    proc.setProgram("start");
    proc.setArguments(QStringList() << item->text());
    proc.start();
    qDebug() << "started: " << proc.waitForStarted();
    qDebug() << "error: " <<proc.errorString();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_10->setNum(value);
    ui->label_10->setText(QString::number(value) + "%");
}

