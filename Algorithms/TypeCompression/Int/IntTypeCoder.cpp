//
// Created by Skaner on 20.04.2021.
//

#include "IntTypeCoder.h"

IntTypeCoder::IntTypeCoder(std::shared_ptr<ColumnAnalyzer> columnAnalyzer) {
    colAnal = columnAnalyzer;
}

void IntTypeCoder::WriteToFile(std::ofstream &out) {
    for (auto val = colAnal->intCol.begin(); val!=colAnal->intCol.end(); ++val)
    {
        out.write((char*)&(*val), sizeof(int));
    }
}

void IntTypeCoder::Encode() {
}