#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <QFile>

#include "compressionmachine.h"
#include "filereader.h"
#include "dictionary.h"

class Decompressor
        : public CompressionMachine
{   
public:
    Decompressor();

    void pause();
    void resume();
    void abort();

protected:
    void run();

private:
    void decompressionAlgorithm();
};

#endif // DECOMPRESSOR_H
