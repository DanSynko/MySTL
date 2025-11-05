#include <iostream>
#include <string>
#include <utility>

namespace my_std {
    template<typename T>
    class MyVector {
    private:
        T* arr;
        T* begin;
        T* end;
        T* end_capacity;
        int arr_size;
        int arr_capacity;
    public:
        MyVector() : arr_size(5), arr_capacity(20) {
            arr = new T[arr_capacity];
            begin = arr;
            end = &arr[arr_size];
            end_capacity = &arr[arr_capacity];
        }
        MyVector(MyVector& other) {
            this->arr = new T[other.arr_capacity];
            this->arr_size = other.arr_size;
            this->arr_capacity = other.arr_capacity;
            for (int i = 0; i < other.arr_size; i++) {
                this->arr[i] = other.arr[i];
            }
            this->begin = this->arr;
            this->end = &this->end[this->arr_size];
            this->end_capacity = &this->end[this->arr_capacity];
            for (int i = 0; i < arr_size; i++) {
                std::cout << i << ", ";
                std::cout << ".";
                std::cout << "" << std::endl;
            }
        }
        /*MyVector(MyVector&& other) {

        }*/
        ~MyVector() {
            delete[] arr;
            begin = nullptr;
            end = nullptr;
            end_capacity = nullptr;
        }
        MyVector& operator=(const MyVector& other) {
            delete this->arr;
            this->arr = new T[other.arr_size];
            for (int i = 0; i < other.arr_size; i++) {
                this->arr[i] = other.arr[i];
            }
            T* old_arr = other.arr;
            this->arr_size = other.arr_size;
            this->arr_capacity = other.arr_capacity;
            this->begin = this->arr;
            this->end = &this->arr[arr_size];
            this->end_capacity = &this->arr[arr_capacity];
            return *this;
        }
        /*MyVector& operator=(MyVector&& other) {

            return *this;
        }*/
        void show_vector() {
            for (int i = 0; i < arr_size; i++) {
                std::cout << arr[i] << ". ";
            }
            std::cout << "" << std::endl;
        }
        void create_some_arr() {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = i;
            }
            show_vector();
        }
        void resize(int new_size) {
            if (this->arr_capacity > new_size) {
                std::cout << "old size: ";
                this->size();
                int old_size = this->arr_size;
                this->arr_size = new_size;
                if (this->arr_size > old_size) {
                    for (int i = old_size; i < this->arr_size; i++) {
                        this->arr[i] = 0;
                    }
                }
                else if (this->arr_size < old_size) {
                    for (int i = this->arr_size; i < old_size; i++) {
                        this->pop_back();
                    }
                }
                std::cout << "new size: ";
                this->size();
            }
            show_vector();
            
        }
        void push_back(T a) {
            if (arr_size < arr_capacity) {
                *end = a;
                end++;
                arr_size++;
                show_vector();
            }
            /*else if (arr_size >= arr_capacity) {

            }
            std::cout << "Element was added." << std::endl;*/
        }
        void pop_back() {
            end--;
        }
        void insert() {
            show_vector();
        }
        void erase() {
            show_vector();
        }
        T& front() {
            T& front_ref = *begin;
            return front_ref;
        }
        T& back() {
            T& back_ref = *end;
            return back_ref;
        }
        T* data() {
            return arr;
        }
        void size() {
            int i;
            for (i = 0; i < arr_size;) {
                i++;
            }
            std::cout << "vector size: " << i << "." << std::endl;
        }
        void reserve(const int& reserved_ram) {
            arr_capacity = reserved_ram;
            capacity();
        }
        void capacity() {
            std::cout << "vector capacity size: " << arr_capacity << "." << std::endl;
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
    my_vector.resize(3);
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
    /*std::cout << "my_std::insert()" << std::endl;
    my_vector.insert();*/
    return 0;
}


//                                   draft 
// ----------------------------------------------------------------------------------
// resize

/*std::cout << "" << std::endl;
            show_vector();
            std::cout << "old size: ";
            this->.size();
            T* new_arr = new T[new_size];
            for (int i = 0; i < this->arr_size; i++) {
                new_arr[i] = this->arr[i];
            }
            T* old_arr = this->arr;
            this->arr = new_arr;
            int old_size = this->arr_size;
            this->arr_size = new_size;
            this->end = &new_arr[new_size];
            delete[] old_arr;*/
            /*MyVector new_vector;
            other = new_vector;
            std::cout << "new size: ";
            other.size();*/
            /*int test_i = 5;
            int old_size = other.arr_size;
            for (int i = old_size; i < other.arr_size; i++) {
                other.arr[i] = test_i;
                test_i++;
            }*/
            /*show_vector();
            std::cout << "" << std::endl;*/
