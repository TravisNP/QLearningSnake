#include<iostream>

/** This class was created in help with chat-gpt and the various sources on the internet
 * I needed a way to use both string and string_view types in my reflection map as
 * I didn't want to create a new string every time I wanted to look at a substring
 */

// Custom hash function for std::string and std::string_view
struct StringViewHash {
    using is_transparent = void; // Enable heterogeneous lookup

    std::size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }

    std::size_t operator()(const std::string& s) const noexcept {
        return std::hash<std::string>{}(s);
    }
};

// Custom equality function for std::string and std::string_view
struct StringViewEqual {
    using is_transparent = void; // Enable heterogeneous lookup

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }
};