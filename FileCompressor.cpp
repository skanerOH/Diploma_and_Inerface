//
// Created by Skaner on 09.03.2021.
//

#include "FileCompressor.h"

FileCompressor::FileCompressor(const std::string &inputPath, const std::string &outputPath, const bool& uselzw, const bool& doparallel) {
    input = std::ifstream(inputPath, std::ios::binary);
    output = std::ofstream(outputPath, std::ios::binary);
    useLZW=uselzw;
    doParallel=doparallel;
}

bool FileCompressor::IsOpened() {
    return input.is_open() && !input.fail() && output.is_open() && !output.fail();
}

void FileCompressor::Compress() {

    std::vector<std::string> namesVect;
    std::string inp;
    std::getline(input,inp);
    namesVect = RecordParser::csv_split(inp,',');

    inp.push_back('\n');

    output<<inp;

    while (!input.fail() && !input.eof())
    {
        CSVBlockCompressor block = CSVBlockCompressor(namesVect, input,true, useLZW, doParallel);
        block.AnalyzeColumns();
        block.CompressColumns();
        block.WriteCompressedColumns(output);
    }

    input.close();
    output.close();
}
