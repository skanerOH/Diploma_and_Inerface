//
// Created by Skaner on 20.04.2021.
//

#ifndef DIPLOMA_INTTYPECODER_H
#define DIPLOMA_INTTYPECODER_H


#include "../../../ColumnCompressor.h"

class IntTypeCoder : public ColumnCompressor {
public:
    IntTypeCoder(std::shared_ptr<ColumnAnalyzer> columnAnalyzer);

    void Encode() override ;

    void WriteToFile(std::ofstream& out) override ;
};

#endif //DIPLOMA_INTTYPECODER_H
