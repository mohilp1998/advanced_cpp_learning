/*
Problem Statement: Implement UniquePtr<T> which mimic std::unique_ptr

Following are the requirements:
  - Disallow copy and allow move:
      UniquePtr<int> p1(new int(5));
      UniquePtr<int> p2 = p1; //Error
      UniquePtr<int> p2 = std::move(p1); //Allowed
  
  - Class with Default Constructor Null Pointer or Constructor to accept raw pointer
      UniquePtr<int> p1; // Default constructor, holds nullptr
      UniquePtr<int> p2(new int(42)); // Takes ownership of the new int
    
  - Destructor deletes the raw pointer associated memory

  - Deference and arrow operator should be supported
      struct MyStruct { void say_hello() {  ... } };
      UniquePtr<MyStruct> ptr(new MyStruct());
      ptr->say_hello(); // Uses operator->
      MyStruct& ref = *ptr; // Uses operator*
  
  - Implement Core Member Functions:
    - get(): Get raw pointer
    - release(): Relinquesh ownership of raw pointer. And make object null.
    - reset(T* ptr = nullptr): Destroy current object and assign the new ptr (empty if not provided)

  - A UniquePtr should be usable in a boolean context to check if it owns a pointer.
      UniquePtr<int> p(new int(99));
      if (p) { // This uses operator bool()
          // This block will execute
      }
*/

/* g++ -std=c++14 -Wall implement_unique_ptr.cpp */

#include<iostream>

template <typename T>
class UniquePtr {
  private:
    T * objPtr;

    // Delete copy constructor
    UniquePtr(const UniquePtr&) = delete;
    // Delete copy assignment operator
    UniquePtr& operator=(const UniquePtr&) = delete;

  public:
    // Basic constructor
    UniquePtr(){
      objPtr = nullptr;
    }

    // Pointer passing constructor
    UniquePtr(T * ptr){
      objPtr = ptr;
    }

    // Delete object on destructor
    ~UniquePtr(){
      delete objPtr;
    }

    // Move assignment operator
    UniquePtr& operator=(UniquePtr&& other) {
      // Check if not self assignment
      if (this != &other)
      {
        // Release current obj if not empty
        delete objPtr;

        // Reassign the ownership of moved pointer here
        objPtr = other.objPtr;

        // Move the other object ptr null
        other.objPtr = nullptr;
      }

      // Return reference to this object to allow chained operation
      return *this;
    }

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept {
      objPtr = other.objPtr;
      other.objPtr = nullptr;
    }

    // Arrow Operator: A simple statement like ptr->data() is converted to
    // (ptr.operator->())->data(). which means we are basically doing a call to operator->()
    // function and will use the returned value for further processing.
    // operator->() expects a pointer return.
    T* operator->() const {
      return objPtr;
    }

    // Deference operator: Similar to above arrow operator. This one looks like
    // (*ptr).data() as (ptr.operator*()).data()
    T& operator*() const {
      return *objPtr;
    }

    T* get() {
      return objPtr;
    }

    void release() {
      delete objPtr;
      objPtr = nullptr;
    }

    void reset(T * newPtr = nullptr) {
      delete objPtr;
      objPtr = newPtr;
    }

    // Now we can use this directly in if(uniquePtrObj)
    explicit operator bool() noexcept {
      return (objPtr != nullptr);
    }
};

struct MyStruct { 
  int number;
  void say_hello() { 
    std::cout << "Hello from MyStruct\n";
  } 
};

int main() {

  UniquePtr<int> p1; // nullptr constructor
  if (p1.get() == nullptr)
  {
    std::cout << "Working nullptr constructor\n";
  }

  UniquePtr<int> p2(new int(5)); // Object Constructor
  if (p2) // using bool check operator
  {
    std::cout << "Checking deference operator: " << (*p2) << std::endl;
  }

  UniquePtr<int> p3 = std::move(p2); // Move constructor call
  if(!p2)
  {
    std::cout << "p2 correctly emptied out and p3 is " << (*p3) << std::endl;
  }

  p1 = std::move(p3); // Move operator call
  if(!p3)
  {
    std::cout << "p3 correctly emptied out and p1 is " << (*p1) << std::endl;
  }

  p3.reset(new int(6));
  if (p3)
  {
    std::cout << "p3 is " << (*p3) << std::endl;
  }

  p1.release();
  if (p1.get() == nullptr)
  {
    std::cout << "p1 released correctly\n";
  }

  UniquePtr<MyStruct> m1(new MyStruct());
  m1->say_hello();
  (*m1).number = 10;
  std::cout << "My Struct number is " << m1->number << std::endl;

  return 0;
}