#include "compressionmachine.h"

CompressionMachine::CompressionMachine()
{
    paused = false;
    abortRequested = false;
    ready = false;
}

void CompressionMachine::setUp(QString inputPath, QString outputPath, QString fileExtension) {
    input = new QFile(inputPath);

    if(!input->open(QIODevice::ReadOnly)){
        emit error("No se pudo abrir el archivo");
        return;
    }

    output = new QFile(outputPath);

    if(!output->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        emit error("No se pudo crear el archivo resultado");
        return;
    }

    this->fileExtension = fileExtension;
    paused = false;
    abortRequested = false;
    ready = true;
}

CompressionMachine::~CompressionMachine()
{
    if(input == nullptr) delete input;
    if(output == nullptr) delete output;
}

bool CompressionMachine::isPaused(){ return paused; }
