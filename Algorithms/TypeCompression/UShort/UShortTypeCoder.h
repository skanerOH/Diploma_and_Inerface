//
// Created by Skaner on 20.04.2021.
//

#ifndef DIPLOMA_USHORTTYPECODER_H
#define DIPLOMA_USHORTTYPECODER_H


#include "../../../ColumnCompressor.h"

class UShortTypeCoder : public ColumnCompressor {
public:
    UShortTypeCoder(std::shared_ptr<ColumnAnalyzer> columnAnalyzer);

    void Encode() override ;

    void WriteToFile(std::ofstream& out) override ;
};


#endif //DIPLOMA_USHORTTYPECODER_H
