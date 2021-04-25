#include "NumericDecoder.h"

NumericDecoder::NumericDecoder() {
    res = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
    freqMap = std::map<char, unsigned short>();
    codesVect = std::vector<char>();
    codesCount=0;

}

void NumericDecoder::Read(std::ifstream &infile) {
    //reading codes vect size
    infile.read((char*)&codesCount, sizeof(unsigned int));
    //reading codes
    unsigned int datasize = codesCount / 2 + codesCount % 2;
    char c;
    for (unsigned int i = 0; i<datasize; ++i)
    {
        infile.read((char*)&c, sizeof(char));
        codesVect.push_back(c);
    }

}

void NumericDecoder::Decode() {
    std::shared_ptr<HuffLeafCh> TreeRoot=std::shared_ptr<HuffLeafCh>(CreateTree());
    HuffLeafCh* currLeaf=TreeRoot.get();

    std::vector<char> microbuff = std::vector<char>();

    std::deque<bool> boolsToDecode;

    unsigned int opCounter=0;

    for(auto c=codesVect.begin(); c!=codesVect.end(); ++c)
    {
        for (int i=0; i<8; ++i)
            boolsToDecode.push_back((*c) & (1 << i));

        while (boolsToDecode.size()>0)
        {
            if (currLeaf->isLeaf) {
                if (currLeaf->value == ',')
                {
                    std::string s ="";
                    for (auto ch=microbuff.begin(); ch!=microbuff.end(); ++ch)
                    {
                        s.push_back(*ch);
                    }
                    res->push_back(s);
                    microbuff.clear();
                }
                else
                    microbuff.push_back(currLeaf->value);
                currLeaf=TreeRoot.get();
                ++opCounter;
            } else {
                if (boolsToDecode.front())
                    currLeaf = currLeaf->left;
                else
                    currLeaf = currLeaf->right;
                boolsToDecode.pop_front();
            }
        }
    }

    if (currLeaf->isLeaf) {
        if (currLeaf->value == ',') {
            std::string s = "";
            for (auto ch = microbuff.begin(); ch != microbuff.end(); ++ch) {
                s.push_back(*ch);
            }
            res->push_back(s);
            microbuff.clear();
            s ="";
            res->push_back(s);
        } else
            microbuff.push_back(currLeaf->value);
        currLeaf = TreeRoot.get();
        ++opCounter;
    }

    if (microbuff.size()>0)
    {
        std::string s ="";
        for (auto ch=microbuff.begin(); ch!=microbuff.end(); ++ch)
        {
            s.push_back(*ch);
        }
        res->push_back(s);
        microbuff.clear();
    }

    if (codesCount<opCounter)
    {
        res->at(res->size()-1).pop_back();
    }
}

HuffLeafCh *NumericDecoder::CreateTree() {
    std::map<char, unsigned short> templateData;
    templateData['0']=1;
    templateData['1']=1;
    templateData['2']=1;
    templateData['3']=1;
    templateData['4']=1;
    templateData['5']=1;
    templateData['6']=1;
    templateData['7']=1;
    templateData['8']=1;
    templateData['9']=1;
    templateData['/']=1;
    templateData['.']=1;
    templateData[' ']=1;
    templateData[':']=1;
    templateData[',']=1;
    templateData['-']=1;


    std::vector<HuffLeafCh*> huffVect;
    for (auto f=templateData.begin(); f != templateData.end(); ++f)
    {
        huffVect.push_back(new HuffLeafCh(f->first, nullptr, f->second));
    }

    while (huffVect.size()>1)
    {
        std::sort(huffVect.begin(), huffVect.end(), [](const HuffLeafCh* lhs, const HuffLeafCh* rhs) {
            return lhs->freq>rhs->freq;});
        HuffLeafCh* rightLeaf=huffVect.back();
        huffVect.pop_back();
        HuffLeafCh* leftLeaf=huffVect.back();
        huffVect.pop_back();
        HuffLeafCh* fuzed= new HuffLeafCh(rightLeaf, leftLeaf);
        rightLeaf->parent=fuzed;
        leftLeaf->parent=fuzed;
        huffVect.push_back(fuzed);
    }

    return huffVect.at(0);
}