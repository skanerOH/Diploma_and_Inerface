#include "HuffmanS.h"

HuffLeafStr::HuffLeafStr(std::string value_t, HuffLeafStr* parent_t, int freq_t)
{
    value=value_t;
    parent=parent_t;
    freq=freq_t;
    isLeaf=true;
    right= nullptr;
    left= nullptr;
}

HuffLeafStr::HuffLeafStr(HuffLeafStr* right_t, HuffLeafStr* left_t) {
    right = right_t;
    left = left_t;
    freq = right_t->freq + left_t->freq;
    value = "";
    isLeaf = false;
    parent = nullptr;
}

HuffLeafStr::~HuffLeafStr()
{
    delete left;
    delete right;
}

std::string HuffLeafStr::GetCode(std::string inp)
{
    if (this->isLeaf)
    {
        return "1";
    }
    if (left->isLeaf)
    {
        if (left->value.compare(inp)==0)
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
        if (right->value.compare(inp)==0)
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


HuffmanSCoder::HuffmanSCoder(const std::shared_ptr<ColumnAnalyzer> &colAnal) {
    this->colAnal = colAnal;
    res = std::vector<char>();
}

void HuffmanSCoder::Encode() {
    std::shared_ptr<HuffLeafStr> TreeRoot=std::shared_ptr<HuffLeafStr>(CreateTree());
    std::map<std::string, std::string> encMap;

    std::vector<bool> codedBools;
    std::string codeS;
    for (int i=0; i<colAnal->columnValues->size(); ++i)
    {
        if (!encMap.count(colAnal->columnValues->at(i)))
            encMap[colAnal->columnValues->at(i)]=TreeRoot->GetCode(colAnal->columnValues->at(i));
        codeS=encMap[colAnal->columnValues->at(i)];
        for (int j=0; j<codeS.size(); ++j)
            codedBools.push_back(codeS[j]=='1');
        while (codedBools.size()>=8)
        {
            char s(0);
            bool bits[8];
            for (int i=0; i<8; ++i) {
                bits[i] = codedBools.front();
                codedBools.erase(codedBools.begin());
            }
            s = pack_byte(bits);
            res.push_back(s);
        }
    }
    if (codedBools.size()>0) {
        while (codedBools.size() < 8)
            codedBools.push_back(false);
        char s(0);
        bool bits[8];
        for (int i = 0; i < 8; ++i) {
            bits[i] = codedBools.front();
            codedBools.erase(codedBools.begin());
        }
        s = pack_byte(bits);
        res.push_back(s);
    }

}

HuffLeafStr *HuffmanSCoder::CreateTree() {
    {
        std::vector<HuffLeafStr*> huffVect;
        for (auto f=colAnal->strMap.begin(); f != colAnal->strMap.end(); ++f)
        {
            huffVect.push_back(new HuffLeafStr(f->first, nullptr, f->second));
        }

        while (huffVect.size()>1)
        {
            std::sort(huffVect.begin(), huffVect.end(), [](const HuffLeafStr* lhs, const HuffLeafStr* rhs) {
                return lhs->freq>rhs->freq;});
            HuffLeafStr* rightLeaf=huffVect.back();
            huffVect.pop_back();
            HuffLeafStr* leftLeaf=huffVect.back();
            huffVect.pop_back();
            HuffLeafStr* fuzed= new HuffLeafStr(rightLeaf, leftLeaf);
            rightLeaf->parent=fuzed;
            leftLeaf->parent=fuzed;
            huffVect.push_back(fuzed);
        }

        return huffVect.at(0);
    }
}

char HuffmanSCoder::pack_byte(bool bits[8]) {
    char res(0);
    for (unsigned i(8); i--;)
    {
        res <<=1;
        res |= char(bits[i]);
    }
    return res;
}

void HuffmanSCoder::WriteToFile(std::ofstream &out) {

    //map size
    unsigned short mapSize = colAnal->strMap.size();
    out.write((char*)&mapSize, sizeof(unsigned short));

    //map
    for (auto item = colAnal->strMap.begin(); item!=colAnal->strMap.end();++item)
    {
        std::string s = item->first;
        do{
            if (s.size()<=254)
            {
                unsigned char sSize = s.size();
                out.write((char*)&sSize, sizeof(unsigned char));
                out.write((char*)(s.c_str()), sizeof(char)*sSize);
                s.clear();
            } else
            {
                unsigned char sSize = 255;
                out.write((char*)&sSize, sizeof(unsigned char));
                out.write((char*)(s.substr(0,sSize).c_str()), sizeof(char)*sSize);
                s.erase(0,sSize);
            }
        }
        while (s.size()!=0);

        unsigned short freq = item->second;
        out.write((char*)&freq, sizeof(unsigned short));
    }

    //res size
    unsigned int ressize = res.size();
    out.write((char*)&ressize, sizeof(unsigned int));

    //codes
    unsigned int from=0;
    char* buff=new char[USHRT_MAX];
    while (from<res.size())
    {
        if (from+USHRT_MAX<res.size())
        {
            for (int i=0; i<USHRT_MAX; ++i)
            {
                buff[i]=res.at(from+i);
            }
            out.write(buff, sizeof(char)*USHRT_MAX);
            from+=USHRT_MAX;
        }
        else {
            for (int i=0; i<res.size()-from; ++i)
            {
                buff[i]=res.at(from+i);
            }
            out.write(buff, sizeof(char)*(res.size()-from));
            from=res.size();
        }
    }
}

HuffmanSDecoder::HuffmanSDecoder(const unsigned short & codesCount) {
    this->valuesCount = codesCount;
    res = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
    freqMap = std::map<std::string, unsigned short>();
    codesVect = std::vector<char>();

}

void HuffmanSDecoder::Read(std::ifstream &infile) {
    //read freq map size
    unsigned short mapSize;
    infile.read((char*)&mapSize, sizeof(unsigned short));
    //read freq map
    for (unsigned short i = 0; i<mapSize; ++i)
    {
        std::string s;
        unsigned short freq;
        unsigned char c;
        do {
            infile.read((char *) &c, sizeof(unsigned char));
            char *buff = new char[c];
            infile.read(buff, sizeof(char) * c);
            for (unsigned char i=0; i<c; ++i)
                s.push_back(buff[i]);
            delete[] buff;
        }
        while (c==255);
        infile.read((char*)&freq, sizeof(unsigned short));
        freqMap[s] = freq;
    }
    //reading codes vect size
    unsigned int inpSize;
    infile.read((char*)&inpSize, sizeof(unsigned int));
    //reading codes
    char c;
    for (unsigned int i = 0; i<inpSize; ++i)
    {
        infile.read((char*)&c, sizeof(char));
        codesVect.push_back(c);
    }

}

void HuffmanSDecoder::Decode() {
    std::shared_ptr<HuffLeafStr> TreeRoot=std::shared_ptr<HuffLeafStr>(CreateTree());
    HuffLeafStr* currLeaf=TreeRoot.get();

    if (freqMap.begin()->first.compare("1347")==0)
    {
        std::cout<<"cock";
    }

    if (freqMap.size()==1)
    {
        for (int i=0; i<valuesCount; ++i)
        {
            res->push_back(TreeRoot->value);
        }
    } else {
        std::deque<bool> boolsToDecode;
        for (auto c = codesVect.begin(); c != codesVect.end(); ++c) {
            for (int i = 0; i < 8; ++i)
                boolsToDecode.push_back((*c) & (1 << i));

            while (boolsToDecode.size() > 0) {
                if (currLeaf->isLeaf) {
                    res->push_back(currLeaf->value);
                    currLeaf = TreeRoot.get();
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
            res->push_back(currLeaf->value);
        }

        while (res->size() > valuesCount)
            res->pop_back();
    }
}

HuffLeafStr *HuffmanSDecoder::CreateTree() {
    std::vector<HuffLeafStr*> huffVect;
    for (auto f=freqMap.begin(); f != freqMap.end(); ++f)
    {
        huffVect.push_back(new HuffLeafStr(f->first, nullptr, f->second));
    }

    while (huffVect.size()>1)
    {
        std::sort(huffVect.begin(), huffVect.end(), [](const HuffLeafStr* lhs, const HuffLeafStr* rhs) {
            return lhs->freq>rhs->freq;});
        HuffLeafStr* rightLeaf=huffVect.back();
        huffVect.pop_back();
        HuffLeafStr* leftLeaf=huffVect.back();
        huffVect.pop_back();
        HuffLeafStr* fuzed= new HuffLeafStr(rightLeaf, leftLeaf);
        rightLeaf->parent=fuzed;
        leftLeaf->parent=fuzed;
        huffVect.push_back(fuzed);
    }

    return huffVect.at(0);
}
