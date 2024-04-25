#define DECOMPRESS_HPP
#include<bits/stdc++.h>
using namespace std;

#include "file_operations.hpp" 

unsigned char* readHeader(unsigned char* buffer, map<unsigned char, string> &codes, int& paddedBits, int &sz)
{
   paddedBits = *((int*)buffer);
   cout<<paddedBits<<"PADDED"<<endl;
   buffer = buffer+4;
   sz-=4;
   int size = *((int*)buffer);
   buffer = buffer+4;
   sz-=4;
   for(int i=0; i<size; i++)
   {    
       unsigned char key = buffer[0];
       buffer++;
       sz--;
       int len = *((int*)buffer);
       buffer+=4;
       sz-=4;
       char* value = (char*)malloc(len+1);

       for(int j = 0; j<len; j++)
       {
           value[j]=buffer[j];
       }
    //    value = (char*)buffer;
       buffer+=len;
       sz-=len;
       value[len]='\0';
       codes[key] = value;
       cout<<key<<" "<<value<<endl;
   }

   return buffer;
}

string getStringFromBuffer(unsigned char* buffer, int sz)
{   //making bytes into bits
    string bitstring = ""; 
    for(int i=0; i<sz; i++)
    {
        bitstring+=FileOperations::toBinary(buffer[i]);
    }

    return bitstring;
}

unsigned char* getDecodedBuffer(string bitstring, vector<unsigned char>&buffer, map<unsigned char, string> codes, int &sz, int paddedBits)
{
    string bit = "";
    map<string, unsigned char> reversecodes;
    
    for(map<unsigned char, string>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        reversecodes[i->second] = i->first;
    }

    for(int i=0; i<bitstring.size()-paddedBits; i++)
    {
        bit+=string(1, bitstring[i]);
        if(reversecodes.find(bit)!=reversecodes.end())
        {
           buffer.push_back(reversecodes[bit]);
           bit = "";
        }
    }

    sz = buffer.size();
    return buffer.data();
}

void decompressFile( char* inputPath,  char* outputPath)
{
    ifstream inputFile(inputPath, std::ios::binary);
    if (!inputFile.is_open()) {
        // Checking whether the compressed file is available or not
        cout << "Error: Compressed file not found at path: " << inputPath << endl;
        cout<<"Decompression Not Successful\n";
        return;
    }

    ifstream outputFile(outputPath);
    if (outputFile.good()) {
        //Checking whether another file with the same output name exists
        cout << "Error: Output file '" << outputPath << "' already exists." << endl;
        cout<<"Decompression Not Successful\n";
        return;
    }

    int sz = 0;
    map<unsigned char, string> codes;
    int paddedBits = 0;
    unsigned char* fileBuffer = FileOperations::readFileIntoBuffer(inputPath, sz);  //reading the compressed file into a buffer
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);     //reading header
    string fileBitString = getStringFromBuffer(fileBuffer, sz);    //converting the buffer of bytes into a binary string representation(bits)
    // cout<<fileBitString<<endl;
    vector<unsigned char> outputBufferV;
    unsigned char* outputBuffer;
    getDecodedBuffer(fileBitString,outputBufferV, codes, sz, paddedBits);   //decoded string stored in vector
    outputBuffer = outputBufferV.data();
    FileOperations::writeFileFromBuffer(outputPath, outputBuffer,sz, 0);  //writing the decoded text file
    //take care of appended zeroes
    cout<<"Decompression Completed Successfully\n";
}

double CompressibilityRatio(const string& originalFilePath, const string& compressedFilePath) {
    ifstream originalFile(originalFilePath, ios::binary | ios::ate);
    ifstream compressedFile(compressedFilePath, ios::binary | ios::ate);

    if (!originalFile.is_open() || !compressedFile.is_open()) {
        cerr << "Error: Failed to open files for calculating compressibility ratio." << endl;
        return -1.0; // Return -1.0 indicating error
    }

    // Calculating the size of the original file
    streamsize originalFileSize = originalFile.tellg();
    originalFile.seekg(0);

    // Calculating the size of the compressed file
    streamsize compressedFileSize = compressedFile.tellg();
    compressedFile.seekg(0);

    // Calculating the compressibility ratio
    double ratio = static_cast<double>(compressedFileSize) / static_cast<double>(originalFileSize);

    originalFile.close();
    compressedFile.close();

    return ratio;
}