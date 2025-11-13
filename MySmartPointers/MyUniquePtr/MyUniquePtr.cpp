#include <iostream>
#include <utility>
#include <new>
#include <stdexcept>

namespace my_std {
    template<typename T>
    class unique_ptr_base {
    private:
        T* ptr;
    public:
        unique_ptr_base(const T& ptr) = delete;

        void operator()() {

        }

        unique_ptr_base(T* ptr) {
            this->ptr = ptr;
        }
        void move(T* other_ptr) {
            ptr = other_ptr;
            ptr = nullptr;
        }
        T* get() {
            return ptr;
        }
        T* release() {
            ptr = nullptr;
            return ptr;
        }
        void reset() {
            delete ptr;
            ptr = nullptr;
        }
    };

    template<typename T>
    class unique_ptr_base<T[]> {
    private:
        T* ptr;
    public:
        unique_ptr_base(T* ptr) {
            this->ptr = ptr;
        }
        ~unique_ptr_base() {
            delete[] ptr;
        }
        void move(T* other_ptr) {
            ptr = other_ptr;
            ptr = nullptr;
        }
        T* get() {
            return ptr;
        }
        T* release() {
            T* old_ptr = ptr
            ptr = nullptr;
            return old_ptr;
        }
        void reset() {
            delete[] ptr;
            ptr = nullptr;
        }
    };
    class MyUniquePtrVal : public unique_ptr_base<T> {
        /*~MyUniquePtrVal() {
            delete ptr;
        }*/
    };
    class MyUniquePtrArr : public unique_ptr_base<T[]> {
    private:
    public:
        /*~MyUniquePtrArr() {
            delete[] ptr;
        }*/
    };
}

class Test {
public:
    Test() {
        std::cout << "Test constructor was called." << std::endl;
    }
    ~Test() {
        std::cout << "Test destructor was called." << std::endl;
    }
};
int main()
{
    std::cout << "Hello World!\n" << std::endl;

    return 0;
}
