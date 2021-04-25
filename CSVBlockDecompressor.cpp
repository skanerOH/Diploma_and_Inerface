//
// Created by Skaner on 09.03.2021.
//

#include "CSVBlockDecompressor.h"

CSVBlockDecompressor::CSVBlockDecompressor(std::ifstream &input,
                                           const unsigned short &_columnCount, const bool& doparallel) {
    columnCount = _columnCount;
    doParallel = doparallel;
    input.read((char*)&blockSize, sizeof(unsigned short));
    decompressorsVect = std::vector<std::shared_ptr<ColumnDecompressor>>();

    char method;

    for (int i=0; i<columnCount; ++i)
    {
        input.read((char*)&method, sizeof(char));
        decompressorsVect.push_back(GetDecompressorByMethod(method));
        decompressorsVect.back()->Read(input);
    }
}

std::shared_ptr<ColumnDecompressor> CSVBlockDecompressor::GetDecompressorByMethod(const char &method) {
    switch (method) {
        case 'h':
            return std::shared_ptr<ColumnDecompressor>(new HuffmanSDecoder(blockSize));
        case 'n':
            return std::shared_ptr<ColumnDecompressor>(new NumericDecoder());
        case 'l':
            return std::shared_ptr<ColumnDecompressor>(new LZWDecoder(blockSize));
        case 'k':
            return std::shared_ptr<ColumnDecompressor>(new HuffmanKDecoder(blockSize));
        case 'u':
            return std::shared_ptr<ColumnDecompressor>(new UShortTypeDecoder(blockSize));
        case 'i':
            return std::shared_ptr<ColumnDecompressor>(new IntTypeDecoder(blockSize));
//        case 'f':
//            return std::shared_ptr<ColumnDecompressor>(new TypeDecoder<float>(blockSize));
//        case 'd':
//            return std::shared_ptr<ColumnDecompressor>(new TypeDecoder<double>(blockSize));
        default:
            return nullptr;
    }
}

void CSVBlockDecompressor::Decomp(std::shared_ptr<ColumnDecompressor> &decomp) {
    decomp->Decode();
}

void CSVBlockDecompressor::Decompress() {
    std::vector<std::thread> thrVect = std::vector<std::thread>();

    if (false) {
        for (int i = 0; i < columnCount; ++i) {
            thrVect.push_back(std::thread(Decomp, std::ref(decompressorsVect.at(i))));
        }

        for (auto thr = thrVect.begin(); thr != thrVect.end(); ++thr) {
            thr->join();
        }
    }
    else{
        for (int i =0; i<columnCount; ++i)
        {
            decompressorsVect.at(i)->Decode();
        }
    }
}
