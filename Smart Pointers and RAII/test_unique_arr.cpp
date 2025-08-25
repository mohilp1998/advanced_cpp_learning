/***************** PROBLEM STATEMENT *****************/
/*
Write a C++ program that:
  - Dynamically creates an array of integers (size 5) using std::unique_ptr<int[]>.
  - Fills the array with the squares of numbers 1–5.
  - Prints the array.
  - Transfers ownership of the array to another unique_ptr and demonstrates that the original pointer no longer owns it.
*/
/* g++ -std=c++14 -Wall test_unique_arr.cpp */

#include<iostream>
#include<memory>

int main() {
  // unique_ptr syntax changes slightly when working with initializing a dynamic array
  // Can use std::make_unique here also and ideally should use it.
  std::unique_ptr<int[]> arr_int(new int[5]);
  for (int i=0; i<5; i++)
  {
    arr_int[i] = i*i;
  }

  // The dynamic array implementation allows us to use [] operator
  std::cout << "Array elements are: ";
  for (int i=0; i<5; i++)
  {
    std::cout << arr_int[i] << " ";
  }
  std::cout << std::endl;

  // Move stills works as done previously
  std::unique_ptr<int[]> new_arr_int = std::move(arr_int);
  if (arr_int == nullptr)
  {
    std::cout << "After move, original pointer is empty\n";
  }

  std::cout << "Transferred pointer prints: ";
  for (int i=0; i<5; i++)
  {
    std::cout << new_arr_int[i] << " ";
  }
  std::cout << std::endl;

  // The dynamic array will get deleted here
  return 0;
}