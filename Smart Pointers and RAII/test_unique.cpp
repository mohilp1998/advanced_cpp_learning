/***************** PROBLEM STATEMENT *****************/
/*
Write a C++ program that:
  - Defines a simple Car class with:
    - A constructor that takes a std::string for the car’s name and prints "Car <name> created".
    - A destructor that prints "Car <name> destroyed".
    - A method drive() that prints "Car <name> is driving".
  - In main():
    - Create a std::unique_ptr<Car> managing a Car object (use std::make_unique if available).
    - Call the drive() method through the smart pointer.
    - Transfer ownership of the car to another unique_ptr using std::move.
    - Show that the original pointer no longer owns the object.
*/
/* g++ -std=c++14 -Wall test_unique.cpp */

#include <iostream>
#include <string>
#include <memory>

class Car {
  public:
  std::string name;

  Car(std::string car_name) {
    name = car_name;
    std::cout << "Car " << name << " created\n";
  }

  ~Car()
  {
    std::cout << "Car " << name << " destroyed\n";
  }

  void drive()
  {
    std::cout << "Car " << name << " is driving\n";
  }

};


int main() {
  // Create a new unique_ptr. Can use either new Car("Tesla") or make_unique
  // We have make_unique instead of just using new because:
  // 1. Exception Safety: Things like func(std::unique_ptr<T>(new T), something()), it is possible that new T is called first
  //    then something() is called which leads to exception. In this case new T is now a memory leak.
  // 2. Also help use auto now: auto p = std::make_unique<T>(...). With new type deduction cannot happen, but still this one
  //    is just a shift in where syntax is written. Although some may argue it's cleaner syntax.
  std::unique_ptr<Car> my_car = std::make_unique<Car>("Tesla");
  
  // See it working
  my_car->drive();

  // This will give error as unique_ptr cannot be copied
  // std::unique_ptr<Car> new_car = my_car;

  // Move the unique_ptr to new_car
  std::unique_ptr<Car> new_car = std::move(my_car);

  // Verify move happened and old pointer is empty
  if (my_car == nullptr)
  {
    std::cout << "Original pointer is empty\n";
  }

  // Make sure the new_car is the working
  new_car->drive();

  // The new created object will only get deleted here
  return 0;
}