#include "filewriter.h"

FileWriter::FileWriter(QFile *file, int bufferSize)
{
    placesTaken = 0;
    entryBuffer = 0;
    lastBitSize = 0;
    this->bufferSize = bufferSize;

    this->file = file;
}

void FileWriter::put(uint index, int bitSize) {
    int shiftAmount = (32 - bitSize) - placesTaken;

    entryBuffer = entryBuffer | (index << shiftAmount);
    placesTaken += bitSize;

    if(placesTaken >= 16) {
        quint16 toPut = quint16(entryBuffer >> 16);
        entryBuffer = entryBuffer << 16;
        placesTaken -= 16;

        char
                a = char(toPut >> 8),
                b = char(toPut);

        buffer.append(a);
        buffer.append(b);
    }

    if(buffer.size() >= bufferSize)
        flush();
}

void FileWriter::flush(){
    file->write(buffer);
    buffer.clear();
}

void FileWriter::close(){
    while(placesTaken > 0){
        int shiftAmount = 24;
        ushort toPut = ushort(entryBuffer >> shiftAmount);
        entryBuffer <<= 8;

        buffer.append(char(toPut));

        placesTaken -= 8;
    }

    if(buffer.size() > 0){
        file->write(buffer);
    }

    file->flush();
}
