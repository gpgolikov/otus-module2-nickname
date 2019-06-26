#pragma once

#include <catch2/catch.hpp>

#include <sstream>
#include <string>

template<typename T>
class equals : public Catch::MatcherBase<T> {
    T value_;

public:
    explicit equals(T value) : value_(std::move(value)) {}

    virtual bool match(const T& to_match) const override {
        return value_ == to_match;
    }

    virtual std::string describe() const override {
        std::ostringstream os;
        os << "equals \"" << value_ << '"';
        return os.str();
    }
};

template <typename T>
inline auto Equals(T value) {
    return equals<T>(std::move(value));
}

inline auto Equals(const char* str) {
    return equals<std::string_view>(str);
}

namespace std {

ostream&
operator<<(ostream& os, const wstring& value) {
    mbstate_t state;
    const wchar_t* value_data = value.data();
    size_t len = wcsrtombs(nullptr, &value_data, 0, &state);
    string mbvalue(len, 0);
    wcsrtombs(mbvalue.data(), &value_data, mbvalue.size() + 1, &state);

    return os << mbvalue;
}

template<typename T1, typename T2>
ostream&
operator<<(ostream& os, const pair<T1, T2>& p) {
    return os << '{' << get<0>(p) << ';' << get<1>(p) << '}';
}

}
