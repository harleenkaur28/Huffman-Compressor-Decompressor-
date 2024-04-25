# Huffman-Compressor-Decompressor-

This project is a file compression and decompression tool developed using the Huffman coding algorithm. Huffman coding is a widely used algorithm for lossless data compression, particularly in text and file compression applications.

Features:
  Compress text files using Huffman coding algorithm.
  Decompress compressed files to restore the original text.
  Calculate the compressibility ratio to measure the effectiveness of compression.
  Command-line interface for easy usage.
  Portable and platform-independent.

Description of the Files:
  There are 3 header files, compress.hpp, decompress.hpp, file_operations.hpp. 
  File_operations.hpp contains functions which are used in both compress.hpp and decompress.hpp .
  myfile.txt and temp.txt are 2 sample text files.

Usage:
  In 'main.cpp', enter the name of the text file you want to compress in the inputFilePath.
  Enter the name of the decompressedFilePath. 
  Run the code.

Drawbacks:
   As the size of the input text file grows, the compression time also increases proportionally. 
   Additionally, compressing small-sized files may result in a larger compressed file size.
