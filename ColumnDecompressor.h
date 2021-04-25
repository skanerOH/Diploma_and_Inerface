//
// Created by Skaner on 06.03.2021.
//

#ifndef DIPLOMA_COLUMNDECOMPRESSOR_H
#define DIPLOMA_COLUMNDECOMPRESSOR_H

#include "ColumnAnalyzer.h"

class ColumnDecompressor {
public:
    std::shared_ptr<std::vector<std::string>> res;
    unsigned short valuesCount;

    virtual void Decode()=0;

    virtual void Read(std::ifstream& infile)=0;
};


#endif //DIPLOMA_COLUMNDECOMPRESSOR_H
