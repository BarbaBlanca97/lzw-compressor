#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <vector>
#include <QString>
#include <QObject>

using namespace std;

#define EMPTY_INDEX -1
#define EOI_INDEX 256
#define FLUSH_INDEX 257

class OptimizedString {
    friend class Dictionary;
private:
    char byte;
    int
        prefixIndex,
        prev,
        next,
        first;
public:
    OptimizedString(char byte, int prefixIndex);
    OptimizedString();
};

class Dictionary : public QObject
{
    Q_OBJECT

private:
     vector<OptimizedString> dictionary;
     int
        maxIndex,
        currentMaxIndex,
        currentBitSize;

     bool toDecompress;

     void initialize(bool toDecompress);
public:
    Dictionary(bool toDecompress);

    int findOrAdd(OptimizedString newString);
    QByteArray getString(int index);
    bool exists(int index);
    int getCurrentBitSize();
    bool isFull();
    void reset();
};

#endif // DICTIONARY_H
