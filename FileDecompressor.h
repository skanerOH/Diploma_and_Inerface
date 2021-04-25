//
// Created by Skaner on 09.03.2021.
//

#ifndef DIPLOMA_FILEDECOMPRESSOR_H
#define DIPLOMA_FILEDECOMPRESSOR_H

#include "CSVBlockDecompressor.h"
#include "RecordParser.h"

class FileDecompressor {
public:
    std::ifstream input;
    std::ofstream output;
    bool doParallel;

    FileDecompressor(const std::string& inputPath, const std::string& outputPath, const bool& doparallel);

    bool IsOpened();

    void Decompress();
};


#endif //DIPLOMA_FILEDECOMPRESSOR_H
