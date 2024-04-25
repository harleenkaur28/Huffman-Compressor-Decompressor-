#ifndef FILE_OPERATIONS_HPP
#define FILE_OPERATIONS_HPP

#include <bits/stdc++.h>
#include <cstdio> // Include for FILE and related functions

namespace FileOperations {

using namespace std;

string toBinary(unsigned char a)
{
    string output = "";
    while (a != 0)
    {
        string bit = (a % 2 == 0) ? "0" : "1";
        output += bit;
        a /= 2;
    }

    if (output.size() < 8)
    {
        int deficit = 8 - output.size();
        for (int i = 0; i < deficit; i++)
        {
            output += "0";
        }
    }

    reverse(output.begin(), output.end());
    return output;
}

unsigned char *readFileIntoBuffer(const char *path, int &sz)
{
    FILE *fp = fopen(path, "rb");
     if (fp == nullptr) {
        // Throw an exception if the file cannot be opened
        cout<<"Error: Unable to open input file '" + string(path) + "'.";
        return nullptr;
    }
    sz = 0;
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(sz);
    fread(buffer, 1, sz, fp);
    fclose(fp); // Close the file after reading
    return buffer;
}

// Declare writeFileFromBuffer function prototype
void writeFileFromBuffer(const char *path, unsigned char *buffer, int sz, int flag)
{

    FILE *fp;
    if (flag == 0)
    {
        fp = fopen(path, "wb");
    }
    else
    {
        fp = fopen(path, "ab");
    }

    fwrite(buffer, 1, sz, fp);
    fclose(fp); // Close the file after writing
}

} // namespace FileOperations

#endif // FILE_OPERATIONS_HPP
