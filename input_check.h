#ifndef INPUTCHECK_INPUT_CHECK_H
#define INPUTCHECK_INPUT_CHECK_H

#if defined(__MINGW32__) || defined(__MINGW64__)

#define CYRILLIC_CHAR_TYPE wchar_t
#define PFX(l) (L'l')

#elif defined(_MSC_VER)

#define CYRILLIC_CHAR_TYPE char
#define PFX(l) (l)

#endif

#if defined(__cplusplus) && __cplusplus >= 201703L

#include <iostream>
#include <set>
#include <algorithm>
#include <exception>

namespace inpch {

    inline const int any_length = -1;

    inline const std::set<char> eng_alphabet{
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    inline const std::set<CYRILLIC_CHAR_TYPE> rus_alphabet{
            PFX(а), PFX(б), PFX(в), PFX(г), PFX(д), PFX(е), PFX(ё), PFX(ж),
            PFX(з), PFX(и), PFX(й), PFX(к), PFX(л), PFX(м), PFX(н), PFX(о),
            PFX(п), PFX(р), PFX(с), PFX(т), PFX(у), PFX(ф), PFX(х), PFX(ц),
            PFX(ч), PFX(ш), PFX(щ), PFX(ъ), PFX(ы), PFX(ь), PFX(э), PFX(ю), PFX(я)
    };

    inline const std::set<char> ENG_alphabet{
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
    };

    inline const std::set<CYRILLIC_CHAR_TYPE> RUS_alphabet{
            PFX(А), PFX(Б), PFX(В), PFX(Г), PFX(Д), PFX(Е), PFX(Ё), PFX(Ж),
            PFX(З), PFX(И), PFX(Й), PFX(К), PFX(Л), PFX(М), PFX(Н), PFX(О),
            PFX(П), PFX(Р), PFX(С), PFX(Т), PFX(У), PFX(Ф), PFX(Х), PFX(Ц),
            PFX(Ч), PFX(Ш), PFX(Щ), PFX(Ъ), PFX(Ы), PFX(Ь), PFX(Э), PFX(Ю), PFX(Я)
    };

    enum base_type {
        decimal = 10,
        octal = 8,
        hexadecimal = 16,
        binary = 2,
        not_a_number = 0
    };

    enum language {
        rus,
        eng,
        RUS,
        ENG,
        Rus,
        Eng,
        RuS,
        EnG
    };

    enum content_type {
        number,
        text,
        text_with_numbers
    };

    class WrongBaseException : public std::exception {
    private:
        std::string message;
    public:
        WrongBaseException() { message = "Wrong base type"; }

        const char *what() const noexcept override {
            return message.c_str();
        }
    };

    class WrongTypeException : public std::exception {
    private:
        std::string message;
    public:
        WrongTypeException() { message = "Wrong type of given value"; }

        const char *what() const noexcept override {
            return message.c_str();
        }
    };

    class WrongLanguageException : public std::exception {
    private:
        std::string message;
    public:
        WrongLanguageException() { message = "Wrong given language"; }

        const char *what() const noexcept override {
            return message.c_str();
        }
    };

    template<class T>
    bool is_hexadecimal(const T &str) noexcept;

    template<class T>
    bool is_binary(const T &str) noexcept;

    template<class T>
    bool is_decimal(const T &str) noexcept;

    template<class T>
    bool is_octal(const T &str) noexcept;

    content_type content_type_of(const std::string &content_string);
    content_type content_type_of(const std::wstring  &content_string);

    template<class T>
    class input_check {
    public:
        input_check(const T &input_number, const T &, const T &, bool inside = true);

        input_check(const T &input_string, const int &length = any_length, const base_type &base = not_a_number);

        input_check(const T &input_string, const language &lang);

        bool is_correct() { return correct; }

        T get_value() { return value; }

    private:
        bool correct;
        T value;

        bool eng_lang_check(const T &str, const std::set<char> &alpha);

        bool rus_lang_check(const T &str, const std::set<CYRILLIC_CHAR_TYPE> &alpha);

        bool eng_lang_check(const T &str, const std::set<char> &alpha_upper, const std::set<char> &alpha_lower);

        bool rus_lang_check(const T &str, const std::set<CYRILLIC_CHAR_TYPE> &alpha_upper,
                            const std::set<CYRILLIC_CHAR_TYPE> &alpha_lower);
    };

    /* -------------------------------- Declarations -------------------------------- */

    template<class T>
    input_check<T>::input_check(const T &input_string, const language &lang) {
        if
                (
                std::is_same<T, std::string>::value ||
                std::is_same<T, std::wstring>::value
                ) {
            if (input_string.empty()) throw std::length_error("input_string");

            switch (lang) {
                case rus:
                    if (rus_lang_check(input_string, rus_alphabet)) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case eng:
                    if (eng_lang_check(input_string, eng_alphabet)) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case RUS:
                    if (rus_lang_check(input_string, RUS_alphabet)) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case ENG:
                    if (eng_lang_check(input_string, ENG_alphabet)) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case Rus:
                    if (rus_lang_check(input_string.substr(1), rus_alphabet) &&
                        (RUS_alphabet.find(input_string[0]) != RUS_alphabet.end())) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case Eng:
                    if (eng_lang_check(input_string.substr(1), eng_alphabet) &&
                        (ENG_alphabet.find(input_string[0]) != ENG_alphabet.end())) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case RuS:
                    if (rus_lang_check(input_string, RUS_alphabet, rus_alphabet)) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                case EnG:
                    if (eng_lang_check(input_string, eng_alphabet, ENG_alphabet)) {
                        value = input_string;
                        correct = true;
                        return;
                    }
                    correct = false;
                    return;
                default:
                    throw WrongLanguageException();
                    return;
            }
        }
        correct = false;
        throw WrongTypeException();
    }

    template<class T>
    input_check<T>::input_check(const T &input, const int &length, const base_type &base) {
        if
                (
                std::is_same<T, std::string>::value ||
                std::is_same<T, std::wstring>::value
                ) {
            if (input.empty()) throw std::length_error("input");
            if (length != any_length) {
                if (input.size() != length) {
                    correct = false;
                    return;
                }
            }
            switch (base) {
                case not_a_number:
                    value = input;
                    correct = true;
                    return;
                case decimal:
                    correct = is_decimal(input);
                    if (!correct) {
                        return;
                    }
                    value = input;
                    return;
                case octal:
                    correct = is_octal(input);
                    if (!correct) {
                        return;
                    }
                    value = input;
                    return;
                case hexadecimal:
                    correct = is_hexadecimal(input);
                    if (!correct) {
                        return;
                    }
                    value = input;
                    return;
                case binary:
                    correct = is_binary(input);
                default:
                    throw WrongBaseException();
                    break;
            }
        }
        correct = false;
        throw WrongTypeException();
    }

    template<class T>
    input_check<T>::input_check(const T &input, const T &left, const T &right, bool inside) {
        if
                (
                std::is_same<T, int>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, long long>::value
                ) {
            if (inside) {
                if (input >= left && input <= right) {
                    correct = true;
                    value = input;
                    return;
                }
            }
            if (input < left || input > right) {
                correct = true;
                value = input;
                return;
            }
        }
        correct = false;
        throw WrongTypeException();
    }

    template<class T>
    bool is_hexadecimal(const T &str) noexcept {
        if
                (
                !(std::is_same<T, std::string>::value ||
                  std::is_same<T, std::wstring>::value)
                ) {
            return false;
        }

        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];
            if (!isxdigit(c)) {
                return false;
            }
        }

