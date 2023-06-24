#include <iterator>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <exception>

class permission_error : public std::exception{};

// Defined elsewhere
auto exec_cmd(std::string const& cmd, std::vector<std::string> const& args) -> void;

int main() {
    auto cmd = std::vector<std::string>();
    while (true) {
        std::copy(
            std::istream_iterator<std::string>(std::cin),
            std::istream_iterator<std::string>(),
            std::back_inserter(cmd)
        );

        try {
            exec_cmd(cmd.front(), cmd);
        } catch (permission_error const& e) {
            std::cout << "Cannot exec: " << cmd.front() << std::endl;
        } catch (std::invalid_argument const& e) {
            std::cout << "Invalid argument to: " << cmd.front() << std::endl;
        }
    }
}