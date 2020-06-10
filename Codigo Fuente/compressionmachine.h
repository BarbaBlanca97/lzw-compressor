#ifndef COMPRESSIONMACHINE_H
#define COMPRESSIONMACHINE_H

#include <QFile>

#include <QThread>

class CompressionMachine
        : public QThread
{
Q_OBJECT

public:
    CompressionMachine();
    ~CompressionMachine();

protected:
    QFile
        *input,
        *output;

    QString fileExtension;

    bool paused, abortRequested, ready;

    virtual void run() = 0;

public:
    void setUp(QString inputPath, QString outputPath, QString targetFileExtension);

    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void abort() = 0;

    bool isPaused();

signals:
    void jobDone();
    void progress(int percentage);
    void error(QString stringError);
    void readyToClose();
};

#endif // COMPRESSIONMACHINE_H
