//
// Created by Skaner on 06.03.2021.
//

#ifndef DIPLOMA_HUFFMANS_H
#define DIPLOMA_HUFFMANS_H


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


class HuffLeafStr{
public:
    std::string value;
    HuffLeafStr* left;
    HuffLeafStr* right;
    HuffLeafStr* parent;
    bool isLeaf;
    unsigned short freq;
public:
    HuffLeafStr(std::string value_t, HuffLeafStr* parent_t, int freq_t);

    HuffLeafStr(HuffLeafStr* right_t, HuffLeafStr* left_t);

    ~HuffLeafStr();

    std::string GetCode(std::string inp);
};


class HuffmanSCoder : public ColumnCompressor{
private:
    HuffLeafStr* CreateTree();

    char pack_byte(bool bits[8]);
public:
    std::vector<char> res;

    HuffmanSCoder(const std::shared_ptr<ColumnAnalyzer>& colAnal);

    void Encode() override;

    void WriteToFile(std::ofstream& out) override;
};

class HuffmanSDecoder : public ColumnDecompressor{
private:
    HuffLeafStr* CreateTree();
public:
    std::map<std::string, unsigned short> freqMap;
    std::vector<char> codesVect;

    HuffmanSDecoder(const unsigned short& codesCount);

    void Decode() override;

    void Read(std::ifstream& infile) override;

};


#endif //DIPLOMA_HUFFMANS_H
