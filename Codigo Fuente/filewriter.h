#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <vector>
#include <QByteArray>
#include <QFile>
#include "simple_casts.h"

using namespace std;

class FileWriter
{
private:
    QFile *file;
    QByteArray buffer, aux;
    uint entryBuffer;
    int placesTaken;
    int bufferSize;
    int lastBitSize;
public:
    FileWriter(QFile *file, int bufferSize);

    void put(uint index, int bit_size);
    void close();
private:
    void flush();
};

#endif // FILEWRITER_H
