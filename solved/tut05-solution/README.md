# Tutorial 5

## Question 1

Look at `source/subscript.cpp`

* Make this code const-correct so it can compile and run sucessfully
* In what cases would we need to overload an operator for its const or non-const version?

> Look at `source/subscript.cpp` on the soutions branch.

## Question 2

Why is prefix increment/decrement usually preferred to postfix increment/decrement? When would we use postfix?

* Prefix is generally preferred (when no copy is needed) due to one less step (a copy and return) being completed. Therefore at scale this operation will yield finite performance improvements
* Postfix is used when you want to receive a copy of the object's current state prior to the increment/decrement occuring.

## Question 3

What is *stack unwinding*?

> The process of exiting the stack frames until we find an exception handler for the function.
  
What happens during stuck unwinding?

> Destructors are called on relevant objects on the way out; and any exceptions thrown in a destructor (that are not caught and handled in that destructor) cause `std::terminate` to be called

What issue can this potentially cause? How can we fix this?

> If unmanaged resources were created before an exception throw, they may not be appropriately released. The solution is to ensure that every resource is owned by either another resource, the stack, or an unnamed temporary variable.

## Question 4

What are the 4 exception safety levels and what does each mean?

> * No-throw (failure transparency): Operations guaranteed to succeed
> * Strong exception safety (commit-or-rollback): Operations can fail, but failed operations will have no side effects
> * Weak exception safety (no-leak): May cause side effects, but guaranteed that no memory leaks will occur
> * No exception safety: No guarantees

## Question 5

Look at `source/rethrow.cpp`

This program currently catches the exception fine within the make_connection function. However, we want the client (the main function) to also log it's own error message "Could not establish connection". How can we use rethrow to achieve this?

> See `source/rethrow.cpp` on the soutions branch.

> When you aren't interested in managing a caught exception (wanting to leave that to a higher level), but you would like to log something to do with the exception

## Question 6

In what cases would we want to have two catch statements for a thrown exception?

> Discuss `source/multicatch.cpp` on the soutions branch.

> When we have some type-specific exceptions to catch (e.g. `std::out_of_range`), but then also want to handle other exceptions (known or unknown) in other cases (e.g. `std::length_error`, `std::exception`).

## Question 7

In what case would we want to define our own exception? Give an example, and explain why we would.

Are there ways to define our own exception without reinvesting the wheel?

> Yes, by using inheritance with exception classes

> See `source/inversedomain.cpp` on the soutions branch.

## Question 8

Look at `source/catch.cpp`

What is wrong with this code?

> See `source/catch.cpp` on the soutions branch.