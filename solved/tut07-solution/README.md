# Tutorial 7

## Question 1

Why do we use smart pointers instead of raw pointers whenever possible?

> Smart pointers mean:
> * We do not have to worry about deleting our heap memory
> * Move and copy semantics are handled for us

## Question 2

What is an example of a circumstance a std::shared_ptr<T> would be preferred over a std::unique_ptr<T>?

> In general, you can have it when either:
> * There is shared ownership (very rare, it's usually one owner and many observers).
> * There is a single owner, but the observers may stick around longer than the owner.    
> When we have ownership loops (eg. a graph that isn't a dag), we have one of two options:
> * Preferred: have something like std::vector<std::unique_ptr<Node>>, and each node stores raw pointers to other nodes. This may not be possible if it's hard to find all the raw pointers and remove them when you delete the unique pointer.
> * Otherwise: Have something like std::vector<std::shared_ptr<Node>>, and each node stores weak pointers to other nodes (DO NOT store shared pointers in each node - the nodes will never get removed)

## Question 3
Note: The parameter type is the type used in the method signature, the argument type is what gets passed to it.

In the following code:
* Which argument types can be converted to which parameter types? Which cannot be converted? What is the rationale?
  * Consider the argument and parameter types `T&`, `const T&`, and `T&&`
  * A `const T&` argument cannot be converted to a `T&` or `T&&`, as that would remove its const.
  * A `T&`:
    * May be converted to a `const T&`, as it's always ok to add const.
    * Must be explicitly converted to a `T&&` with std::move (since passing an lvalue into a parameter explicitly requiring an rvalue may not be intended)
  * A `T&&`
    * May not be converted to a `T&`, as the function will be modifying something that won't be around once it's complete, so it couldn't do anything useful.
    * May be converted to a `const T&`, as this won't suffer the same issues
* What is the type of the arguments to each of these, and how do they get converted to the type of the parameters?
  * eg. Can an rvalue argument get passed into an lvalue parameter? Vice versa?
  * The `"hello"` argument: `const char*&&`, constructs a std::string rvalue and passes that in ()
  * The `s` argument: `std::string&`, no work required to convert, except `value`, which copy-constructs the argument.
  * The `std::move(s)` argument: `std::string&&`, no work required to convert, except `value`, which move-constructs the argument
  * The `s2` argument: const char\*&, constructs a std::string rvalue and passes that in.
* Based on the previous two questions, which of the following lines will compile, and which will not?

```txt
void c_lvalue(const std::string& lvalue);
void lvalue(std::string& lvalue);
void rvalue(std::string&& rvalue);
void value(std::string value);

c_lvalue("hello");
lvalue("hello"); // Won't compile
rvalue("hello");
value("hello");
std::string s = "world";
c_lvalue(s);
lvalue(s);
rvalue(s); // Won't compile
value(s);
c_lvalue(std::move(s));
lvalue(std::move(s)); // Won't compile
rvalue(std::move(s));
value(std::move(s));
auto s2 = "goodbye";
c_lvalue(s2);
lvalue(s2); // Won't compile
rvalue(s2);
value(s2);
```

## Question 4

Look at `source/instant.h`

How many instantiations of max are generated?

> 4

## Question 5

What is wrong with the implementation in `source/stack.h`, `source/stack.cpp`, `source/stack_use.cpp`?
  
> Template definitions must be in the .h file because files using templates must be able to generate the code at compile time (as opposed to link time)

## Question 6

Look at `source/stringqueue.h`

Convert the following class to a generic (templated) type. After converting it to a generic type:
 * Complete all incomplete implementations.
 * Add an appropriate default constructor
 * Add an appropriate copy constructor

> See `solutions/stringqueue.h`

Finally, make the size of this queue a non-type parameter.

> See `solutions/stringqueuenontype.h`

What are the advantages and disadvantages of the non-type parameter?

> *Advantage:* Improved performance. Size of a queue is determined at compile time requiring less to be done / processed at runtime.
> *Disadvantage:* (1) Code explosion - instantiation created for a queue of every size. (2) Unable to copy construct easily</p>

## Question 7

Which part of this graph are:
 * Edges
 * Nodes
 * Weightings

  https://i.imgur.com/gCDHqrD.png

## Question 8

Look at `source/constexpr.cpp`.

Rewrite the code so that the look up table is generated at compile time, and remove magic numbers.

What are the benefits of `constexpr`? When might you use it? What are some of the downsides?

> See `source/constexpr.cpp` on the solutions branch.
>
> `constexpr` allows performing complex calculations at compile time instead of runtime,
> which can improve runtime performance.
>
> It can also be used to supply values to non-type template parameters.
>
> The downside is that compile times are worse,
> because the work now needs to be done and checked by the compiler,
> which is generally slower than a native program.
> In addition, you can't run a debugger on a compile-time function;
> you must run the function at runtime to be able to use a debugger.
