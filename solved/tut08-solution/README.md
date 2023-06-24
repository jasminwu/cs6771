# Tutorial 8

## Question 1

What is iterator invalidation? Why does it occur?

> Iterator invalidation occurs when a mutable operation changes the state of a container in such a
> way that any previous iterators for the container are no longer valid. This may occur simply
> because the object no longer exists (eg. `std::set::erase`), because the iterator was stored
> internally as a pointer and the object has been moved (eg. `std::vector`), or for some other
> reason.

## Question 2

Assignment 3 involves abstracting a container of containers as a single container. Consider a simple
example of `std::vector<std::vector<int>>`. Abstracting the container requires storing the outer
iterator and the inner iterator. Discuss potential issues with this idea, and once you think you've
discovered all of them, discuss how you might solve these issues.

> * Comparing end iterators (no inner iterator exists, and trying to access one is undefined)
> * ++ operator on an empty vector in the middle
> * begin() with an empty vector at the start
> * Solution: Store a sentinel value (you need to know when it's the end, so you can know when not
>            to check the inner iterator).

## Question 3

Look at `source/rope_user.cpp` and `source/rope.hpp`

We have defined a rope class as a bunch of strings tied together, and have defined a basic class and
some starter code using it.

1. Write an input iterator for this class so that you can uncomment lines 65-72. You can use the
   `static_assert`s that we provide to check you're on the right path.

> See `rope_q31.hpp` and `rope_user_q31.cpp`.

2. Discuss two ways we might modify our code to allow both a const and a non-const iterator. What
   are the advantages and disadvantages for each?

> The first, and most naïve way is to create an `iterator` type and a `const_iterator` type. This
> will yield simple and easy to read code, but at the cost of near-100% duplication. Any changes to
> `iterator` will yield a change to `const_iterator`, and the testing will also need to be
> completely duplicated.
>
> The second, and more robust way, is to parameterise the constness of the iterator. This minimises
> the amount of code duplication, but at a minor cognitive burden (we need to remember to do this
> for every iterator that we write).
>
> The third, and most robust way, is to implement an iterator adaptor similar to
> `std::reverse_iterator`. Then, we can write a single `iterator` implementation and declare our
> `const_iterator` as an alias for `const_iterator<iterator>`. The major advantage of this is that
> the cognitive burden imposed from the second solution is lifted with almost no associated cost for
> any individual iterator: the cost is largely incurred by the iterator adaptor author, who needs to
> design, implement, and test the iterator. Unfortunately, there are no major libraries known to
> directly support this, so you'd need to write the adaptor yourself (or [propose][1] it to a major
> library).

3. Modify your iterator so that it models both `std::bidirectional_iterator` and
   `std::output_iterator<rope::iterator, char>`.

> See `rope_q33.hpp` and `rope_user_q33.cpp`.

## Question 4

Look at `source/silly_set.cpp`

1. Complete this `silly_set` wrapper class (yes, it's a bit redundant).

* Implement the member functions
* Template the container type and set set to be the default container

> See `source/silly_set_q41.cpp` on the solutions branch.

Use template template parameters to avoid the case where we can accidentally define
`silly_set<int, std::vector<float>>`.

> See `source/silly_set_q42.cpp` on the solutions branch.

## Question 5 

For question 3, can this iterator be designed to model `std::random_access_iterator`? Justify your answer. (Note: if it can, you _do not_ need to
   implement it for the tutorial.)

> While this type could permit a random-access iterator, the current interface and internal
> representation don't provide sufficient guarantees to for an iterator to model
> `std::random_access_iterator`.
>
> A good way to summarise a random-access iterator is that it is a bidirectional iterator that can
> move arbitrary distances in _constant_ time, with respect to the total number of elements in the
> range. Since our interface passes an unadultered vector of strings to the internal representation,
> it's entirely possible that a `rope` could be constructed using a vector where each of its
> elements is exactly one character in length. This would mean that the number of elements is
> proportional to the number of strings, and any random-access operations would happen in linear
> time, which breaks the concept's axioms (semantic, plaintext requirements).
>
> There are a few ways in which we could change our `rope` type to make modelling the concept of a
> random-access iterator possible. One way would be for our internal representation to also keep
> track of the size of each string. Then, any calculations that we need to make when defining our
> operations would use this second vector as a lookup table.
>
> Another way is to curate the input data so that it fits in a different internal representation.
> We could maintain a sequence of strings that have a predefined and fixed size; any strings that
> are longer than that will be split up and put into multiple strings. This would involve a lot of
> work in the constructor, but it would simplify the internal representation than the first solution
> (note: a simplified representation doesn't mean that it's faster or smaller). This second solution
> approaches the implementation of `std::deque<char>`, so it'd be better again to use that, given
> it's maintained by industry experts.
