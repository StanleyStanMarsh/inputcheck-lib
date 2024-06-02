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

} // inpch