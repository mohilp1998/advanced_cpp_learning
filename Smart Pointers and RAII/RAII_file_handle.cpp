/*
Wrap a raw FILE* (fopen/fclose) inside a C++ class that:
  - Opens a file in the constructor.
  - Closes the file in the destructor.
  - Forbids copy (to avoid double-close).
  - Provides a simple method to write text into the file.

*/

/* g++ -std=c++14 -Wall RAII_file_handle.cpp */

#include <cstdio>
#include<iostream>
#include <string>
#include <stdexcept>

class MyFile {
  public:
    FILE * file;

    MyFile(const std::string& filename, const std::string& mode)
    {
      file = std::fopen(filename.c_str(), mode.c_str());
      if (!file)
      {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      std::cout << "File: " << filename << " opened\n";
    }

    ~MyFile()
    {
      if (file)
      {
        std::fclose(file);
      }

      std::cout << "File closed\n";
    }

    void writeString(const std::string data)
    {
      int rc = std::fputs(data.c_str(), file);
      if (rc == EOF)
      {
        throw std::runtime_error("Failed to write");
      }

      std::cout << "File written\n";
    }

    // Delete copy constructor and assignment operator to ensure copy won't happen
    MyFile(const MyFile&) = delete;
    MyFile& operator= (const MyFile&) = delete;
};

int main()
{
  MyFile test1("a.txt", "w");
  test1.writeString("abc");
  return 0;
}