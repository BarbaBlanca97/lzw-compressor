#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QFile>
#include <QObject>

#include "dictionary.h"
#include "filewriter.h"
#include "compressionmachine.h"

class Compressor : public CompressionMachine
{
public:
    Compressor();

    void pause();
    void resume();
    void abort();
protected:
    void run();
private:
    void compressionAlgorithm();
};

#endif // COMPRESSOR_H
