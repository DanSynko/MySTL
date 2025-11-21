#include <iostream>
#include <initializer_list>

namespace my_std {
    template<typename T>
    class MyList {
    private:
        template<typename T>
        struct Node {
            T data;
            Node<T>* next;
            Node<T>* prev;
        };
        Node<T>* head;
        Node<T>* tail;
        size_t size;

        friend void swap(MyList& first, MyList& second) {
            using std::swap;
            swap(first.head, second.head);
            swap(first.tail, second.tail);
            swap(first.size, second.size);
        }
    public:
        MyList() : head(nullptr), tail(nullptr), size(0) {}
        MyList(std::initializer_list<T> list) {}
        MyList& operator=(MyList other) {
            using std::swap;
            swap(*this, other);
            display();
            return *this;
        }
        ~MyList() {
            // clear();
        }
        void clear() {}

        /*static void ruleoffive_list_demo() {
            my_std::MyList<int> list = { 0, 1, 1, 2, 3, 5, 8, 13, 21 };
            my_std::MyList<int> list_c_copy(list);
            my_std::MyList<int> list_c_move = std::move(list_c_copy);
            my_std::MyList<int> list_op_cas;
            my_std::MyList<int> list_c_move;
            std::cout << "" << std::endl;
        }*/

        T& operator[](const size_t i) {}
        void push_front() {}
        void pop_front() {}
        // push_back() and pop_back() in std::list have O(1) while std::forward_list - 0(N)
        void push_back() {}
        void pop_back() {}
        void insert_after() {}
        void erase_after() {}

        bool empty() {
            return !head;
        }

        void display() {}

    };
}

int main()
{
    std::cout << "Hello World!\n";

    return 0;
}