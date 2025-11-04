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
        MyVector() : arr_size(5), arr_capacity(10) {
            arr = new T[arr_size];
            begin = arr;
            end = &arr[arr_size];
        }
        MyVector(MyVector& other) {
            this->arr = new T[other.arr_size];
            for (int i = 0; i < other.arr_size; i++) {
                this->arr[i] = other.arr;
            }
            for (int i = 0; i < arr_size; i++) {
                std::cout << i << ", ";
                std::cout << ".";
                std::cout << "" << std::endl;
            }
        }
        MyVector(MyVector&& other) {

        }
        ~MyVector() {
            delete[] arr;
            begin = nullptr;
            end = nullptr;
            end_capacity = nullptr;
        }
        MyVector& operator=(MyVector& other) {

            return;
        }
        MyVector& operator=(MyVector&& other) {

            return;
        }
        void create_some_arr() {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = i;
            }
            for (int i = 0; i < arr_size; i++) {
                std::cout << i << ", ";
            }
            std::cout << ".";
            std::cout << "\n" << std::endl;
        }
        void resize(MyVector& other, int new_size) {
            std::cout << "old size: ";
            other.size();
            T* new_arr = new T[new_size];
            for (int i = 0; i < other.arr_size; i++) {
                new_arr[i] = other.arr[i];
            }
            T* old_arr = other.arr;
            other.arr = new_arr;
            other.arr_size = new_size;
            other.end = &new_arr[new_size];
            delete[] old_arr;
            std::cout << "new size: ";
            other.size();
            std::cout << "" << std::endl;
        }
        //void push_back(const T& a) {
        //    if (arr_size <= arr_capacity) {
        //        end[arr_size]
        //    }
        //    else if (arr_size >= arr_capacity) {

        //    }
        //    //std::cout << "Element was added." << std::endl;
        //}
        void insert() {

        }
        void erase() {

        }
        void size() {
            int i;
            for (i = 0; i < arr_size;) {
                i++;
            }
            std::cout << "vector size: " << i << "." << std::endl;
            std::cout << "\n" << std::endl;
        }
        /*void reserve() {

        }*/
        void capacity() {
            /*int sum = 0;
             for (int i = 0; i < arr_size; i++) {
                sum += sizeof(arr[i]);
            }*/
            std::cout << "vector capacity size: " << arr_capacity << "." << std::endl;
        }
    };
}

int main()
{
    my_std::MyVector<int> my_vector;
    //std::cout << "You have an empty vector<int>. Add a new element..." << std::endl;
    my_vector.create_some_arr();
    my_vector.size();
    my_vector.capacity();
    my_vector.resize(my_vector, 10);
    //my_vector.push_back(5);
    return 0;
}
