//
// Created by Skaner on 20.04.2021.
//

#include "UShortTypeDecoder.h"

void UShortTypeDecoder::Decode() {
    for (auto val=values.begin(); val!=values.end(); ++val)
    {
        if (*val==USHRT_MAX)
            res->push_back("");
        res->push_back(std::to_string(*val));
    }
}

UShortTypeDecoder::UShortTypeDecoder(const unsigned short &blockSize) {
    valuesCount = blockSize;
    values = std::vector<unsigned short>();
    res = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
}

void UShortTypeDecoder::Read(std::ifstream &infile) {
    unsigned short val;
    for (int i=0; i<valuesCount; ++i)
    {
        infile.read((char*)&val, sizeof(val));
        values.push_back(val);
    }
}