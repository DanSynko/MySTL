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
            this->arr = other.arr;
            this->begin_it = other.begin_it;
            this->end_it = other.end_it;
            this->end_capacity = other.end_capacity;
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
            /*for (int i = 0; i < other.arr_size; i++) {
                this->arr[i] = other.arr[i];
            }*/
            for (T* ptr = other.begin_it, i = this->arr; ptr != other.end_it; ptr++, i++) {
                *i = *ptr;
            }
            //T* old_arr = other.arr;
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
            this->arr = other.arr;
            this->begin_it = other.begin_it;
            this->end_it = other.end_it;
            this->end_capacity =      other.end_capacity;
            other.arr = nullptr;
            other.begin_it = nullptr;
            other.end_it = nullptr;
            other.end_capacity = nullptr;
            this->show_vector();
            return *this;
        }
        ~MyVector() {
            std::cout << "The destructor was called: " << std::endl;
            //std::cout << "Now desctructor for: " << *this << std::endl;
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
        void create_some_arr() {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = i+3;
            }
            show_vector();
        }
        void reallocation() {
            if (arr_capacity <= arr_size) {
                arr_capacity *= 1.5;
                T* new_arr = new T[arr_capacity];
                for (T* ptr = begin_it, i = new_arr; ptr != end_it; ptr++, i++) {
                    *i = *ptr;
                }
                delete[] arr;
                arr = new_arr;
                begin_it = arr;
                end_it = &new_arr[arr_size];
                end_capacity = &new_arr[arr_capacity];
            }
        }
        // The method's complexity should be O(n)(linear time)
        void resize(int new_size) {
            reallocation();
            std::cout << "old size: ";
            size();
            int old_size = arr_size;
            arr_size = new_size;
            if (arr_size > old_size) {
                for (int i = old_size; i < arr_size; i++) {
                    arr[i] = 1;
                }
            }
            else if (arr_size < old_size) {
                for (int i = arr_size; i < old_size; i++) {
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
        // O(1)(constant time)
        void push_back(T a) {
            reallocation();
            *end_it = a;
            end_it++;
            arr_size++;
            show_vector();
        }
        // The method's complexity should be O(1)
        void pop_back() {
            end_it--;
            arr_size--;
        }
        void insert(T* iterator_position, const int val) {
            reallocation();
            // O(n)
            if (iterator_position != end()) {
                // shift
                show_vector();
                for (T* ptr = iterator_position-1; ptr != end_capacity; ptr++) {
                    *iterator_position = *ptr;
                    //iterator_position++;
                }
                *iterator_position = val;
            }
            // O(1)
            else {
                // no shift
                push_back(val);
            }
            show_vector();
        }
        /*void insert(const T* iterator_position, int n) {
            reallocation();
        }*/
        /*void insert(const T* iterator_begin, int n, const T* iterator_end,) {
            reallocation();
        }*/
        // O(n)
        /*void erase(T* iterator_position) {
            show_vector();
        }
        void erase(T* iterator_position_begin, T* iterator_position_end) {

        }*/
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
        void size() {
            int i;
            for (i = 0; i < arr_size;) {
                i++;
            }
            std::cout << "vector size: " << i << "." << std::endl;
        }
        void reserve(const int reserved_ram) {
            arr_capacity = reserved_ram;
            capacity();
        }
        void capacity() {
            std::cout << "vector capacity size: " << arr_capacity << "." << std::endl;
        }
        /*T max_size() {
            return sizeof(arr);
        }*/
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
            show_vector();
        }
        void shrink_to_fit() {
            arr_capacity = arr_size;
            T* new_arr = new T[arr_capacity];
            for (T* ptr = begin_it, i = new_arr; ptr != end_it; ptr++, i++) {
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

int main()
{
    my_std::MyVector<int> my_vector;
    std::cout << "Welcome to MyStdVector! There is a vector<int>: ";
    my_vector.create_some_arr();
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
    my_vector.push_back(10);
    std::cout << "" << std::endl;
    std::cout << "my_std::pop_back()" << std::endl;
    my_vector.pop_back();
    my_vector.show_vector();
    std::cout << "" << std::endl;
    std::cout << "my_std::reserve()" << std::endl;
    my_vector.reserve(50);
    std::cout << "" << std::endl;
    std::cout << "my_std::front()" << std::endl;
    int& front_element = my_vector.front();
    std::cout << front_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::back()" << std::endl;
    int& back_element = my_vector.back();
    std::cout << back_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::data()" << std::endl;
    int* data_element = my_vector.data();
    std::cout << data_element << std::endl;
    std::cout << "" << std::endl;
    /*std::cout << "my_std::insert() - some value in some position" << std::endl;
    my_vector.insert(my_vector.begin() + 7, 15);*/
    /*std::cout << "my_std::insert() - some values in some position" << std::endl;
    my_vector.insert(my_vector.begin(), 15, 20, 25);
    std::cout << "my_std::insert() - some values in range" << std::endl;
    my_vector.insert(my_vector2, my_vector3.begin(), my_vector3.end());*/
    std::cout << "my_std::at() (correct work)" << std::endl;
    my_vector.at(2);
    std::cout << "" << std::endl;
    try {
        std::cout << "my_std::at() (if index is invalid)" << std::endl;
        my_vector.at(-2);
        std::cout << "" << std::endl;
    }
    catch (std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    /*std::cout << "my_std::clear()" << std::endl;
    my_vector.clear();
    my_vector.size();*/
    std::cout << "my_std::shrink_to_fit()" << std::endl;
    my_vector.shrink_to_fit();
    /*std::cout << "my_std::max_size()" << my_vector.max_size() << std::endl;
    std::cout << "" << std::endl;*/

    // rule of five
    my_std::MyVector<int> my_vector2;
    my_vector = my_vector;
    my_vector = my_vector2;
    my_std::MyVector<int> my_vector3(my_vector);

    return 0;
}


//                                   draft 
// ----------------------------------------------------------------------------------

//new_arr_capacity = arr_capacity * 1.5;
                //T* new_arr = new T[new_arr_capacity];
                //int i = 0;
                //for (T* ptr = begin_it; ptr != end_it; ptr++) {
                //    new_arr[i] = *ptr;
                //    //iterator_position++;
                //}
                //iterator_position = new_arr;
                //end_capacity = &new_arr[new_arr_capacity];
                //for (T* ptr = iterator_position + 1; ptr != end_capacity; ptr++) {
                //    *iterator_position = *ptr;
                //    //iterator_position++;
                //}
                //iterator_position = begin_it;
                //end_it = &new_arr[arr_size];
                //delete[] arr;
                //arr = new_arr;


/*void arr_shift() {

    T* new_arr = new T[];
}
*/
