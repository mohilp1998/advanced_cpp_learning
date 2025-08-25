/***************** PROBLEM STATEMENT *****************/
/*
You are designing a small system to track library books.
Each book is represented by a Book class. Multiple readers can borrow (share) the same book at the same time.
Requirements:
  - Define a Book class with:
    - a title (string)
    - a constructor and destructor (print messages when they are called).
    - a method read() that prints "Reading <title>".

  - Use std::shared_ptr<Book> to:
    - Create a new book ("C++ Primer").
    - Create multiple readers (reader1, reader2, …) sharing the same book.
    - Print the reference count (use_count()) after each reader borrows the book.
    - Have one reader give up the book (reset), and print the reference count again.

  - When the last shared_ptr is destroyed, the book should be automatically deleted.
*/

/* g++ -std=c++14 -Wall test_shared.cpp */
#include<iostream>
#include<string>
#include<memory>

class Book {
  public:
    std::string title;
    
    Book(std::string name) {
      title = name;
      std::cout << "Book '" << title << "' created\n"; 
    }

    ~Book() {
      std::cout << "Book '" << title << "' destroyed\n"; 
    }

};

int main() {
  // Book created and reader1 reference count added
  std::shared_ptr<Book> reader1 = std::make_shared<Book>("C++ Primer");
  std::cout << "Reader1 borrowed the book, reference count = " << reader1.use_count() << std::endl;

  // Adding one more reference
  std::shared_ptr<Book> reader2 = reader1;
  std::cout << "Reader2 borrowed the book, reference count = " << reader2.use_count() << std::endl;

  // Adding one more reference
  std::shared_ptr<Book> reader3 = reader2;
  std::cout << "Reader3 borrowed the book, reference count = " << reader3.use_count() << std::endl;

  // Removing one of the reference
  reader2.reset();
  if (reader2 == nullptr)
  {
    // Notice the use count is using reader1 as reader2 will be nullptr now
    std::cout << "Reader2 returned the book, reference count = " << reader1.use_count() << std::endl;
  }

  // Finally the objects gets deleted
  return 0;
}
