#include <iostream>
#include <string>
#include <utility>
#include <new>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace my_std {
    template<typename T>
    class vector {
    private:
        T* arr;
        T* begin_it;
        T* end_it;
        T* end_capacity;
        size_t arr_size;
        size_t arr_capacity;

        void destroy_elements() {
            for (T* ptr = begin_it; ptr != end_it; ++ptr) {
                ptr->~T();
            }
        }
        void update_pointers() {
            begin_it = arr;
            end_it = &arr[arr_size];
            end_capacity = &arr[arr_capacity];
        }
        friend void swap(vector& first, vector& second) {
            using std::swap;
            swap(first.arr, second.arr);
            swap(first.begin_it, second.begin_it);
            swap(first.end_it, second.end_it);
            swap(first.end_capacity, second.end_capacity);
            swap(first.arr_size, second.arr_size);
            swap(first.arr_capacity, second.arr_capacity);
        }
        void create_some_vec_obj() {
            for (int i = 0; i < arr_size; i++) {
                new (&arr[i]) T();
            }
            show_vector();
        }
        void reallocation() {
            arr_capacity *= 1.5;
            T* new_arr = static_cast<T*>(operator new(arr_capacity * sizeof(T)));
            std::uninitialized_move(begin_it, end_it, new_arr);
            destroy_elements();
            operator delete (arr);
            arr = new_arr;
            update_pointers();
        }
    public:
        vector() : arr_size(10), arr_capacity(40) {
            arr = static_cast<T*>(operator new(arr_capacity * sizeof(T)));
            begin_it = arr;
            end_it = &arr[arr_size];
            end_capacity = &arr[arr_capacity];
            create_some_vec_obj();
        } 
        vector(const vector& other) : arr_size(other.arr_size), arr_capacity(other.arr_capacity) {
            std::cout << "The deep copy constructor was called: " << std::endl;
            this->arr = static_cast<T*>(operator new(this->arr_capacity * sizeof(T)));
            this->end_it = &this->arr[this->arr_size];
            this->begin_it = this->arr;
            std::uninitialized_copy(other.begin_it, other.end_it, this->arr);
            this->end_capacity = &this->arr[this->arr_capacity];
            std::cout << "A new vector created by deep copy constructor: " << std::endl;
            this->show_vector();
        }
        vector(vector&& other) noexcept {
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
            std::cout << "A new vector created by move constructor: " << std::endl;
            this->show_vector();
        }
        vector& operator=(vector other) noexcept {
            using std::swap;
            swap(*this, other);
            std::cout << "A new vector after overloaded operator=(with copy-and-swap): " << std::endl;
            this->show_vector();
            return *this;
        }
        ~vector() {
            std::cout << "The destructor of vector was called. " << std::endl;
            destroy_elements();
            operator delete (arr);
            begin_it = nullptr;
            end_it = nullptr;
            end_capacity = nullptr;
        }

        friend std::ostream& operator<<(std::ostream& os, const vector& other) {
            os << other;
            return os;
		}
        void show_vector() {
            for (T* ptr = begin_it; ptr != end_it; ptr++) {
                std::cout << *ptr << ". ";
            }
            std::cout << "" << std::endl;
        }
        // The method's complexity should be O(n)(linear time)
        void resize(size_t new_size) {
            if (arr_capacity <= arr_size) {
                reallocation();
            }
            std::cout << "old size: " << size() << std::endl;
            show_vector();
            size_t old_size = arr_size;
            arr_size = new_size;
            if (arr_size > old_size) {
                for (size_t i = old_size; i < arr_size; i++) {
                    new (&arr[i]) T();
                }
            }
            else if (arr_size < old_size) {
                for (size_t i = arr_size; i < old_size; i++) {
                    end_it--;
                    end_it->~T();
                }
            }
            begin_it = arr;
            end_it = &arr[arr_size];
            end_capacity = &arr[arr_capacity];
            std::cout << "new size: " << size() << std::endl;
            show_vector(); 
        }
        // The method's complexity should be O(1)(constant time)
        void push_back(const T& val) {
            if (arr_capacity <= arr_size) {
                reallocation();
            }
            new (end_it) T(val);
            end_it++;
            arr_size++;
            show_vector();
        }
        // O(1)
        void pop_back() {
            end_it--;
            arr_size--;
            end_it->~T();
        }
        bool empty() {
            return !arr_size;
        }
        T& front() {
            return *begin_it;
        }
        T& back() {
            return *(end_it - 1);
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
        size_t size() {
            return arr_size;
        }
        void reserve(const size_t reserved_ram) {
            arr_capacity = reserved_ram;
            T* new_arr = static_cast<T*>(operator new(arr_capacity * sizeof(T)));
            std::uninitialized_move(begin_it, end_it, new_arr);
            destroy_elements();
            operator delete (arr);
            arr = new_arr;
            update_pointers();
            show_vector();
            std::cout << "Capacity size of vector above: " << capacity() << "\n" << std::endl;
        }
        size_t capacity() {
            return arr_capacity;
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
            destroy_elements();
            arr_size = 0;
            end_it = &arr[arr_size];
        }
        void shrink_to_fit() {
            arr_capacity = arr_size;
            T* new_arr = static_cast<T*>(operator new(arr_capacity * sizeof(T)));
            std::uninitialized_move(begin_it, end_it, new_arr);
            destroy_elements();
            operator delete (arr);
            arr = new_arr;
            update_pointers();
            show_vector();
        }
    };
}
template<typename TT>
class Test {
private:
    TT* t;
    TT* t_end;
    TT* t_end_capacity;
    size_t t_size;
    size_t t_capacity;
    std::string text;

