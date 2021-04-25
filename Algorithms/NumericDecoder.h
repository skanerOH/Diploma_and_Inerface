//
// Created by Skaner on 20.04.2021.
//

#ifndef DIPLOMA_NUMERICDECODER_H
#define DIPLOMA_NUMERICDECODER_H

#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include "../ColumnAnalyzer.h"
#include "../ColumnDecompressor.h"
#include "HuffLeafCh.h"

class NumericDecoder : public ColumnDecompressor{
private:
    HuffLeafCh* CreateTree();
public:
    std::map<char, unsigned short> freqMap;
    std::vector<char> codesVect;
    unsigned int codesCount;

    NumericDecoder();

    void Decode() override;

    void Read(std::ifstream& infile) override;

};

#endif //DIPLOMA_NUMERICDECODER_H
