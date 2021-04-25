//
// Created by Skaner on 09.03.2021.
//

#include "FileDecompressor.h"

bool FileDecompressor::IsOpened() {
    return input.is_open() && !input.fail() && output.is_open() && !output.fail();
}

FileDecompressor::FileDecompressor(const std::string &inputPath, const std::string &outputPath, const bool& doparallel) {
    input = std::ifstream(inputPath, std::ios::binary);
    output = std::ofstream(outputPath, std::ios::binary);
    doParallel = doparallel;
}

void FileDecompressor::Decompress() {
    std::string namesStr;
    getline(input, namesStr);

    std::vector<std::string> colNamesVect = RecordParser::csv_split(namesStr, ',');

    namesStr.push_back('\n');
    output<<namesStr;

    while (!input.fail() && !input.eof())
    {
        CSVBlockDecompressor decomp = CSVBlockDecompressor(input, colNamesVect.size(), doParallel);

        decomp.Decompress();
        int a =decomp.decompressorsVect.at(0)->res->size();

        std::vector<std::string> line = std::vector<std::string>();

        for (int i=0; i<decomp.blockSize; ++i)
        {
            line.clear();
            for (int j=0; j<decomp.decompressorsVect.size(); ++j )
            {
                line.push_back(decomp.decompressorsVect.at(j)->res->at(i));
            }

            std::string line_t = RecordParser::csv_pack(line, ',');

            output<<line_t;
        }

        char c = input.peek();
    }

    input.close();
    output.close();
}

