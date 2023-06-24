#include <array>
#include <cassert>
#include <vector>
#include <iostream>

enum class number_type {
  prime,
  composite,
};

// we use a template and a `std::array` so that we don't need to use `std::vector`
template <int largest_value>
constexpr auto generate_primes_lookup_table()
  -> std::array<number_type, largest_value>
{
  // all values initialized to "prime"
  auto result = std::array<number_type, largest_value>{};

  // 0 and 1 don't count, so we start at 2
  for (auto x = std::size_t{2}; x < result.size() / 2; ++x) {
    if (result[x] == number_type::prime) {
      for (auto y = x + x; y < result.size(); y += x) {
        result[y] = number_type::composite;
      }
    }
  }

  return result;
}

// we can make this function run at compile time too, now!
constexpr auto is_prime(int value) -> bool {
  // negative numbers, 0, and 1 are not prime
  if (value < 2) {
    return false;
  }

  // have a lookup table for fast checking of small primes
  constexpr auto lookup_size = 10'000;
  constexpr auto lookup_table = generate_primes_lookup_table<lookup_size>();
  if (value < lookup_size) {
    return lookup_table[static_cast<std::size_t>(value)] == number_type::prime;
  }

  // larger values, we use trial by division
  // we can still use our lookup table to only check with relevant factors
  // (division is expensive!)
  for (auto i = 2; i < lookup_size and i < (value / 2); ++i) {
    if (lookup_table[static_cast<std::size_t>(i)] == number_type::prime) {
      if (value % i == 0) {
        return false;
      }
    }
  }

  // outside the range of our lookup table, let's just go for broke
  for (auto i = lookup_size; i < (value / 2); ++i) {
    if (value % i == 0) {
      return false;
    }
  }

  // we have found no factors, therefore the number is prime
  return true;
}

auto main() -> int {
  std::cout << "Enter a number: ";
  auto i = 0;
  std::cin >> i;

  // happens at runtime
  if (is_prime(i)) {
    std::cout << "The number is prime!\n";
  } else {
    std::cout << "The number is not prime.\n";
  }

  // we can also do this entirely at compile time :)
  constexpr auto value = 104729;
  constexpr auto result = is_prime(value);
  std::cout << "Also, " << value << " is ";
  if (not result) {
    std::cout << "not ";
  }
  std::cout << "prime.\n";
}
