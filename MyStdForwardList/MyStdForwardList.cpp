#include <iostream>
#include <initializer_list>

namespace my_std {
    template<typename T>
    class MyForwardList {
    private:
        template<typename T>
        struct Node {
            T data;
            Node<T>* next;
        };
        Node<T>* head;
        size_t size;

        Node<T>* find_Node(Node<T>* needed_Node) {
            for (Node<T>* current = head; current != nullptr; current = current->next) {
                if (current == needed_Node) {
                    return current;
                }
            }
            return nullptr;
        }
    public:
        MyForwardList() : head(nullptr), size(0) {}

        MyForwardList(std::initializer_list<T> list) : head(nullptr), size(0) {
            Node<T>* current = nullptr;
            for (const T& i : list) {
                Node<T>* newNode = new Node<T>{ i, nullptr };
                if (!head) {
                    head = newNode;
                    current = newNode;
                }
                else {
                    current->next = newNode;
                    current = newNode;
                }
                size++;
            }
        }
        ~MyForwardList() {
            clear();
        }
        void clear() {
            if (head != nullptr) {
                Node<T>* current_for_next = nullptr;
                for (Node<T>* current = head; current != nullptr; current = current_for_next) {
                    current_for_next = current->next;
                    delete current;
                    size--;
                }
            }
            else {
                return;
            }
        }

        // O(N)
        T& operator[](const size_t i) {
            size_t count = 0;
            Node* current = head;
            if (head == nullptr) {
                throw std::out_of_range("Error: out_of_range");
            }
            else {
                while (count != i) {
                    if (current->next == nullptr) {
                        return current->data;
                    }
                    current = current->next;
                    count++;
                }
            }
            return current->data;
        }
        // O(1)
        void push_front(const T& data) {
            Node<T>* newNode = new Node<T>{ data, head };
            head = newNode;
            display();
        }
        void insert_after() {

        }
        void erase_after() {

        }
        void resize(const size_t size) {
            if (this->size < size) {
                find_Node();
            }
            else if (this->size > size) {
                find_Node();
            }
            else {
                return;
            }
        }
        // O(1)
        void pop_front() {
            if (head != nullptr) {
                Node<T>* old_head = head;
                Node<T>* new_head = head->next;
                head = new_head;
                delete old_head;
            }
            else {
                return;
            }
        }
        void display() {
            if (empty()) {
                std::cout << "There are no elements in the list." << std::endl;
            }
            else {
                for (Node<T>* current = head; current != nullptr; current = current->next) {
                    std::cout << current->data << ". ";
                }
                std::cout << "" << std::endl;
            }
        }
        bool empty() {
            return !head;
        }
    };
}

int main()
{
    my_std::MyForwardList<int> forward_list;
    forward_list.display();
    std::cout << "" << std::endl;
    my_std::MyForwardList<int> init_forward_list = {11, 22, 45, 32};
    std::cout << "Initializing a new forward list." << std::endl;
    init_forward_list.display();
    std::cout << "my_std::MyForwardList.push_front()" << std::endl;
    init_forward_list.push_front(404);
    std::cout << "my_std::MyForwardList.pop_front()." << std::endl;
    init_forward_list.pop_front();
    return 0;
}