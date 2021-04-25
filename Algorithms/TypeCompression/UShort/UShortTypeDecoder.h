//
// Created by Skaner on 20.04.2021.
//

#ifndef DIPLOMA_USHORTTYPEDECODER_H
#define DIPLOMA_USHORTTYPEDECODER_H


#include "../../../ColumnDecompressor.h"
#include "fstream"

class UShortTypeDecoder : public ColumnDecompressor {
public:
    std::vector<unsigned short> values;

    UShortTypeDecoder(const unsigned short& blockSize);

    void Decode() override ;

    void Read(std::ifstream& infile) override ;
};


#endif //DIPLOMA_USHORTTYPEDECODER_H
