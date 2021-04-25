#ifndef DIPLOMA_HUFFMANK_H
#define DIPLOMA_HUFFMANK_H
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include "../ColumnAnalyzer.h"
#include "../ColumnCompressor.h"
#include "../ColumnDecompressor.h"
#include "HuffLeafCh.h"


class HuffmanKCoder : public ColumnCompressor{
private:
    HuffLeafCh* CreateTree();

    char pack_byte(bool bits[8]);
public:
    std::vector<char> res;
    unsigned int codesCount;

    HuffmanKCoder(const std::shared_ptr<ColumnAnalyzer>& colAnal);

    void Encode() override;

    void WriteToFile(std::ofstream& out) override;
};

class HuffmanKDecoder : public ColumnDecompressor{
private:
    HuffLeafCh* CreateTree();
public:
    std::map<char, unsigned short> freqMap;
    std::vector<char> codesVect;
    unsigned int codesCount;

    HuffmanKDecoder(unsigned short vc);

    void Decode() override;

    void Read(std::ifstream& infile) override;

};


#endif //DIPLOMA_HUFFMANK_H
