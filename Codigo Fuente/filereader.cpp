#include "filereader.h"

FileReader::FileReader(QFile *file)
{
    this->file = file;

    entryBuffer = 0;
    placesTaken = 0;
}

ushort FileReader::nextIndex(int bitSize){
    ushort output = 0;
    quint32 aux = 0;

    if(placesTaken >= bitSize){
        output = ushort(entryBuffer >>  (32 - bitSize));
        entryBuffer = entryBuffer << bitSize;

        placesTaken -= bitSize;
    } else {
        char input[2];
        qint64 readed = file->read(input, 2);

        if(readed == 1){
            aux = quint32(input[0]) << 24;
        } else {
            quint32 tmp = 0; tmp |= uchar(input[0]); tmp <<= 8;
            aux |= uchar(input[1]);
            aux |= tmp;
            aux <<= (16 - placesTaken);
        }

        entryBuffer |= aux;
        quint32 aux2 = entryBuffer >> (32 - bitSize);
        output = ushort(aux2);
        entryBuffer = (aux << bitSize);

        placesTaken  += (16 - bitSize);
    }

    return output;
}
