//
// Created by Skaner on 02.03.2021.
//

#ifndef DIPLOMA_CSVBLOCKCOMPRESSOR_H
#define DIPLOMA_CSVBLOCKCOMPRESSOR_H

#include <memory>
#include <vector>
#include <string>
#include <climits>
#include <fstream>
#include <thread>
#include "RecordParser.h"
#include "ColumnAnalyzer.h"
#include "ColumnCompressor.h"
#include "Algorithms/NumericCoder.h"
#include "Algorithms/HuffmanS.h"
#include "Algorithms/LZWCoder.h"
#include "Algorithms/HuffmanK.h"
#include "Algorithms/TypeCompression/Int/IntTypeCoder.h"
#include "Algorithms/TypeCompression/UShort/UShortTypeCoder.h"

class CSVBlockCompressor {
private:
    const unsigned short BLOCK_MAX_SIZE = UINT16_MAX;

    static void ThrAnalize(std::shared_ptr<ColumnAnalyzer> columnAnalyzerPtr, char& res);
    static void ThrCompress(std::shared_ptr<ColumnCompressor> columnCompressorPtr);

    std::shared_ptr<ColumnCompressor> CreateCompressorByMethod(const char& m, std::shared_ptr<ColumnAnalyzer> colAnalPtr);
public:
    std::vector<std::string> columnsNames;
    std::vector<char> comprMethodsCodes;
    std::vector<std::shared_ptr<std::vector<std::string>>> columnsVector;
    std::vector<std::shared_ptr<ColumnAnalyzer>> analizedColumns;
    std::vector<std::shared_ptr<ColumnCompressor>> columnCompressorsVect;
    bool useTypes;
    bool useLZW;
    bool doParallel;
    unsigned short blockSize;

    CSVBlockCompressor(std::vector<std::string> columnNames, std::ifstream& file, const bool& usetypes, const bool& uselzw, const bool& doparallel);
    void AnalyzeColumns();
    void CompressColumns();
    void WriteCompressedColumns(std::ofstream& out);
};


#endif //DIPLOMA_CSVBLOCKCOMPRESSOR_H
