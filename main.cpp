#include<bits/stdc++.h>
#include "compress.hpp" // Including the header file for compression functions
#include "decompress.hpp" // Including the header file for decompression functions
using namespace std;

int main()
{   
    // Define input and output file paths
      char* inputFilePath = "temp.txt";
      char* compressedFilePath = "compressed2.bin";
      char* decompressedFilePath = "decompress2.txt";

    // Compression Function
    map<unsigned char,string> codes; // Map to store Huffman codes
    compressFile(inputFilePath, compressedFilePath, codes);

    // Decompression Function 
    decompressFile(compressedFilePath, decompressedFilePath);

    cout << "Ratio:";
    cout<<CompressibilityRatio(inputFilePath, compressedFilePath);
    return 0;
}
