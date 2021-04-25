//
// Created by Skaner on 04.03.2021.
//

#ifndef DIPLOMA_COLUMNCOMPRESSOR_H
#define DIPLOMA_COLUMNCOMPRESSOR_H

#include <vector>
#include <string>
#include <memory>
#include <climits>
#include "ColumnAnalyzer.h"
#include <fstream>

class ColumnCompressor {
public:
    std::shared_ptr<ColumnAnalyzer> colAnal;

    virtual void Encode() = 0;

    virtual void WriteToFile(std::ofstream& out) = 0;
};


#endif //DIPLOMA_COLUMNCOMPRESSOR_H
