#include "input_check.h"

namespace inpch {

    content_type content_type_of(const std::string &content_string) {
        if (content_string.empty()) throw std::length_error("content_string");
        bool digit = false, text  = false;
        for (const char  &ch : content_string) {
            digit |= std::isdigit(ch);
            text  |= std::isalpha(ch);
        }
        if (digit && text) return content_type::text_with_numbers;
        if (digit) return content_type::number;
        return content_type::text;
    }

    content_type content_type_of(const std::wstring  &content_string) {
        if (content_string.empty()) throw std::length_error("content_string");
        bool digit = false, text  = false;
        for (const wchar_t  &ch : content_string) {
            digit |= std::isdigit(ch);
            #if defined(__MINGW64__) || defined(__MINGW32__)
            text  |= std::isalpha(ch) ||
                    (rus_alphabet.find(ch) != rus_alphabet.end()) ||
                    (RUS_alphabet.find(ch) != RUS_alphabet.end());
            #else
            text  |= std::isalpha(ch);
            #endif
        }
        if (digit && text) return content_type::text_with_numbers;
        if (digit) return content_type::number;
        return content_type::text;
    }

    bool input_match(const std::string &input_string, const std::string &regex_string) {
        return std::regex_match(input_string, std::regex(regex_string));
    }

    bool input_match(const std::string &input_string, const std::regex &regex) {
        return std::regex_match(input_string, regex);
    }

    int int_input_loop(const int &l, const int &r, const std::string &err, bool in_range) {
        int input;
        while (true) {
            std::cin >> input;
            if (std::cin.fail()) {
                std::cin.clear();
                continue;
            }
            if (!input_check<int>(input, l, r, in_range).is_correct()) {
                std::cerr << err << std::endl;
                continue;
            }
            return input;
        }
    }

    int int_input_loop(const int &l, const int &r, const std::wstring &err, bool in_range) {
        int input;
        while (true) {
            std::wcin >> input;
            if (std::wcin.fail()) {
                std::wcin.clear();
                continue;
            }
            if (!input_check<int>(input, l, r, in_range).is_correct()) {
                std::wcerr << err << std::endl;
                continue;
            }
            return input;
        }
    }

    std::string str_num_input_loop(const base_type &base, const std::string &err, const int &length) {
        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (std::cin.fail()) {
                std::cin.clear();
                continue;
            }
            if (!input_check<std::string>(input, length, base).is_correct()) {
                std::cerr << err << std::endl;
                continue;
            }
            return input;
        }
    }

    std::wstring wstr_num_input_loop(const base_type &base, const std::wstring &err, const int &length) {
        std::wstring input;
        while (true) {
            std::getline(std::wcin, input);
            if (std::cin.fail()) {
                std::cin.clear();
                continue;
            }
            if (!input_check<std::wstring>(input, length, base).is_correct()) {
                std::wcerr << err << std::endl;
                continue;
            }
            return input;
        }
    }

} // inpch