    void t_destroy_elements() {
        for (TT* ptr = t; ptr != t_end; ++ptr) {
            ptr->~TT();
        }
    }
    friend void swap(Test& first, Test& second) {
        using std::swap;
        swap(first.t, second.t);
        swap(first.t_end, second.t_end);
        swap(first.t_end_capacity, second.t_end_capacity);
        swap(first.t_size, second.t_size);
        swap(first.t_capacity, second.t_capacity);
        swap(first.text, second.text);
    }
    void create_some_t_arr_int() {
        for (int i = 0; i < t_size; i++) {
            new (&t[i]) TT(i);
        }
    }
public:
    Test() : t_size(3), t_capacity(5), text("'Test'-obj, ") {
        t = static_cast<TT*>(operator new(t_capacity * sizeof(TT)));
        t_end = &t[t_size];
        t_end_capacity = &t[t_capacity];
        create_some_t_arr_int();
        this->show_t_arr();
    }
    Test& operator=(const Test other) noexcept {
        using std::swap;
        swap(*this, other);
        return *this;
    }
    Test(const Test& other) : t_size(other.t_size), t_capacity(other.t_capacity), text(other.text) {
        this->t = static_cast<TT*>(operator new(this->t_capacity * sizeof(TT)));
        this->t_end = &this->t[this->t_size];
        std::uninitialized_copy(other.t, other.t_end, this->t);
        TT* new_t_end_capacity = &this->t[this->t_capacity];
        this->t_end_capacity = new_t_end_capacity;
        create_some_t_arr_int();
        this->show_t_arr();
    }
    Test(Test&& other) noexcept {
        this->t_size = other.t_size;
        this->t_capacity = other.t_capacity;
        this->t = other.t;
        this->t_end = other.t_end;
        this->t_end_capacity = other.t_end_capacity;
        this->text = other.text;
        other.t_size = 0;
        other.t_capacity = 0;
        other.t = nullptr;
        other.t_end = nullptr;
        other.t_end_capacity = nullptr;
        this->show_t_arr();
    }
    Test& operator=(const Test& other) {
        using std::swap;
        swap(*this, other);
        this->show_t_arr();
        return *this;
    }
    ~Test() {
        t_destroy_elements();
        operator delete (t);
        t = nullptr;
        t_end = nullptr;
        t_end_capacity = nullptr;
    }
    void show_t_arr() {
        for (TT* tptr = t; tptr != t_end; tptr++) {
            std::cout << *tptr << ". ";
        }
        std::cout << "" << std::endl;
    }
    friend std::ostream& operator<<(std::ostream& os, const Test& other) {
        os << other.text;
        return os;
    }
};
int main()
{
    std::cout << "Welcome to MyStdVector! There is a vector<custom_type>: " << std::endl;
    my_std::vector<Test<int>> my_vector;
    std::cout << "(The output of the object's memory contents(like 0, 1, 2) only occurs within its constructor.)" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::size()" << std::endl;
    std::cout << my_vector.size() << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::capacity()" << std::endl;
    std::cout << my_vector.capacity() << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::resize()" << std::endl;
    my_vector.resize(22);
    std::cout << "" << std::endl;
    std::cout << "my_std::push_back()" << std::endl;
    Test<int> some_obj;
    my_vector.push_back(some_obj);
    std::cout << "" << std::endl;
    std::cout << "my_std::pop_back()" << std::endl;
    my_vector.pop_back();
    my_vector.show_vector();
    std::cout << "" << std::endl;
    std::cout << "my_std::reserve()" << std::endl;
    my_vector.reserve(50);
    std::cout << "" << std::endl;
    std::cout << "my_std::front()" << std::endl;
    Test<int>& front_element = my_vector.front();
    std::cout << front_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::back()" << std::endl;
    Test<int>& back_element = my_vector.front();
    std::cout << back_element << std::endl;
    std::cout << "" << std::endl;
    std::cout << "my_std::data()" << std::endl;
    Test<int>& data_element = my_vector.front();
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


    std::cout << "rule of five: \n" << std::endl;

    std::cout << "deep copy constructor: " << std::endl;
    my_std::vector<Test<int>> my_vector_dp(my_vector);

    std::cout << "\n";

    std::cout << "move constructor: " << std::endl;
    std::cout << "a new vector for move constructor: " << std::endl;
    my_std::vector<Test<int>> my_vector_rval;
    my_std::vector<Test<int>> my_vector_m = std::move(my_vector_rval);

    std::cout << "\n";
    std::cout << "deep copy operator=: " << std::endl;
    std::cout << "a new vector for deep copy operator=: " << std::endl;
    my_std::vector<Test<int>> my_vector_dp_op;
    my_vector_dp_op = my_vector;

    std::cout << "\n";
    std::cout << "move operator=: " << std::endl;
    std::cout << "a new vector for move operator=: " << std::endl;
    my_std::vector<Test<int>> my_vector_rval2;
    my_vector_dp = std::move(my_vector_rval2);


    std::cout << "my_std::clear()" << std::endl;
    my_vector.clear();
    std::cout << "Vector size after clear(): " << my_vector.size() << std::endl;
    std::cout << "my_std::empty()" << std::endl;
    if (my_vector.empty()) {
        std::cout << "The vector is empty. \n" << std::endl;
    }
    else {
        std::cout << "The vector is not empty. \n" << std::endl;
    }

    return 0;
}