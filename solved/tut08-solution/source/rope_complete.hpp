#ifndef COMP6771_ROPE_HPP
#define COMP6771_ROPE_HPP

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <version>

class rope {
	// forward-declare our iterator implementation
	// templated so we can get both `const` and non-`const` implementations
	// without duplicating lots of code
	//
	// (another alternative is to use a type as `char` or `const char`,
	// and use `decltype` and friends to determine what to do.)
	template <bool is_const>
	class iterator_impl;

public:
	// iterator types
	using iterator = iterator_impl<false>;
	using const_iterator = iterator_impl<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	explicit rope(std::vector<std::string> rope)
		: rope_{std::move(rope)}
	{}

	// non-const begin/end
	[[nodiscard]] auto begin() -> iterator {
		return { rope_.begin(), rope_.end() };
	}
	[[nodiscard]] auto end() -> iterator {
		return { rope_.end(), rope_.end() };
	}

	// const begin/end; get cbegin/cend for free
	[[nodiscard]] auto begin() const -> const_iterator {
		return { rope_.begin(), rope_.end() };
	}
	[[nodiscard]] auto end() const -> const_iterator {
		return { rope_.end(), rope_.end() };
	}
	[[nodiscard]] auto cbegin() const -> const_iterator {
		return begin();
	}
	[[nodiscard]] auto cend() const -> const_iterator {
		return end();
	}

	// reverse iterators all come for free as well
	[[nodiscard]] auto rbegin() -> reverse_iterator {
		return reverse_iterator{ end() };
	}
	[[nodiscard]] auto rend() -> reverse_iterator {
		return reverse_iterator{ begin() };
	}
	[[nodiscard]] auto rbegin() const -> const_reverse_iterator {
		return const_reverse_iterator{ end() };
	}
	[[nodiscard]] auto rend() const -> const_reverse_iterator {
		return const_reverse_iterator{ begin() };
	}
	[[nodiscard]] auto crbegin() const -> const_reverse_iterator {
		return rbegin();
	}
	[[nodiscard]] auto crend() const -> const_reverse_iterator {
		return rend();
	}

private:
	std::vector<std::string> rope_;

	template <bool is_const>
	class iterator_impl {
	public:
		// conditional_t is like the "if/else" of type-level stuff
		using value_type = char;
		using reference = std::conditional_t<is_const, const char&, char&>;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// default constructor
		iterator_impl() = default;

		// copy/move constructor/assignment
		iterator_impl(const iterator_impl&) = default;
		iterator_impl(iterator_impl&&) noexcept = default;
		iterator_impl& operator=(const iterator_impl&) = default;
		iterator_impl& operator=(iterator_impl&&) noexcept = default;

		// allow `const_iterator x = iterator{};` to work
		// (this is implemented C++20 style, don't worry too much about it)
		iterator_impl(const iterator_impl<false>& other)
			requires is_const
			: last_(other.last_)
			, outer_(other.outer_)
			, inner_(other.inner_)
		{}

		auto operator++() -> iterator_impl& {
			++inner_;  // go to the next char
			// if hit the end of this string, and aren't at the end of the rope
			while (outer_ != last_ and inner_ == outer_->end()) {
				// go to the next string
				++outer_;
				if (outer_ != last_) {
					// and look at the start of it
					inner_ = outer_->begin();
				}
			}
			return *this;
		}

		auto operator--() -> iterator_impl& {
			// gotta do it the other way around, now
			// if we're at the end, step back until we find a non-empty string
			// (don't need to handle if they don't exist,
			//  since if so `--r.end()` doesn't make sense anyway)
			while (outer_ == last_ or inner_ == outer_->begin()) {
				--outer_;
				inner_ = outer_->end();
			}
			--inner_;
			return *this;
		}

		// postfix is always a simple formula
		auto operator++(int) -> iterator_impl {
			auto old = *this;
			++*this;
			return old;
		}

		auto operator--(int) -> iterator_impl {
			auto old = *this;
			--*this;
			return old;
		}

		auto operator*() const -> reference {
			return *inner_;
		}

		friend auto operator==(const iterator_impl& lhs, const iterator_impl& rhs) -> bool {
			// make sure the "outer" is the same; if so,
			return lhs.outer_ == rhs.outer_ and
			// either we're at the end of the rope, or we're pointing at the same char
				(lhs.outer_ == lhs.last_ or lhs.inner_ == rhs.inner_);
		}

	private:
		using outer_t = std::conditional_t<
			is_const,
			std::vector<std::string>::const_iterator,
			std::vector<std::string>::iterator>;

		// could do the same thing here, but let's use decltype to get the type
		// remember this is never actually executed
		// we could do similar tricks everywhere else, as well; no actual need to use conditional_t
		// (but it's easier to remember what to do)
		using inner_t = decltype(outer_t{}->begin());

		iterator_impl(outer_t first, outer_t last)
			: last_(last)
			, outer_(std::find_if_not(first, last, [](const auto& s) { return s.empty(); }))
			, inner_(outer_ == last_ ? inner_t{} : outer_->begin())
		{}

		outer_t last_ {};		// the end of the rope
		outer_t outer_ {};	// which string we're looking at
		inner_t inner_ {};	// which character we're looking at in the string

		friend class rope;
	};
};

#endif // COMP6771_ROPE_HPP
