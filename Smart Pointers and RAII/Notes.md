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