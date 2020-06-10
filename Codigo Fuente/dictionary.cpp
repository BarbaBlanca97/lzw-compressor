#include "dictionary.h"
#include <cmath>
#include "simple_casts.h"

OptimizedString::OptimizedString(char byte, int prefixIndex) {
    this->byte = byte;
    this->prefixIndex = prefixIndex;

    first = EMPTY_INDEX;
    prev = EMPTY_INDEX;
    next = EMPTY_INDEX;
}

Dictionary::Dictionary(bool toDecompress) {
    initialize(toDecompress);
}

void Dictionary::initialize(bool toDecompress){
    maxIndex = int(pow(2, 16) - 1);
    currentBitSize = 9;
    currentMaxIndex = int(pow(2, currentBitSize) - ((toDecompress)? 3 : 1));

    this->toDecompress = toDecompress;

    dictionary = vector<OptimizedString>();

    for(unsigned int i = 0; i < EOI_INDEX; i++) {
        dictionary.push_back(OptimizedString(char(i), static_cast<int>(EMPTY_INDEX)));
    }

    dictionary.push_back(OptimizedString(char(EOI_INDEX), EMPTY_INDEX));
    dictionary.push_back(OptimizedString(char(FLUSH_INDEX), EMPTY_INDEX));
}

int Dictionary::findOrAdd(OptimizedString newString) {
    if(newString.prefixIndex == EMPTY_INDEX)
        return uchar(newString.byte);
    else
    {
        int index = dictionary[uint(newString.prefixIndex)].first;
        int currStringIndex = newString.prefixIndex;

        while(index != EMPTY_INDEX && dictionary[uint(index)].byte != newString.byte) {
            currStringIndex = index;

            index = (newString.byte < dictionary[uint(index)].byte)?
                        dictionary[uint(index)].prev:
                        dictionary[uint(index)].next;
        }

        if(index == EMPTY_INDEX){
            dictionary.push_back(newString);

            if(dictionary[uint(newString.prefixIndex)].first == EMPTY_INDEX)
                dictionary[uint(newString.prefixIndex)].first = int(dictionary.size() - 1);
            else {
                if(newString.byte < dictionary[uint(currStringIndex)].byte)
                    dictionary[uint(currStringIndex)].prev = int(dictionary.size() - 1);
                else
                    dictionary[uint(currStringIndex)].next = int(dictionary.size() - 1);
            }


            if(int(dictionary.size() - 1) == currentMaxIndex)
                currentMaxIndex = int(pow(2, ++currentBitSize) - ((toDecompress)? 3 : 1));
        }
        return index;
    }
}

QByteArray Dictionary::getString(int index) {
   QByteArray result;

   while(index != EMPTY_INDEX){
       result.push_back(dictionary[uint(index)].byte);
       index = dictionary[uint(index)].prefixIndex;
   }

   QByteArray trueResult;
   for(int i = int(result.length()) - 1; i >= 0; i--){
       trueResult.push_back(result[uint(i)]);
   }

   return trueResult;
}

void Dictionary::reset(){
    dictionary.clear();
    initialize(toDecompress);
}

bool Dictionary::exists(int index) {
    return index < int(dictionary.size());
}

int Dictionary::getCurrentBitSize(){
    return currentBitSize;
}

bool Dictionary::isFull(){
    return int((dictionary.size()) - 1) == maxIndex;
}

