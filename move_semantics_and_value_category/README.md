# Value Categories
In Cpp each expression has 2 types of information: type and value category. A type can be anything like int, MyClass, etc. Unlike type which can be a property of a variable, a value category is solely a property of the expression. You cannot say this variable is of following value category, it depends on how it is used in an expression.
Value categories classify cpp expression with 2 core properties: first does this expression has a stable memory address/location and second can we move this expression, i.e. steal its resources safely.

There are 3 types of value categories:
1. prvalues (pure rvalues):  These type of expressions do not have a stable memory address but can be moved safely. E.g. can result of `x+y`, any literals `42` or tempory objects like `MyClass()`.
2. lvalues (locator values): These type of expression has a stable memory location but cannot be moved safely directly (with std::move we can cast them to xrvalues then they can be moved). E.g. `int x = 42` here `x` is a lvalue expression, any string `text` is a lvalue expression in cpp (unlike other literals string literals are special case lvalues).
3. xrvalues (expiring rvalues): These type of expressions has both a stable memory location and can be moved safely. This represents a lvalue that is nearing end of it's lifetime and can be moved safely. E.g. `int y = std::move(x)` the `std::move(x)` is an xrvalue expression.

# Move Semantics
C++11 introduced move semantics. The idea is that instead of creating copies of an object we can transfer the ownership of their memory resources to another object. This helps avoid expensive copies.
E.g. Suppose you have a function creating `std::vector` inside a function and returns it. Normally during return it will first create a copy on new `std::vector` copying all the entries and then it will delete the original `std::vector`. This is a slow process and can be expensive if size of the vector is large. So instead of doing this if we use move semantics the new vector can just copy the base memory address pointer of the old vector (i.e. directly steal the memory resource), and the old vector will now point to `nullptr` and this way an expensive copy changes to a simple stealing of resources (in the form of copying just the base pointer).

Some key elements to implement move semantics involve implementing:
1. Move Constructor: `MyClass(MyClass&& other)` this constructor is called when we want to invoke move semantics to create new object instead of copy. Here you can safely steal the passed in `other` pointer resources.
2. Move Assignment: `MyClass& operator=(MyClass&& other)` same as the constructor but here we invoke move semantics for assignment operator. We can safely steal `other` object resources inside.
3. `std::move`: To call the move constructor or move assignment operator we have to cast the object in rvalue reference type for compiler to know that it should call move operations not copy, `std::move` allows you to do that. E.g. `Myclass y = std::move(x)` will call move constructor.

# RVO/NRVO (Return Value Optimization/Named Return Value Optimization)
RVO/NRVO is a compiler optimization which allows creating the object directly in the parent function call to avoid extra copies/moves.
RVO/NRVO has existed in C++ since C++98. But since C++17 RVO is mandatory (i.e. even if copy operator & move operators are deleted you will still get RVO). NRVO is still optional but most compilers will do it for higher optimization flags.

RVO e.g.:
```cpp
// A simple class that logs its function calls
struct MyObject {
    MyObject() {
        std::cout << "CONSTRUCTOR called (default)\n";
    }
    
    // Copy Constructor
    MyObject(const MyObject& other) {
        std::cout << "COPY CONSTRUCTOR called\n";
    }

    // Move Constructor
    MyObject(MyObject&& other) noexcept {
        std::cout << "MOVE CONSTRUCTOR called\n";
    }

    ~MyObject() {
        std::cout << "DESTRUCTOR called\n";
    }
};

MyObject createObject() {
    std::cout << "--- Inside createObject() ---\n";
    // This is the classic RVO case. We are returning an unnamed temporary object (a "prvalue").
    return MyObject(); 
}

int main() {
    std::cout << "--- Calling createObject() ---\n";
    // The object returned from createObject() is used to initialize 'obj'.
    MyObject obj = createObject();
    std::cout << "--- After createObject() ---\n";
    return 0;
} 
```

The output of the following will be:
```
--- Calling createObject() ---
--- Inside createObject() ---
CONSTRUCTOR called (default)
--- After createObject() ---
DESTRUCTOR called
```

