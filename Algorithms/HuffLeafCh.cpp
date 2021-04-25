#include "HuffLeafCh.h"

HuffLeafCh::HuffLeafCh(char value_t, HuffLeafCh* parent_t, int freq_t)
{
    value=value_t;
    parent=parent_t;
    freq=freq_t;
    isLeaf=true;
    right= nullptr;
    left= nullptr;
}

HuffLeafCh::HuffLeafCh(HuffLeafCh* right_t, HuffLeafCh* left_t) {
    right = right_t;
    left = left_t;
    freq = right_t->freq + left_t->freq;
    value = '\0';
    isLeaf = false;
    parent = nullptr;
}

HuffLeafCh::~HuffLeafCh()
{
    delete left;
    delete right;
}

std::string HuffLeafCh::GetCode(char inp)
{
    if (left->isLeaf)
    {
        if (left->value==inp)
            return "1";
    }
    else
    {
        std::string v=left->GetCode(inp);
        if (v[0]!='n')
            return "1"+v;
    }

    if (right->isLeaf)
    {
        if (right->value==inp)
            return "0";
    }
    else
    {
        std::string v=right->GetCode(inp);
        if (v[0]!='n')
            return "0"+v;
    }
    return "n";
}