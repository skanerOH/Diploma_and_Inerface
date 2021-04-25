//
// Created by Skaner on 17.04.2021.
//

#ifndef DIPLOMA_HUFFLEAFCH_H
#define DIPLOMA_HUFFLEAFCH_H

#include <string>


class HuffLeafCh{
public:
    char value;
    HuffLeafCh* left;
    HuffLeafCh* right;
    HuffLeafCh* parent;
    bool isLeaf;
    unsigned long int freq;
public:
    HuffLeafCh(char value_t, HuffLeafCh* parent_t, int freq_t);

    HuffLeafCh(HuffLeafCh* right_t, HuffLeafCh* left_t);

    ~HuffLeafCh();

    std::string GetCode(char inp);
};


#endif //DIPLOMA_HUFFLEAFCH_H
