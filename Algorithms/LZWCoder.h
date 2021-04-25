//
// Created by Skaner on 07.03.2021.
//

#ifndef DIPLOMA_LZWCODER_H
#define DIPLOMA_LZWCODER_H

#include "../ColumnCompressor.h"
#include "../ColumnDecompressor.h"


class LZWCoder : public ColumnCompressor
{
private:
    void InitDict();
    std::string GetStrByCode(unsigned short code);
public:
    unsigned int codesCount;
    std::vector<unsigned short> res;
    std::map<std::string, unsigned short int> dict;

    LZWCoder(const std::shared_ptr<ColumnAnalyzer>& colAnal);

    void Encode() override;

    void WriteToFile(std::ofstream& out) override;
};


#endif //DIPLOMA_LZWCODER_H
