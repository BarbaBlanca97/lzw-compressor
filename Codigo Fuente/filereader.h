#ifndef FILEREADER_H
#define FILEREADER_H

#include <QFile>

class FileReader
{
private:
    QFile *file;
    uint entryBuffer;
    int placesTaken;
public:
    FileReader(QFile *file);

    ushort nextIndex(int bitSize);
};

#endif // FILEREADER_H
