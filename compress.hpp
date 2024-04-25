#define COMPRESS_HPP
#include<bits/stdc++.h>
#include "file_operations.hpp" 
using namespace std; 

struct Tree
{
    int frequency;
    unsigned char character;
    Tree *left = NULL;
    Tree *right = NULL;
};

class TreeComparator
{
  public:
    bool operator()(Tree *a, Tree *b)
    {
        return a->frequency > b->frequency;
    }
};

Tree *buildHuffmanTree(vector<pair<unsigned char, int> > freqtable)
{

    priority_queue<Tree *, vector<Tree *>, TreeComparator> huffqueue;
    for (int i = 0; i < freqtable.size(); i++)
    {
        Tree *node = new Tree();
        node->frequency = freqtable[i].second;
        node->character = freqtable[i].first;
        
        huffqueue.push(node);
    }

    while (huffqueue.size() > 1)
    {
        Tree *a, *b;
        a = huffqueue.top();
        huffqueue.pop();
        
        b = huffqueue.top();
        huffqueue.pop();
        Tree *c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        huffqueue.push(c);
    }

    return huffqueue.top();
}

void traverseHuffmanTree(Tree *root, string prev, string toAppend, map<unsigned char, string> &codemap)
{
    prev+=toAppend;
    if (root->right == NULL && root->left == NULL)
    {
        // cout<<root->character<<" "<<prev<<endl;   
        codemap[root->character] = prev;
    }
    if (root->right != NULL)
    {
        traverseHuffmanTree(root->right, prev, "1", codemap);
    }
    if (root->left != NULL)
    {
        traverseHuffmanTree(root->left, prev, "0", codemap);
    }
}

vector<pair<unsigned char, int> > convertToVector(map<unsigned char, int> codes)
{
    vector<pair<unsigned char, int> > codesV;

    for (map<unsigned char, int>::iterator i = codes.begin(); i != codes.end(); i++)
    {
        codesV.push_back(make_pair(i->first, i->second));
    }

    return codesV;
}

string getHuffmanBitstring(unsigned char *buffer, map<unsigned char, string> codes, int sz, int& paddedBits)
{
    string outputBuffer="";
    for(int i=0; i<sz; i++)
    {
        outputBuffer=outputBuffer+codes[buffer[i]];
    }

    if(outputBuffer.size()%8!=0)
    {
        int deficit = 8*((outputBuffer.size()/8)+1)-outputBuffer.size();
        paddedBits = deficit;
        for(int i=0; i<deficit; i++)
        {
            outputBuffer+="0";
        }
    }

    
    return outputBuffer;
}

unsigned char* getBufferFromString(string bitstring, vector<unsigned char>&outputBuffer, int& sz)
{
    int interval = 0;
    unsigned char bit = 0;

    for(int i=0; i<sz; i++)
    {
         bit = (bit<<1)|(bitstring[i]-'0');
         
        interval++;
        if(interval==8)
        {
            interval = 0;
            outputBuffer.push_back(bit);
            bit = 0;
        
        }   
    }
    sz = outputBuffer.size();
    return outputBuffer.data();
}

void writeHeader(char* path,map<unsigned char, string> codes,  int paddedBits){
    
    int size = codes.size();
    FileOperations::writeFileFromBuffer(path, (unsigned char*)&paddedBits, sizeof(int), 0);
    FileOperations::writeFileFromBuffer(path, (unsigned char*)&size, sizeof(int), 1);
    char nullBit = '\0';
    for(map<unsigned char, string>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        FileOperations::writeFileFromBuffer(path, (unsigned char*)&i->first, 1, 1);
        int len = i->second.size();
        FileOperations::writeFileFromBuffer(path, (unsigned char*)&len, sizeof(int), 1);
        FileOperations::writeFileFromBuffer(path, (unsigned char*)i->second.c_str(), i->second.size(), 1);
    }
}

void compressFile( char *path,  char *output_path, map<unsigned char, string> &codes)
{
    int sz = 0;
    int paddedBits = 0;
    map<unsigned char, int> freqtable;
    unsigned char *buffer = FileOperations::readFileIntoBuffer(path, sz);
    if (buffer == nullptr) {
        cout << "Error: Input file '" << path << "' not found or unable to open." << endl;
        cout<<"Compression Not Successful\n";
        return; // Exit the function if the input file is not found
    }

    for (int i = 0; i < sz; i++)
    {
            freqtable[buffer[i]]++;
    }
    Tree *root = buildHuffmanTree(convertToVector(freqtable));
    cout<<root<<endl;
    traverseHuffmanTree(root, "", "", codes);
    string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    sz  = outputString.size();
    vector<unsigned char> outputBufferV;
    getBufferFromString(outputString, outputBufferV, sz);
    unsigned char* outputBuffer = outputBufferV.data();
    writeHeader(output_path, codes, paddedBits);
    FileOperations::writeFileFromBuffer(output_path, outputBuffer, sz, 1);
    cout<<"Compression Successful\n";
}
