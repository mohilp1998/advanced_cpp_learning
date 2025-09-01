# Smart Pointers
Smart pointers are basically classes designed to wrap a pointer which allows things like auto deletion when no longer in scope, reference counting and single ownership as needed. They allow automatic memory management, proper deallocation and avoiding memory leaks.

## unique_ptr
`std::unique_ptr` is a smart pointer that uses exclusive ownership semantics. It ensures only a single owner change exists for a pointer, does not allow copy operation, only move operation is allowed.

## shared_ptr
`std::shared_ptr` is a smart pointer which works on shared ownership semantics. It keeps a reference count for all the pointers pointing to the object and deletes the object once the reference count reaches 0.
Interesting note: Incrementing and decrementing reference count is thread safe, but access to base underlying object is not thread safe.

## weak_ptr
`std::weak_ptr` is a smart pointer with "non-owning" reference to an object managed by `std::shared_ptr`. `std::weak_ptr` allow breaking cyclic dependencies making sure we can delete objects in case of cyclic shared ptr dependencies.
To access a weak_ptr, you need to use `weak_ptr.lock()`, which converts it to a shared_ptr for the duration thus allowing access to the object. `.lock()` will return null ptr if base `shared_ptr` has been deleted.

## auto_ptr
`std::auto_ptr` used to exist in C++98 and C++03 standard but was deprecated in C++11. This used to work similar to `std::unique_ptr` in the sense that only one object can have ownership of the pointer.
But unlike unique_ptr in auto_ptr using assignment operator (`auto p1 = p2;`) will lead to transfer of ownership. It was depracted as auto_ptr is not compatible with STL due to transfer of ownership, plus this transfer can lead to dangerous errors.


# RAII (Resource Acquisition Is Initialization)
RAII is a C++ programming technique where resource life cycle is binded with object life cycle. This boils down to using object constructor to acquire the resource & object destructor to delete the resource. The resource is also always used via this object
thus ensuring that object life cycle and resource life cycle matches perfectly.

Doing this allows user to avoid doing manual code for many cases, for e.g. if we use simple `std::mutex.lock` and `std::mutex.unlock`, we will have to `unlock` a mutex all cases like exception, early return, etc. This is error prone and a better way is to
use `std::lock_guard`. What `lock_guard` does is on construction of the object it automatically acquires the lock and when object goes out of scope the destructor will destroy the object for us, thus never needing use to make sure we do `unlock` everywhere.

Refer to the example code of RAII for file handle to see how it is done.


# UniquePtr Class Implementation
Check out UniquePtr class implementation. This class replicated the functionality of `std::unique_ptr`. The details about the requirements and problem statement is in the `implement_unique_ptr.cpp` class.