        return true;
    }

    template<class T>
    bool is_binary(const T &str) noexcept {
        if
                (
                !(std::is_same<T, std::string>::value ||
                  std::is_same<T, std::wstring>::value)
                ) {
            return false;
        }

        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];
            // Проверка, чтобы каждый символ был либо '0', либо '1'
            if (c != '0' && c != '1') {
                return false;
            }
        }

        return true;
    }

    template<class T>
    bool is_decimal(const T &str) noexcept {
        if
                (
                !(std::is_same<T, std::string>::value ||
                  std::is_same<T, std::wstring>::value)
                ) {
            return false;
        }
        if (str.empty()) {
            return false; // Пустая строка не является десятичным числом
        }

        size_t start = 0;
        if (str[0] == '+' || str[0] == '-') { // проверка на наличие знака в начале строки
            if (str.length() == 1) {
                return false; // строка состоит только из знака
            }
            start = 1; // начинаем проверку с следующего символа после знака
        }

        for (size_t i = start; i < str.size(); ++i) {
            if (!isdigit(str[i])) {
                return false; // если текущий символ не является цифрой, то это не десятичное число
            }
        }

        return true; // все символы строки подходят под критерии десятичного числа
    }

    template<class T>
    bool is_octal(const T &str) noexcept {
        if
                (
                !(std::is_same<T, std::string>::value ||
                  std::is_same<T, std::wstring>::value)
                ) {
            return false;
        }
        if (str.empty()) {
            return false; // Пустая строка не является восьмеричным числом
        }

        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];
            if (c < '0' || c > '7') {
                return false; // если символ не в диапазоне от '0' до '7'
            }
        }

        return true; // все символы строки подходят под критерии восьмеричного числа
    }

    template<class T>
    bool input_check<T>::eng_lang_check(const T &str, const std::set<char> &alpha) {
        if
                (
                std::all_of(str.begin(), str.end(),
                            [&alpha](char ch) {
                                return alpha.find(ch) != alpha.end();
                            })
                ) {
            return true;
        } else {
            return false;
        }
    }

    template<class T>
    bool input_check<T>::rus_lang_check(const T &str, const std::set<CYRILLIC_CHAR_TYPE> &alpha) {
        if
                (
                std::all_of(str.begin(), str.end(),
                            [&alpha](CYRILLIC_CHAR_TYPE ch) {
                                return alpha.find(ch) != alpha.end();
                            })
                ) {
            return true;
        } else {
            return false;
        }
    }

    template<class T>
    bool
    input_check<T>::eng_lang_check(const T &str, const std::set<char> &alpha_upper, const std::set<char> &alpha_lower) {
        for (const auto &ch: str) {
            if (alpha_upper.find(ch) == alpha_upper.end() || alpha_lower.find(ch) == alpha_lower.end()) {
                return false;
            }
        }
        return true;
    }

    template<class T>
    bool input_check<T>::rus_lang_check(const T &str, const std::set<CYRILLIC_CHAR_TYPE> &alpha_upper,
                                        const std::set<CYRILLIC_CHAR_TYPE> &alpha_lower) {
        for (const auto &ch: str) {
            if (alpha_upper.find(ch) == alpha_upper.end() || alpha_lower.find(ch) == alpha_lower.end()) {
                return false;
            }
        }
        return true;
    }

} // inpch

#else

#define ERROR "using C or pre C++17"

#endif // CHECK C++ VERSION

#endif //INPUTCHECK_INPUT_CHECK_H
