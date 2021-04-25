//
// Created by Skaner on 07.03.2021.
//

#ifndef DIPLOMA_LZWDECODER_H
#define DIPLOMA_LZWDECODER_H

#include "../ColumnDecompressor.h"
#include <fstream>

class LZWDecoder : public ColumnDecompressor
{
private:
    void InitDict();
    std::string GetStrByCode(unsigned short code);
public:
    std::map<std::string, unsigned short int> dict;
    unsigned int codesCount;
    std::vector<unsigned short> codesVect;

    LZWDecoder(unsigned short vc);

    void Decode() override;

    void Read(std::ifstream& infile) override;
};


#endif //DIPLOMA_LZWDECODER_H
