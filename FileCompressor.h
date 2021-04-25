//
// Created by Skaner on 09.03.2021.
//

#ifndef DIPLOMA_FILECOMPRESSOR_H
#define DIPLOMA_FILECOMPRESSOR_H

#include <fstream>
#include "CSVBlockCompressor.h"


class FileCompressor {
private:
    std::ifstream input;
    std::ofstream output;
public:
    bool useTypes;
    bool useLZW;
    bool doParallel;

    FileCompressor(const std::string& inputPath, const std::string& outputPath, const bool& uselzw, const bool& doparallel);

    bool IsOpened();

    void Compress();
};


#endif //DIPLOMA_FILECOMPRESSOR_H
