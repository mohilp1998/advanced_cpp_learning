/***************** PROBLEM STATEMENT *****************/
/* 
Implement a C++ class MyString that manages a dynamic C-style string (char*). 
The implementation must correctly follow the Rule of Five(Destructor, Copy Constructor, Copy Assignment Operator, Move Constructor, and Move Assignment Operator) to handle resource management.

Following are the requirements:
- Private Members:
  - char* m_data: Ptr to heap-allocated character array
  - size_t m_size: size of the string
- Constructors:
  - MyString(const char* str = ""): Default constructor
  - MyString(const MyString& other): Copy constructor doing a deep copy.
  - MyString(MyString&& other) noexcept: Move constructor.
- Assignment Operator:
  - MyString& operator=(const MyString& other): Copy Assignment operator
  - MyString& operator=(MyString&& other) noexcept: Move assignment operator
- Destructor:
  - ~MyString(): Destructor
- Helper Methods:
  - const char* c_str() const: Returns the pointer to internal data
  - size_t size() const: Returns the size
  - friend std::ostream& operator<<(std::ostream& os, const MyString& str): Allow cout string operator
*/

/* g++ -std=c++14 -Wall -g string.cpp */

#include<iostream>
#include <cstring>

class MyString
{
  private:
    char *m_data;
    size_t m_size;

  public:
    
    MyString(const char* str = "")
    {
      if (str == nullptr)
      {
        m_data = nullptr;
        m_size = 0;
      }

      m_size = std::strlen(str);
      m_data = new char[m_size+1]; //+1 for ending '\0'
      std::strcpy(m_data, str);
    }

    ~MyString()
    {
      if (m_size != 0)
      {
        delete[] m_data;
      }
    }

    MyString(const MyString& other)
    {
      m_size = other.m_size;
      
      if (m_size != 0)
      {
        m_data = new char[m_size+1]; //+1 for ending '\0'
        std::strcpy(m_data, other.c_str());
      }
      else
      {
        m_data = nullptr;
      }
    }

    MyString(MyString&& other) noexcept
    {
      m_size = other.size();
      m_data = other.m_data;

      other.m_data = nullptr;
      other.m_size = 0;
    }

    MyString& operator=(const MyString& other)
    {
      // self assignment
      if (this == &other)
      {
        return *this;
      }

      // Clear out exsisting resource else we will get memory leak
      if (m_size != 0)
      {
        delete[] m_data;
      }

      m_size = other.size();
      
      if (m_size != 0)
      {
        m_data = new char[m_size+1];
        std::strcpy(m_data, other.c_str());
      }
      else
      {
        m_data = nullptr;
      }

      return *this;
    }

    MyString& operator=(MyString&& other) noexcept
    {
      // self assignment
      if (this == &other)
      {
        return *this;
      }

      // Clear out exsisting resource else we will get memory leak
      if (m_size != 0)
      {
        delete[] m_data;
      }

      m_size = other.size();
      m_data = other.m_data;

      other.m_data = nullptr;
      other.m_size = 0;

      return *this;
    }

    size_t size() const
    {
      return m_size;
    }

    const char* c_str() const
    {
      return m_data;
    }

    // Although implementing inside this class definition the friend function  
    // is part of std::ostream class.
    friend std::ostream& operator<<(std::ostream& os, const MyString& str)
    {
      if (str.m_data != nullptr)
      {
        os << str.m_data;
      }
      return os;
    }
};

// --- Test Program ---
int main() {
    std::cout << "--- 1. Constructor ---" << std::endl;
    MyString s1("Hello");
    std::cout << "s1: " << s1 << " (Size: " << s1.size() << ")" << std::endl;

    MyString empty;
    std::cout << "empty: \"" << empty << "\" (Size: " << empty.size() << ")" << std::endl;

    std::cout << "\n--- 2. Copy Constructor ---" << std::endl;
    MyString s2 = s1; // Calls Copy Constructor
    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s2: " << s2 << std::endl;

    std::cout << "\n--- 3. Copy Assignment ---" << std::endl;
    MyString s3("World");
    std::cout << "Before s3 = s2:" << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    s3 = s2; // Calls Copy Assignment
    std::cout << "After s3 = s2:" << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    
    std::cout << "\n--- 4. Self-Assignment ---" << std::endl;
    std::cout << "Before s1 = s1: " << s1 << std::endl;
    s1 = s1; // Test self-assignment
    std::cout << "After s1 = s1: " << s1 << std::endl;

    std::cout << "\n--- 5. Move Constructor ---" << std::endl;
    std::cout << "Before move, s1: " << s1 << std::endl;
    MyString s4 = std::move(s1); // Calls Move Constructor
    std::cout << "After move:" << std::endl;
    std::cout << "s4: " << s4 << std::endl;
    std::cout << "s1: \"" << s1 << "\" (Size: " << s1.size() << ")" << std::endl;

    std::cout << "\n--- 6. Move Assignment ---" << std::endl;
    std::cout << "Before move, s2: " << s2 << std::endl;
    MyString s5("Temporary");
    std::cout << "Before move, s5: " << s5 << std::endl;
    s5 = std::move(s2); // Calls Move Assignment
    std::cout << "After move:" << std::endl;
    std::cout << "s5: " << s5 << std::endl;
    std::cout << "s2: \"" << s2 << "\" (Size: " << s2.size() << ")" << std::endl;

    std::cout << "\n--- End of Scope (Destructors called) ---" << std::endl;
    return 0;
}

