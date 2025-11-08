#include <iostream>
#include <string>
#include <utility>
//#include <stdexcept>

namespace my_std {
    template<typename T>
    class MyVector {
    private:
        T* arr;
        T* begin_it;
        T* end_it;
        T* end_capacity;
        size_t arr_size;
        size_t arr_capacity;
    public:
        MyVector() : arr_size(10), arr_capacity(40) {
            arr = new T[arr_capacity];
            create_some_vec_int();
            //create_some_vec_char();
            //create_some_vec_bool();
            //create_some_vec_obj();
            //create_some_vec_str();
            begin_it = arr;
            end_it = &arr[arr_size];
            end_capacity = &arr[arr_capacity];
        }
        MyVector(const MyVector& other) {
            std::cout << "The deep copy constructor was called: " << std::endl;
            this->arr = new T[other.arr_capacity];
            this->arr_size = other.arr_size;
            this->end_it = &this->arr[this->arr_size];
            this->arr_capacity = other.arr_capacity;
            this->begin_it = this->arr;
            T* i = this->begin_it;
            for (T* ptr = other.begin_it; ptr != other.end_it; ptr++) {
                *i = *ptr;
                i++;
            }
            T* new_end_capacity = &this->arr[this->arr_capacity];
            this->end_capacity = new_end_capacity;
            this->show_vector();
        }
        MyVector(MyVector&& other) noexcept {
            std::cout << "The move constructor was called: " << std::endl;
            this->arr_size = other.arr_size;
            this->arr_capacity = other.arr_capacity;
            this->arr = other.arr;
            this->begin_it = other.begin_it;
            this->end_it = other.end_it;
            this->end_capacity = other.end_capacity;
            other.arr_size = 0;
            other.arr_capacity = 0;
            other.arr = nullptr;
            other.begin_it = nullptr;
            other.end_it = nullptr;
            other.end_capacity = nullptr;
            this->show_vector();
        }
        MyVector& operator=(const MyVector& other) {
            std::cout << "The deep copy operator= was called: " << std::endl;
            delete[] this->arr;
            this->arr = new T[other.arr_capacity];
            for (T* ptr = other.begin_it, *i = this->arr; ptr != other.end_it; ptr++, i++) {
                *i = *ptr;
            }
            this->arr_size = other.arr_size;
            this->arr_capacity = other.arr_capacity;
            this->begin_it = this->arr;
            this->end_it = &this->arr[arr_size];
            this->end_capacity = &this->arr[arr_capacity];
            this->show_vector();
            return *this;
        }
        MyVector& operator=(MyVector&& other) noexcept {
            std::cout << "The move operator= was called: " << std::endl;
            this->arr_size = other.arr_size;
            this->arr_capacity = other.arr_capacity;
            this->arr = other.arr;
            this->begin_it = other.begin_it;
            this->end_it = other.end_it;
            this->end_capacity = other.end_capacity;
            other.arr_size = 0;
            other.arr_capacity = 0;
            other.arr = nullptr;
            other.begin_it = nullptr;
            other.end_it = nullptr;
            other.end_capacity = nullptr;
            this->show_vector();
            return *this;
        }
        ~MyVector() {
            std::cout << "The destructor was called. " << std::endl;
            delete[] arr;
            begin_it = nullptr;
            end_it = nullptr;
            end_capacity = nullptr;
        }
        void show_vector() {
            for (T* ptr = begin_it; ptr != end_it; ptr++) {
                std::cout << *ptr << ". ";
            }
            std::cout << "" << std::endl;
        }
        void create_some_vec_int() {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = i+3;
            }
            show_vector();
        }
        void create_some_vec_char() {
            int char_code_finish = 256;
            for (int char_code_start = 0; char_code_start < char_code_finish; char_code_start++) {
                arr[char_code_start] = static_cast<char>(char_code_start);
                if (char_code_start == arr_size) {
                    break;
                }
            }
            show_vector();
        }
        void create_some_vec_bool() {
            std::cout << std::boolalpha;
            for (int i = 0; i < arr_size; i++) {
                if (i % 2 == 0) {
                    arr[i] = true;
                }
                else {
                    arr[i] = false;
                }
            }
            show_vector();
        }
        /*void create_some_vec_obj() {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = i + 3;
            }
            show_vector();
        }*/
        /*void create_some_vec_str() {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = i + 3;
            }
            show_vector();
        }*/
        void reallocation() {
            if (arr_capacity <= arr_size) {
                arr_capacity *= 3 / 2;
                T* new_arr = new T[arr_capacity];
                for (T* ptr = begin_it, *i = new_arr; ptr != end_it; ptr++, i++) {
                    *i = *ptr;
                }
                delete[] arr;
                arr = new_arr;
                begin_it = arr;
                end_it = &arr[arr_size];
                end_capacity = &arr[arr_capacity];
            }
        }
        // The method's complexity should be O(n)(linear time)
        void resize(int new_size) {
            reallocation();
            std::cout << "old size: ";
            size();
            size_t old_size = arr_size;
            arr_size = new_size;
            if (arr_size > old_size) {
                for (size_t i = old_size; i < arr_size; i++) {
                    arr[i] = 1;
                }
            }
            else if (arr_size < old_size) {
                for (size_t i = arr_size; i < old_size; i++) {
                    end_it--;
                }
            }
            begin_it = arr;
            end_it = &arr[arr_size];
            end_capacity = &arr[arr_capacity];
            std::cout << "new size: ";
            size();
            show_vector(); 
        }
        // The method's complexity should be O(1)(constant time)
        void push_back(T a) {
            reallocation();
            *end_it = a;
            end_it++;
            arr_size++;
            show_vector();
        }
        // O(1)
        void pop_back() {
            end_it--;
            arr_size--;
        }
        bool empty() {
            return !arr_size;
        }
        T& front() {
            T& front_ref = *begin_it;
            return front_ref;
        }
        T& back() {
            T* back_it = end_it - 1;
            T& back_ref = *back_it;
            return back_ref;
        }
        T* data() {
            return arr;
        }
        T* begin() {
            return begin_it;
        }
        T* end() {
            return end_it;
        }
        const T* сbegin() const {
            return begin_it;
        }
        const T* сend() const {
            return end_it;
        }
        void size() {
            std::cout << "vector size: " << arr_size << "." << std::endl;
        }
        void reserve(const int reserved_ram) {
            arr_capacity = reserved_ram;
            capacity();
        }
        void capacity() {
            std::cout << "vector capacity size: " << arr_capacity << "." << std::endl;
        }
        T& at(int i) {
            if (i < 0 || i >= arr_size) {
                throw std::out_of_range("out_of_range");
            }
            else {
                return arr[i];
            }
        }
        // O(1)
        void clear() {
            arr_size = 0;
            end_it = &arr[arr_size];
        }
        void shrink_to_fit() {
            arr_capacity = arr_size;
            T* new_arr = new T[arr_capacity];
            for (T* ptr = begin_it, *i = new_arr; ptr != end_it; ptr++, i++) {
                *i = *ptr;
            }
            delete[] arr;
            arr = new_arr;
            begin_it = arr;
            end_it = &new_arr[arr_size];
            end_capacity = &new_arr[arr_capacity];
            show_vector();
        }
    };
}
class Test {
    int num;
    char symb;
    bool tof;
public:
    //Test(){}
};
int main()
{
    my_std::MyVector<int> my_vector;
    //my_std::MyVector<char> my_vector;
    //my_std::MyVector<bool> my_vector;
    Test obj;
    //my_std::MyVector<Test> my_vector;
    std::cout << "Welcome to MyStdVector! There is a vector<int>: ";
    //my_vector.create_some_vec_int();
    //my_vector.create_some_vec_char();
    my_vector.create_some_vec_bool();
    std::cout << "" << std::endl;
    std::cout << "my_std::size()" << std::endl;
    my_vector.size();
    std::cout << "" << std::endl;
    std::cout << "my_std::capacity()" << std::endl;
    my_vector.capacity();
    std::cout << "" << std::endl;
    std::cout << "my_std::resize()" << std::endl;
    my_vector.resize(22);
    std::cout << "" << std::endl;
    std::cout << "my_std::push_back()" << std::endl;
    my_vector.push_back(0.452);
    std::cout << "" << std::endl;
    std::cout << "my_std::pop_back()" << std::endl;
    my_vector.pop_back();
    my_vector.show_vector();
    std::cout << "" << std::endl;
    std::cout << "my_std::reserve()" << std::endl;
    my_vector.reserve(50);
    std::cout << "" << std::endl;
    std::cout << "my_std::front()" << std::endl;
    /*int& front_element = my_vector.front();
    char& front_element = my_vector.front();*/
    int& front_element = my_vector.front();
    //Test& front_element = my_vector.front();
    std::cout << front_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::back()" << std::endl;
    /*int& back_element = my_vector.front();
    char& back_element = my_vector.front();*/
    int& back_element = my_vector.front();
    //Test& back_element = my_vector.front();
    std::cout << back_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::data()" << std::endl;
    /*int& data_element = my_vector.front();
    char& data_element = my_vector.front();*/
    int& data_element = my_vector.front();
    //Test& data_element = my_vector.front();
    std::cout << data_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::at() (correct work)" << std::endl;
    my_vector.at(4);
    std::cout << "" << std::endl;
    try {
        std::cout << "my_std::at() (if index is invalid)" << std::endl;
        my_vector.at(-1);
        std::cout << "" << std::endl;
    }
    catch (std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    std::cout << "my_std::shrink_to_fit()" << std::endl;
    my_vector.shrink_to_fit();
    std::cout << "\n" << std::endl;


    std::cout << "rule of five: " << std::endl;
    // deep copy constructor
    my_std::MyVector<int> my_vector_dp(my_vector);
    // move constructor
    my_std::MyVector<int> my_vector_rval;
    my_std::MyVector<int> my_vector_m = std::move(my_vector_rval);
    // deep copy operator=
    my_std::MyVector<int> my_vector_dp_op;
    my_vector_dp_op = my_vector;
    // move operator=
    my_std::MyVector<int> my_vector_rval2;
    my_vector_dp = my_vector_rval2;


    std::cout << "my_std::clear()" << std::endl;
    my_vector.clear();
    my_vector.size();
    std::cout << "my_std::empty()" << std::endl;
    if (my_vector.empty()) {
        std::cout << "The vector is empty. \n" << std::endl;
    }
    else {
        std::cout << "The vector is not empty. \n" << std::endl;
    }
    return 0;
}


//                                     draft 
// ----------------------------------------------------------------------------------