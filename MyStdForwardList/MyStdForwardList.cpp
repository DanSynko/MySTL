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

        /*Node<T>* find_Node(Node<T>* needed_Node) {
            for (Node<T>* current = head; current != nullptr; current = current->next) {
                if (current == needed_Node) {
                    return current;
                }
            }
            return nullptr;
        }*/

        friend void swap(MyForwardList& first, MyForwardList& second) {
            using std::swap;
            swap(first.head, second.head);
            swap(first.size, second.size);
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
        MyForwardList(MyForwardList& other) : head(nullptr), size(other.size) {
            Node<T>* current = nullptr;
            for (Node<T>* other_current = other.head; other_current != nullptr; other_current = other_current->next) {
                Node<T>* newNode = new Node<T>{ other_current->data, nullptr };
                if (!head) {
                    head = newNode;
                    current = newNode;
                }
                else {
                    current->next = newNode;
                    current = newNode;
                }
            }
            display();
        }
        MyForwardList(MyForwardList&& other) noexcept {
            head = other.head;
            size = other.size;
            other.head = nullptr;
            other.size = 0;
            display();
        }
        MyForwardList& operator=(MyForwardList other) {
            using std::swap;
            swap(*this, other);
            display();
            return *this;
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

        static void ruleoffive_list_demo() {
            my_std::MyForwardList<int> forward_list = { 0, 1, 1, 2, 3, 5, 8, 13, 21 };
            my_std::MyForwardList<int> forward_list_c_copy(forward_list);
            my_std::MyForwardList<int> forward_list_c_move = std::move(forward_list_c_copy);
            my_std::MyForwardList<int> forward_list_op_cas;
            forward_list_op_cas = forward_list_c_move;
            std::cout << "" << std::endl;
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
        // O(1)
        void pop_front() {
            if (head != nullptr) {
                Node<T>* old_head = head;
                Node<T>* new_head = head->next;
                head = new_head;
                delete old_head;
                display();
            }
            else {
                return;
            }
        }
        // O(1)
        void insert_after(Node<T>* it, const T& val) {
            Node<T>* newNode_for_next = it->next;
            Node<T>* newNode = new Node<T>{ val, newNode_for_next };
            it->next = newNode;
            display();
        }
        // O(1)
        void erase_after(Node<T>* it) {
            Node<T>* oldNode = it->next;
            it->next = oldNode->next;
            delete oldNode;
            display();
        }
        void resize(const size_t size) {
            size_t old_size = this->size;
            size_t new_size = size;
            if (old_size < new_size) {
                Node<T>* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                for (size_t i = old_size; i < new_size; i++) {
                    Node<T>* newNode = new Node<T>{ 0, nullptr };
                    current->next = newNode;
                    current = newNode;
                }
            }
            else if (old_size > new_size) {
                Node<T>* current = head;
                for (size_t i = 0; i < new_size - 1; i++) {
                    current = current->next;
                }
                Node<T>* current_for_next = nullptr;
                while (current->next != nullptr) {
                    current_for_next = current->next;
                    delete current;
                    current = current_for_next;
                }
            }
            else if (new_size == 0) {
                clear();
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
        Node<T>* begin() {
            return head;
        }

        // Note: std::forward_list does not provide push_back() and pop_back() because it`s O(N),
        // while std::vector has O(1) for this. 
    };
}

int main()
{
    my_std::MyForwardList<int>::ruleoffive_list_demo();

    my_std::MyForwardList<int> forward_list;
    forward_list.display();
    std::cout << "" << std::endl;
    my_std::MyForwardList<int> init_forward_list = {11, 22, 45, 32, 2025};
    std::cout << "Initializing a new forward list." << std::endl;
    init_forward_list.display();
    std::cout << "my_std::MyForwardList.push_front()" << std::endl;
    init_forward_list.push_front(404);
    std::cout << "my_std::MyForwardList.pop_front()." << std::endl;
    init_forward_list.pop_front();
    std::cout << "my_std::MyForwardList.insert_after()." << std::endl;
    auto it_insert = init_forward_list.begin();
    init_forward_list.insert_after(it_insert, 123);
    std::cout << "my_std::MyForwardList.erase_after()." << std::endl;
    auto it_erase = init_forward_list.begin();
    init_forward_list.erase_after(it_erase);
    std::cout << "my_std::MyForwardList.resize() (to bigger size)." << std::endl;
    init_forward_list.resize(10);
    std::cout << "my_std::MyForwardList.resize() (to smaller size)." << std::endl;
    init_forward_list.resize(3);
    std::cout << "my_std::MyForwardList.clear()." << std::endl;
    init_forward_list.clear();
    init_forward_list.display();
    std::cout << "my_std::MyForwardList.empty()." << std::endl;
    if(init_forward_list.empty()){
        std::cout << "forward list is empty." << std::endl;
    }
    else {
        std::cout << "forward list is NOT empty." << std::endl;
    }
    

    return 0;
}