As you can see although the object is created in side `createObject()` function we don't see any copy operation. We also don't see any destructor of temporary `MyObject()` inside the `createObject()`. This happens because the compiler was able to identify that we can directly create the `MyObject()` in `obj` memory using RVO and did exactly that. This thus help avoid all the extra copies, constructor and deletion.

NRVO is exactly the same but with named variables:
```cpp
// A function that creates and returns a NAMED MyObject
MyObject createObjectNRVO() {
    std::cout << "--- Inside createObjectNRVO() ---\n";
    
    // 1. We create a NAMED local variable
    MyObject namedObj; 
    
    // We could do more work on namedObj here...
    // e.g., namedObj.setValue(123);
    
    // 2. We return the NAMED variable
    return namedObj; 
}

int main() {
    std::cout << "--- Calling createObjectNRVO() ---\n";
    
    // The object returned from createObjectNRVO() is
    // used to initialize 'obj'.
    MyObject obj = createObjectNRVO();
    
    std::cout << "--- After createObjectNRVO() ---\n";
    return 0;
}
```

Output will be same as previous:
```
--- Calling createObjectNRVO() ---
--- Inside createObjectNRVO() ---
DEFAULT CONSTRUCTOR called
--- After createObjectNRVO() ---
DESTRUCTOR called
```

## Move Semantics vs RVO
Sometimes you may think that using `std::move()` while returning an object will be more benefical then just normal return but this can potentially lead to you not having RVO and thus slower performance.
Following examples shows the same:

```cpp
// The same class that logs its function calls
struct MyObject {
    MyObject() {
        std::cout << "DEFAULT CONSTRUCTOR called\n";
    }
    
    MyObject(const MyObject& other) {
        std::cout << "COPY CONSTRUCTOR called\n";
    }

    MyObject(MyObject&& other) noexcept {
        std::cout << "MOVE CONSTRUCTOR called\n";
    }

    ~MyObject() {
        std::cout << "DESTRUCTOR called\n";
    }
};

// --- Version 1: Good (Allows NRVO) ---
MyObject createObjectWithNRVO() {
    std::cout << "--- Inside NRVO version ---\n";
    MyObject namedObj;
    // Just return the named object. The compiler will elide the copy/move.
    return namedObj;
}

// --- Version 2: Bad (Prevents NRVO) ---
MyObject createObjectWithMove() {
    std::cout << "--- Inside std::move version ---\n";
    MyObject namedObj;
    // This is the anti-pattern! It prevents NRVO.
    return std::move(namedObj); 
}

int main() {
    std::cout << "--- 1. Testing NRVO (Good) ---\n";
    MyObject obj1 = createObjectWithNRVO();
    std::cout << "--- End of NRVO Test ---\n\n";

    std::cout << "--- 2. Testing std::move (Bad) ---\n";
    MyObject obj2 = createObjectWithMove();
    std::cout << "--- End of std::move Test ---\n\n";
    
    return 0;
}
```

The output will be:
```
--- 1. Testing NRVO (Good) ---
--- Inside NRVO version ---
DEFAULT CONSTRUCTOR called
--- End of NRVO Test ---

--- 2. Testing std::move (Bad) ---
--- Inside std::move version ---
DEFAULT CONSTRUCTOR called
MOVE CONSTRUCTOR called
DESTRUCTOR called
--- End of std::move Test ---

DESTRUCTOR called
DESTRUCTOR called
```

As you can see NRVO normal case will call the constructor only once but when we use `std::move` we call the constructor twice (default constructor and move constructor) and we also have an additional destructor call. This happens because by calling `std::move()` we can changing the return type of the object to an xrvalue and the compiler cannot optimize using RVO in this case.
It is one of the anti-patterns that avoids RVO and leads to lesser performant code.
Thankfully it is easy to catch this pattern in your code with flag `-Wpessimizing-move` which will give an error when RVO/NRVO could have happened but was skipped due to this pattern.

Overall based on the best optimization we have:
1. RVO/NRVO is the best.
2. Following this we have `std::move` which does call extra constructor/destructor but could be faster.
3. Normal copy constructor routine.

*Note: Although RVO/NRVO seems like a great optimization it is still a micro optimization and it is better to first identify your code bottlenecks before doing any optimization. Future PRs should keep this in mind but going to change the exsisting code will not lead to better performance always. Profile before changing.*