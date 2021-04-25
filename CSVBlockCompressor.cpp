//
// Created by Skaner on 02.03.2021.
//

#include "CSVBlockCompressor.h"

CSVBlockCompressor::CSVBlockCompressor(std::vector<std::string> columnNames, std::ifstream& file, const bool& usetypes, const bool& uselzw, const bool& doparallel) {
    columnsNames = columnNames;
    useTypes = usetypes;
    useLZW=uselzw;
    doParallel=doparallel;
    analizedColumns = std::vector<std::shared_ptr<ColumnAnalyzer>>();
    columnsVector = std::vector<std::shared_ptr<std::vector<std::string>>>();
    for (auto i = columnNames.begin(); i < columnNames.end(); ++i) {
        columnsVector.push_back(std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>));
    }
    columnCompressorsVect = std::vector<std::shared_ptr<ColumnCompressor>>();

    unsigned short counter = 0;
    std::string inp;
    std::vector<std::string> res;

    if (file.is_open() && !file.fail()) {
        while (counter<BLOCK_MAX_SIZE && std::getline(file, inp)) {
            res = RecordParser::csv_split(inp, ',');
            if (res.size()!=columnNames.size())
                throw (std::runtime_error(std::string("Invalid data in file")));
            for (int i=0; i<columnNames.size(); ++i)
            {
                columnsVector.at(i)->push_back(res.at(i));
            }
            ++counter;
        }
    }

    blockSize = counter;

}

void CSVBlockCompressor::AnalyzeColumns() {
    comprMethodsCodes = std::vector<char>();

    if (doParallel) {
        std::vector<std::thread> threadVector = std::vector<std::thread>();
        for (auto i = columnsVector.begin(); i != columnsVector.end(); ++i) {
            analizedColumns.push_back(std::shared_ptr<ColumnAnalyzer>(new ColumnAnalyzer(*i, useTypes, useLZW)));
            comprMethodsCodes.push_back(' ');
        }

        for (int i = 0; i < columnsVector.size(); ++i) {
            threadVector.push_back(
                    std::thread(ThrAnalize, std::ref(analizedColumns.at(i)), std::ref(comprMethodsCodes.at(i))));
        }

        for (auto i = threadVector.begin(); i != threadVector.end(); ++i) {
            i->join();
        }
    } else{
        for (auto i = columnsVector.begin(); i != columnsVector.end(); ++i) {
            analizedColumns.push_back(std::shared_ptr<ColumnAnalyzer>(new ColumnAnalyzer(*i, useTypes, useLZW)));
            comprMethodsCodes.push_back(' ');
        }

        for (int i = 0; i < columnsVector.size(); ++i) {
            ThrAnalize(std::ref(analizedColumns.at(i)), std::ref(comprMethodsCodes.at(i)));
        }

    }
}

void CSVBlockCompressor::ThrAnalize(std::shared_ptr<ColumnAnalyzer> columnAnalyzerPtr, char& res) {
    res = columnAnalyzerPtr->Analyze();
}

void CSVBlockCompressor::CompressColumns() {
    for (int i=0; i<comprMethodsCodes.size(); ++i)
    {
        columnCompressorsVect.push_back(CreateCompressorByMethod(comprMethodsCodes[i], analizedColumns[i]));
    }

    if (false) {
        std::vector<std::thread> threadVector = std::vector<std::thread>();

        for (int i = 0; i < comprMethodsCodes.size(); ++i) {
            threadVector.push_back(std::thread(ThrCompress, std::ref(columnCompressorsVect.at(i))));
        }

        for (auto i = threadVector.begin(); i != threadVector.end(); ++i) {
            i->join();
        }
    } else{
        for (int i = 0; i < comprMethodsCodes.size(); ++i) {
            ThrCompress(std::ref(columnCompressorsVect.at(i)));
        }
    }

}

std::shared_ptr<ColumnCompressor> CSVBlockCompressor::CreateCompressorByMethod(const char& m, std::shared_ptr<ColumnAnalyzer> colAnalPtr) {
    switch (m)
    {
        case 'h':
            return std::shared_ptr<ColumnCompressor>(new HuffmanSCoder(colAnalPtr));
        case 'n':
            return std::shared_ptr<ColumnCompressor>(new NumericCoder(colAnalPtr));
        case 'l':
            return std::shared_ptr<ColumnCompressor>(new LZWCoder(colAnalPtr));
        case 'k':
            return std::shared_ptr<ColumnCompressor>(new HuffmanKCoder(colAnalPtr));
        case 'u':
            return std::shared_ptr<ColumnCompressor>(new UShortTypeCoder(colAnalPtr));
        case 'i':
            return std::shared_ptr<ColumnCompressor>(new IntTypeCoder(colAnalPtr));
//        case 'f':
//            return std::shared_ptr<ColumnCompressor>(new TypeCoder<float>(colAnalPtr));
//        case 'd':
//            return std::shared_ptr<ColumnCompressor>(new TypeCoder<double>(colAnalPtr));
        default:
            return nullptr;
    }
}

void CSVBlockCompressor::ThrCompress(std::shared_ptr<ColumnCompressor> columnCompressorPtr) {
    columnCompressorPtr->Encode();
}

void CSVBlockCompressor::WriteCompressedColumns(std::ofstream &out) {
    out.write((char*)&blockSize, sizeof(unsigned short));

    for (int i=0; i<comprMethodsCodes.size(); ++i)
    {
        out.write((char*)&comprMethodsCodes.at(i), sizeof(char));
        columnCompressorsVect.at(i)->WriteToFile(out);
    }
}

