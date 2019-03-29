// a faked version of the Gaudi project's GaudiException

#include <exception>
#include <string>

struct StatusCodeValue {};

struct StatusCode {
    static StatusCodeValue FAILURE;
};

class GaudiException final : public std::exception {
    std::string m_message{"unspecified failure"};

public:
    GaudiException() = default;
    GaudiException(std::string s, std::string, StatusCodeValue)
            : m_message(std::move(s))
    {}
    std::string message() { return m_message; }
};
