#include <QElapsedTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | (Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint) & ~Qt::WindowContextHelpButtonHint);

    working = false;
    currentFileStatus = FileStatus::NotSelected;

    QObject::connect(ui->buttonPlatPause, &QPushButton::clicked, this, &MainWindow::startWork);
    QObject::connect(ui->buttonAddFile, &QPushButton::clicked, this, &MainWindow::openFile);
    QObject::connect(ui->buttonAbort, &QPushButton::clicked, this, &MainWindow::requestInterruption);

    QObject::connect(&compressor, &Compressor::jobDone, this, &MainWindow::compressionDone);
    QObject::connect(&decompressor, &Decompressor::jobDone, this, &MainWindow::decompressionDone);

    QObject::connect(&compressor, &Compressor::progress, this, &MainWindow::onProgress);
    QObject::connect(&decompressor, &Decompressor::progress, this, &MainWindow::onProgress);

    QObject::connect(&compressor, &Compressor::error, this, &MainWindow::onError);
    QObject::connect(&decompressor, &Decompressor::error, this, &MainWindow::onError);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startWork()
{
    if(working)
    {
        if(!currentMachine->isPaused()){
            currentMachine->pause();
            ui->buttonPlatPause->setText(buttonPlayLabel);
        } else {
            currentMachine->resume();
            ui->buttonPlatPause->setText("PAUSAR");
        }

        return;
    }

    currentFile.refresh();
    if(!currentFile.exists()){
        QMessageBox::warning(
                    this,
                    tr("Error"),
                    tr("No se pudo abrir el archivo")
        );
        return;
    }

    working = true;

    switch (currentFileStatus) {
    case FileStatus::NotSelected: {
        return;
    }
    case FileStatus::Compressed: {
        decompressor.setUp(currentFile.absoluteFilePath(), currentFile.absoluteDir().absolutePath() + "/" + currentFile.baseName(), "");
        decompressor.start();
        currentMachine = &decompressor;
        break;
    }
    case FileStatus::Normal: {
        compressor.setUp(currentFile.absoluteFilePath(), currentFile.absoluteDir().absolutePath() + "/" + currentFile.baseName() + ".lzw", currentFile.suffix());
        compressor.start();
        currentMachine = &compressor;
        break;
    }
    }

    ui->buttonPlatPause->setText("PAUSAR");
}

void MainWindow::openFile()
{
    if(working) return;

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QDir::home());

    QString filePath;
    if(dialog.exec())
        filePath = dialog.selectedFiles()[0];
    else {
        currentFileStatus = FileStatus::NotSelected;
        return;
    }

    currentFile = QFileInfo(filePath);
    if(currentFile.suffix() == "lzw")
    {
        currentFileStatus = FileStatus::Compressed;
        ui->buttonPlatPause->setText("DESCOMPRIMIR");
        buttonPlayLabel = "DESCOMPRIMIR";
    }
    else
    {
        currentFileStatus = FileStatus::Normal;
        ui->buttonPlatPause->setText("COMPRIMIR");
        buttonPlayLabel = "COMPRIMIR";
    }

    ui->labelFile->setText(currentFile.fileName());
}

void MainWindow::onProgress(int percentage)
{
    ui->progressBar->setValue(percentage);
}

void MainWindow::decompressionDone()
{
    working = false;
    ui->progressBar->setValue(0);
    ui->buttonPlatPause->setText(buttonPlayLabel);
    QMessageBox::information(
                this,
                tr("Info"),
                tr("Descompresion finalizada")
                );
}

void MainWindow::compressionDone()
{
    working = false;
    ui->progressBar->setValue(0);
    ui->buttonPlatPause->setText(buttonPlayLabel);
    QMessageBox::information(
                this,
                tr("Info"),
                tr("Compresion finalizada")
                );
}

void MainWindow::requestInterruption()
{
    if(currentMachine != nullptr && working)
        currentMachine->abort();
}

void MainWindow::threadAborted()
{
    working = false;
    ui->buttonPlatPause->setText(buttonPlayLabel);
    ui->progressBar->setValue(0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(working)
    {
        currentMachine->abort();
        QElapsedTimer t; t.start();

        while(t.elapsed() < 1000);

        event->accept();
    }

    event->accept();
}

void MainWindow::onError(QString stringError){
    QMessageBox::warning(
                this,
                tr("Error"),
                stringError
    );
}
