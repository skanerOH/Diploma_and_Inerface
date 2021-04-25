//
// Created by Skaner on 07.03.2021.
//

#include "LZWCoder.h"

LZWCoder::LZWCoder(const std::shared_ptr<ColumnAnalyzer> &colAnal) {
    this->colAnal = colAnal;
    res = std::vector<unsigned short>();
    InitDict();
}

void LZWCoder::InitDict()
{
    dict = std::map<std::string, unsigned short>();
    for (char i=CHAR_MIN; i!=CHAR_MAX; ++i)
    {
        dict[std::string(1, i)]=dict.size();
    }
}

std::string LZWCoder::GetStrByCode(unsigned short code)
{
    for (auto &i : dict)
    {
        if (i.second == code)
            return i.first;
    }
    return "";
}

void LZWCoder::Encode() {
    std::vector<char> chars = std::vector<char>();

    for (auto str = colAnal->columnValues->begin(); str != colAnal->columnValues->end(); ++str) {
        for (auto c = str->begin(); c != str->end(); ++c) {
            chars.emplace_back(*c);
        }
        chars.emplace_back(',');
    }

    chars.pop_back();
    std::string curr;
    std::string priv;
    for (unsigned int i = 0; i < chars.size(); ++i) {
        curr = std::string(1, chars.at(i));
        if (dict.count(priv + curr)) {
            priv += curr;
        } else {
            res.push_back(dict[priv]);
            if (dict.size() < USHRT_MAX - 1)
                dict[priv + curr] = dict.size() + 1;
            priv = curr;
        }
    }
    res.push_back(dict[priv]);
}

void LZWCoder::WriteToFile(std::ofstream &out) {

    //codesCount
    codesCount = res.size();
    out.write((char*)&codesCount, sizeof(unsigned int));

    //codes
    for (auto a = res.begin(); a!=res.end(); ++a)
    {
        out.write((char*)&(*a), sizeof(unsigned short));
    }
}

