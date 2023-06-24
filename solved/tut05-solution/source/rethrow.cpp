#include <exception>
#include <string>
#include <iostream>

class Logger {
public:
    auto log(std::string const& msg) -> void;
};

class DBConn {
public:
    auto try_connect(std::string const& uname, std::string const& pword) -> void;

private:
    auto active_ = bool{false};
};

auto make_connection(std::string const& uname, std::string const& pword) -> DBConn {
    auto db = DBConn{};
    try {
        db.try_connect(uname, pword);
        return db;
    } catch (std::string const& e) {
        std::cout << e << std::endl;
        throw e;
    }
}

int main() {
    auto l = Logger{};

    try {
        make_connection("hsmith", "swagger/10");
    } catch (std::string const& e) {
        l.log("Could not establish connection");
    }
}