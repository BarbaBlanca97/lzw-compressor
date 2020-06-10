#include "decompressor.h"

Decompressor::Decompressor() : CompressionMachine (){}

void Decompressor::run()
{
    decompressionAlgorithm();

    input->close();

    delete input; delete output;

    emit jobDone();
}

void Decompressor::decompressionAlgorithm() {
    QString fileExtension;
    qint64 fileSize = input->size(), currentProgress = 0;

    char c;
    while(input->read(&c, 1)){
        if(c == '.') break;

        fileExtension.append(c);
    }

    FileReader fileReader(input);
    Dictionary dictionary(true);
    QByteArray result;
    char byte;

    int index = fileReader.nextIndex(dictionary.getCurrentBitSize());
    result = dictionary.getString(index);
    output->write(result);

    int old = index;

    while(true){
        while(paused){ this->msleep(500); }

        index = fileReader.nextIndex(dictionary.getCurrentBitSize());

        if(abortRequested) { index = EOI_INDEX; }

        if(index == EOI_INDEX)
        {
            output->flush();
            output->close();

            if(!fileExtension.isEmpty()){
                fileExtension.push_front('.');
                if(!output->rename(output->fileName() + fileExtension)){
                    if(!output->rename(output->fileName() + "-(duplicado)" + fileExtension)){
                        QString stringError = output->errorString();
                        emit error("No se pudo crear el archivo descomprimido\nProbablemente el archivo ya existe");
                    }
                }
            }

            return;
        }
        else if(index == FLUSH_INDEX)
        {
            dictionary.reset();

            index = fileReader.nextIndex(dictionary.getCurrentBitSize());
            result = dictionary.getString(index);
            output->write(result);

            old = index;

            continue;
        }

        if(dictionary.exists(index))
        {
            result = dictionary.getString(index);
            output->write(result);

            byte = result[0];

            OptimizedString entry(byte, old);
            dictionary.findOrAdd(entry);

        }
        else
        {
            result = dictionary.getString(old);
            byte = result[0];

            OptimizedString entry(byte, old);
            dictionary.findOrAdd(entry);

            result.push_back(byte);

            output->write(result);
        }
        old = index;

        currentProgress = input->pos();
        float percentage = (float(currentProgress) / float(fileSize)) * 100;
        emit progress(int(percentage));
    }

}

void Decompressor::pause(){ paused = true; }
void Decompressor::resume(){ paused = false; }
void Decompressor::abort(){ abortRequested = true; }
