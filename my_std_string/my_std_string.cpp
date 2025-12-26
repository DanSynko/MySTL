#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cstring>

namespace my_std {
    class string {
    private:
        char* str;
        char* str_end;
        char* str_end_capacity;
        size_t str_size = 0;
        size_t str_capacity;

        void update_pointers() {
            str_end = &str[str_size];
            str_end_capacity = &str[str_capacity];
        }
        void reallocation() {
            str_capacity *= 1.5;
            char* new_str = new char[str_capacity];
            std::move(str, str_end, new_str);
            delete[] str;
            str = new_str;
            update_pointers();
        }
        friend void swap(string& first, string& second) {
            using std::swap;
            swap(first.str, second.str);
            swap(first.str_end, second.str_end);
            swap(first.str_end_capacity, second.str_end_capacity);
            swap(first.str_size, second.str_size);
            swap(first.str_capacity, second.str_capacity);
        }
    public:
        string() {
            str_capacity = 32;
            str = new char[str_capacity];
            str_end = &str[str_size];
            str_end_capacity = &str[str_capacity];
            *str_end = '\0';
        }
        string(const char* other_str) {
            for (const char* ptr = other_str; *ptr != '\0'; ptr++) {
                str_size++;
            }
            str_capacity = str_size * 1.5;
            str = new char[str_capacity];
            /*for (const char* ptr = other_str; *ptr != '\0'; ptr++) {
                str_size++;
            }*/
            std::copy(other_str, &other_str[str_capacity], str);
            update_pointers();
            show_string();
        }
        string(const string& other) {
            this->str_size = other.str_size;
            this->str_capacity = other.str_capacity;
            this->str = new char[this->str_capacity];
            this->str_end = &this->str[this->str_size];
            const char* temp = other.str;
            for (char* ptr = this->str; ptr != this->str_end; ptr++) {
                *ptr = *temp;
                temp++;
            }
            *this->str_end = '\0';
            this->str_end_capacity = &this->str[this->str_capacity];
            this->show_string();
        }
        string(string&& other) noexcept {
            this->str = other.str;
            this->str_size = other.str_size;
            this->str_capacity = other.str_capacity;
            this->str_end = other.str_end;
            this->str_end_capacity = other.str_end_capacity;
            other.str_size = 0;
            other.str_capacity = 0;
            other.str = nullptr;
            other.str_end = nullptr;
            other.str_end_capacity = nullptr;
            this->show_string();
        }
        ~string() {
            delete[] str;
            str = nullptr;
            str_end = nullptr;
            str_end_capacity = nullptr;
        }
        string& operator=(string other) noexcept {
            using std::swap;
            swap(*this, other);
            this->show_string();
            return *this;
        }
        string& operator=(const char* other) {
            size_t other_size = 0;
            for (const char* ptr = other; *ptr != '\0'; ptr++) {
                other_size++;
            }
            other_size++;
            if (other_size >= str_capacity) {
                reallocation();
                str_size = other_size;
            }
            str_size = other_size;
            std::move(other, &other[other_size], str);
            update_pointers();
            this->show_string();
            return *this;
        }

        static void ruleoffive_demo() {
            my_std::string str;
            str = "Hello World!";
            str.show_string();
            my_std::string str_copy_c(str);
            my_std::string str_move_c = str;
            my_std::string str_copy_op = str_copy_c;
            my_std::string str_move_op;
            str_move_op = str_move_c;
        }

