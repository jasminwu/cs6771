#ifndef COMP6771_ROPE_HPP
#define COMP6771_ROPE_HPP

#include <iterator>
#include <string>
#include <utility>
#include <vector>

class rope {
public:
	class iterator {
		using outer_iterator = std::vector<std::string>::const_iterator;
		using inner_iterator = outer_iterator::value_type::const_iterator;

	public:
		using value_type = inner_iterator::value_type;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;

		iterator() = default;

		explicit iterator(std::vector<std::string> const& pointee,
		                  outer_iterator outer,
		                  inner_iterator inner) noexcept
		: pointee_(&pointee)
		, outer_(outer)
		, inner_(inner) {}

		// Indirect mutability isn't a property of const_iterators
		auto operator*() const -> value_type {
			return *inner_;
		}

		auto operator++() -> iterator& {
			if (inner_ != outer_->end()) {
				++inner_;

				if (inner_ != outer_->end()) {
					return *this;
				}
			}

			++outer_;
			inner_ = outer_ == pointee_->end() ? inner_iterator() : outer_->begin();
			return *this;
		}

		auto operator++(int) -> iterator {
			auto temp = *this;
			++*this;
			return temp;
		}

		friend auto operator==(iterator, iterator) -> bool = default;

	private:
		std::vector<std::string> const* pointee_ = nullptr;
		outer_iterator outer_;
		inner_iterator inner_;
	};

	rope() = default;

	explicit rope(std::vector<std::string> rope)
	: rope_{std::move(rope)} {}

	[[nodiscard]] auto begin() const -> iterator {
		auto inner = rope_.empty() ? decltype(rope_.begin()->begin()){} : rope_.begin()->begin();
		return iterator(rope_, rope_.begin(), inner);
	}

	[[nodiscard]] auto end() const -> iterator {
		return iterator(rope_, rope_.end(), {});
	}

private:
	std::vector<std::string> rope_;
};

#endif // COMP6771_ROPE_HPP
