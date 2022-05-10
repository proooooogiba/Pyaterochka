#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //TO DO: изменить иконку приложения
    ui->setupUi(this);
    ui->listWidget->sizeHintForColumn(30);

    ui->pushButton_5->setIcon(QIcon("C:/Users/Komp/Desktop/Icon/Trash_bin.png"));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotFindFiles()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotFindFiles()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectFile()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(slotDeleteItem()));
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
    //TO DO: настроить View mode
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
    //TODO: QProgress Dialog
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        AddToFolder(ui->lineEdit_3->text(), ui->lineEdit_4->text(), ui->listWidget->item(i)->text());
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

void MainWindow::AddToFolder(QString file_path, QString folder_name, QString file_name)
{
    QDir dir(file_path + "/" + folder_name);
    // если такой папки в указанной директории не сущствует, то создает новую папку с указанным именем
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    // копирование файлов пока не работает
    if (!QFile::copy(file_name, file_path + "/" + folder_name))
    {
        QMessageBox::warning(this, "Ошибка", file_name + " не смог скопироваться");
        return;
    }
    return;
}


