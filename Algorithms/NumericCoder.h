#ifndef DIPLOMA_NUMERICCODER_H
#define DIPLOMA_NUMERICCODER_H

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
#include "HuffLeafCh.h"


class NumericCoder : public ColumnCompressor{
private:
    HuffLeafCh* CreateTree();

    char pack_byte(bool bits[8]);
public:
    std::vector<char> res;
    unsigned int codesCount;

    NumericCoder(const std::shared_ptr<ColumnAnalyzer>& colAnal);

    void Encode() override;

    void WriteToFile(std::ofstream& out) override;
};


#endif //DIPLOMA_NUMERICCODER_H
