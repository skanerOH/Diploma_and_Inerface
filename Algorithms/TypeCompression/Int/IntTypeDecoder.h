//
// Created by Skaner on 20.04.2021.
//

#ifndef DIPLOMA_INTTYPEDECODER_H
#define DIPLOMA_INTTYPEDECODER_H

#include "../../../ColumnDecompressor.h"
#include "fstream"

class IntTypeDecoder : public ColumnDecompressor {
public:
    std::vector<int> values;

    IntTypeDecoder(const unsigned short& blockSize);

    void Decode() override ;

    void Read(std::ifstream& infile) override ;
};


#endif //DIPLOMA_INTTYPEDECODER_H
