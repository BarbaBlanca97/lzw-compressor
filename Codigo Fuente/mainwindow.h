#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

#include "decompressor.h"
#include "compressor.h"

enum FileStatus{ NotSelected, Compressed, Normal };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    CompressionMachine *currentMachine;
    Decompressor decompressor;
    Compressor compressor;

    bool working;

    FileStatus currentFileStatus;

    QFileInfo currentFile;
    QString buttonPlayLabel;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void startWork();
    void openFile();
    void requestInterruption();

    void onProgress(int percentage);

    void decompressionDone();
    void compressionDone();
    void threadAborted();

    void onError(QString stringError);
};

#endif // MAINWINDOW_H
