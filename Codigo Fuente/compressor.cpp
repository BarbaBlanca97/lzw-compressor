#include <QString>
#include <QFileInfo>

#include "compressor.h"

Compressor::Compressor(){}

void Compressor::run()
{
    compressionAlgorithm();

    input->close();

    output->flush();
    output->close();

    delete input; delete output;

    emit jobDone();
    if(abortRequested)
        emit readyToClose();
}

void Compressor::compressionAlgorithm() {
    qint64 fileSize = input->size(), currentProgress = 0;

    QByteArray fileExtension(this->fileExtension.toLocal8Bit());
    fileExtension.append('.');
    output->write(fileExtension);

    FileWriter fileWriter(output, 1024);
    Dictionary dictionary(false);

    int index = EMPTY_INDEX;
    char byte;

    while(!input->atEnd()){
        while(paused){ this->msleep(500); }
        if(abortRequested) break;

        input->read(&byte, 1);

        OptimizedString newString = OptimizedString(byte, index);
        int result = int(dictionary.findOrAdd(newString));


        if(result != EMPTY_INDEX)
            index = result;
        else {
           fileWriter.put(uint(index), dictionary.getCurrentBitSize());
           index = uchar(byte);
        }

        if(dictionary.isFull()){
            fileWriter.put(uint(index), dictionary.getCurrentBitSize());
            fileWriter.put(uint(FLUSH_INDEX), dictionary.getCurrentBitSize());

            dictionary.reset();
            index = EMPTY_INDEX;
        }

        currentProgress = input->pos();
        float percentage = (float(currentProgress) / float(fileSize)) * 100;
        emit progress(int(percentage));
    }

    fileWriter.put(uint(index), dictionary.getCurrentBitSize());
    fileWriter.put(uint(EOI_INDEX), dictionary.getCurrentBitSize());
    fileWriter.close();
}

void Compressor::pause(){ paused = true; }
void Compressor::resume(){ paused = false; }
void Compressor::abort(){ abortRequested = true; }
