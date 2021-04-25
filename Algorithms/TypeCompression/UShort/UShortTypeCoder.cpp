//
// Created by Skaner on 20.04.2021.
//

#include "UShortTypeCoder.h"

UShortTypeCoder::UShortTypeCoder(std::shared_ptr<ColumnAnalyzer> columnAnalyzer) {
    colAnal = columnAnalyzer;
}

void UShortTypeCoder::WriteToFile(std::ofstream &out) {
    for (auto val = colAnal->ushortCol.begin(); val!=colAnal->ushortCol.end(); ++val)
    {
        out.write((char*)&(*val), sizeof(unsigned short));
    }
}

void UShortTypeCoder::Encode() {
}