        char& operator[](size_t i){
            if (i >= str_size) {
                throw std::out_of_range("string out_of_range");
            }
            else {
                return str[i];
            }
        }
        string& operator+=(const string& other) {
            size_t new_str_size = other.str_size + this->str_size + 1;
            char* new_str = new char[new_str_size];
            char* new_str_start = new_str;
            const char* ptr;
            for (ptr = str; *ptr != *str_end; ptr++) {
                *new_str = *ptr;
                new_str++;
            }
            for (ptr = other.str; *ptr != *other.str_end; ptr++) {
                *new_str = *ptr;
                new_str++;
            }
            *new_str = '\0';
            delete[] str;
            //delete[] other.str;
            str = new_str_start;
            str_end = new_str;
            return *this;
        }
        string& operator+=(const char* other) {
            size_t new_str_size = 0;
            for (const char* ptr = other; *ptr != '\0'; ptr++) {
                new_str_size++;
            }
            new_str_size += str_size + 1;
            char* new_str = new char[new_str_size];
            char* new_str_start = new_str;
            const char* ptr;
            for (ptr = str; *ptr != *str_end; ptr++) {
                *new_str = *ptr;
                new_str++;
            }
            for (ptr = other; *ptr != '\0'; ptr++) {
                *new_str = *ptr;
                new_str++;
            }
            *new_str = '\0';
            delete[] str;
            str = new_str_start;
            return *this;
        }
        string operator+(const string& second) {
            string temp = *this;
            temp += second.str;
            return temp;
        }
        string operator+(const char* other) {
            string temp = *this;
            temp += other;
            return temp;
        }
        friend std::ostream& operator<<(std::ostream& os, const string& other) {
            os << other.str;
            return os;
        }
        friend std::istream& operator>>(std::istream& is, string& other) {
            delete[] other.str;
            other.str = nullptr;
            other.str_size = 0;
            const size_t temp_capacity = 256;
            char* buffer = new char[temp_capacity];
            is.getline(buffer, temp_capacity);
            if (is) {
                other.str_size = std::strlen(buffer);
                other.str_capacity = other.str_size + 1;
                other.str = new char[other.str_capacity];
                other.update_pointers();
                std::copy(buffer, &buffer[other.str_capacity], other.str);
            }
            delete[] buffer;
            return is;
        }
        friend bool operator<(const string& first, const string& second) {
            const char* ptr_first = first.str;
            const char* ptr_second = second.str;
            while (*ptr_first != '\0' && *ptr_second != '\0') {
                if (*ptr_first < *ptr_second) {
                    return true;
                }
                else if (*ptr_first > *ptr_second) {
                    return false;
                }
                ptr_first++;
                ptr_second++;
            }
            return *ptr_first < *ptr_second;
        }
        friend bool operator>(const string& first, const string& second) {
            const char* ptr_first = first.str;
            const char* ptr_second = second.str;
            while (*ptr_first != '\0' && *ptr_second != '\0') {
                if (*ptr_first > *ptr_second) {
                    return true;
                }
                else if (*ptr_first < *ptr_second) {
                    return false;
                }
                ptr_first++;
                ptr_second++;
            }
            return *ptr_first > *ptr_second;
        }
        friend bool operator<=(const string& first, const string& second) {
            const char* ptr_first = first.str;
            const char* ptr_second = second.str;
            while (*ptr_first != '\0' && *ptr_second != '\0') {
                if (*ptr_first > *ptr_second) {
                    return false;
                }
                else if (*ptr_first < *ptr_second) {
                    return true;
                }
                ptr_first++;
                ptr_second++;
            }
            return *ptr_first <= *ptr_second;
        }
        friend bool operator>=(const string& first, const string& second) {
            const char* ptr_first = first.str;
            const char* ptr_second = second.str;
            while (*ptr_first != '\0' && *ptr_second != '\0') {
                if (*ptr_first < *ptr_second) {
                    return false;
                }
                ptr_first++;
                ptr_second++;
            }
            return *ptr_first >= *ptr_second;
        }
        friend bool operator==(const string& first, const string& second) {
            const char* ptr_first = first.str;
            const char* ptr_second = second.str;
            while (*ptr_first != '\0' && *ptr_second != '\0') {
                if (*ptr_first != *ptr_second) {
                    return false;
                }
                ptr_first++;
                ptr_second++;
            }
            return *ptr_first == *ptr_second;
        }
        friend bool operator!=(const string& first, const string& second) {
            const char* ptr_first = first.str;
            const char* ptr_second = second.str;
            while (*ptr_first != '\0' && *ptr_second != '\0') {
                if (*ptr_first != *ptr_second) {
                    return true;
                }
                ptr_first++;
                ptr_second++;
            }
            return *ptr_first != *ptr_second;
        }
        void show_string() const {
            std::cout << str << std::endl;
        }
        
        char* c_str() const {
            return str;
        }
    };
}

int main()
{
    my_std::string::ruleoffive_demo();
    my_std::string str = "Lorem ipsum";
    my_std::string str2 = "Lorem ipsum";
    my_std::string str3 = "dolor sit amet";
    if (str == str2) {
        std::cout << "Two strings is equal." << std::endl;
    }
    else {
        std::cout << "Two strings is NOT equal." << std::endl;
    }
    if (str != str3) {
        std::cout << "Two strings is NOT equal." << std::endl;
    }
    else {
        std::cout << "Two strings is equal." << std::endl;
    }
    if (str < str3) {
        std::cout << "First string is bigger." << std::endl;
    }
    else if (str > str3) {
        std::cout << "Second string is bigger." << std::endl;
    }
    str += str3;
    str.show_string();
    std::cout << "Enter some text in string:" << std::endl;
    std::cin >> str;
    str.show_string();
    return 0;
}