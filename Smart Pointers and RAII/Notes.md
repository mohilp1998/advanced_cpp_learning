# Smart Pointers
Smart pointers are basically classes designed to wrap a pointer which allows things like auto deletion when no longer in scope, reference counting and single ownership as needed. They allow automatic memory management, proper deallocation and avoiding memory leaks.

## unique_ptr
`std::unique_ptr` is a smart pointer that uses exclusive ownership semantics. It ensures only a single owner change exists for a pointer, does not allow copy operation, only move operation is allowed.

## shared_ptr
`std::shared_ptr` is a smart pointer which works on shared ownership semantics. It keeps a reference count for all the pointers pointing to the object and deletes the object once the reference count reaches 0.
Interesting note: Incrementing and decrementing reference count is thread safe, but access to base underlying object is not thread safe.