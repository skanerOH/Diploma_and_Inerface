//
// Created by Skaner on 09.03.2021.
//

#ifndef DIPLOMA_CSVBLOCKDECOMPRESSOR_H
#define DIPLOMA_CSVBLOCKDECOMPRESSOR_H

#include <thread>
#include "ColumnDecompressor.h"
#include "Algorithms/HuffmanS.h"
#include "Algorithms/LZWDecoder.h"
#include "Algorithms/HuffmanK.h"
#include "Algorithms/TypeCompression/Int/IntTypeDecoder.h"
#include "Algorithms/TypeCompression/UShort/UShortTypeDecoder.h"
#include "Algorithms/NumericDecoder.h"


class CSVBlockDecompressor {
private:
    std::shared_ptr<ColumnDecompressor> GetDecompressorByMethod(const char& method);

    static void Decomp(std::shared_ptr<ColumnDecompressor>& decomp);
public:
    unsigned short blockSize;
    unsigned short columnCount;
    bool doParallel;
    std::vector<std::shared_ptr<ColumnDecompressor>> decompressorsVect;

    CSVBlockDecompressor(std::ifstream& input, const unsigned short& _columnCount, const bool& doparallel);

    void Decompress();
};


#endif //DIPLOMA_CSVBLOCKDECOMPRESSOR_H
