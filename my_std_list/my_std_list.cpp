#include <iostream>
#include <initializer_list>

namespace my_std {
    template<typename T>
    class list {
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

        friend void swap(list& first, list& second) {
            using std::swap;
            swap(first.head, second.head);
            swap(first.tail, second.tail);
            swap(first.size, second.size);
        }

        // There are two display methods for demonstrating the work of a doubly linked list in both directions
        void forward_display() {
            if (empty()) {
                std::cout << "There are no elements in the list." << std::endl;
            }
            else {
                std::cout << "forward: ";
                for (Node<T>* current = head; current != nullptr; current = current->next) {
                    std::cout << current->data << ". ";
                }
            }
            std::cout << "\n";
        }
        void backward_display() {
            if (empty()) {
                std::cout << "There are no elements in the list." << std::endl;
            }
            else {
                std::cout << "backward: ";
                for (Node<T>* current = tail; current != nullptr; current = current->prev) {
                    std::cout << current->data << ". ";
                }
                std::cout << "" << std::endl;
            }
        }

        Node<T>* find_Node(const size_t i) const {
            size_t count = 0;
            for (Node<T>* current = head; current != nullptr; current = current->next) {
                if (count == i) {
                    return current;
                }
                count++;
            }
            return nullptr;
        }

    public:
        list() : head(nullptr), tail(nullptr), size(0) {}

        list(std::initializer_list<T> list) {
            Node<T>* current = nullptr;
            for (const T& i : list) {
                Node<T>* newNode = new Node<T>{ i, nullptr, nullptr };
                if (!head) {
                    head = newNode;
                    current = newNode;
                }
                else {
                    current->next = newNode;
                    newNode->prev = current;
                    current = newNode;
                }
                size++;
            }
            tail = current;
            display();
        }
        list(list& other) {
            Node<T>* current = nullptr;
            for (Node<T>* other_current = other.head; other_current != nullptr; other_current = other_current->next) {
                Node<T>* newNode = new Node<T>{ other_current->data, nullptr, nullptr };
                if (!head) {
                    head = newNode;
                    current = newNode;
                }
                else {
                    current->next = newNode;
                    newNode->prev = current;
                    current = newNode;
                }
                tail = current;
            }
            display();
        }
        list(list&& other) noexcept {
            this->head = other.head;
            this->tail = other.tail;
            this->size = other.size;
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;
        }
        list& operator=(list other) {
            using std::swap;
            swap(*this, other);
            display();
            return *this;
        }
        ~list() {
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
                head = nullptr;
                tail = nullptr;
            }
            else {
                return;
            }
        }

        static void ruleoffive_list_demo() {
            my_std::list<int> list = { 0, 1, 1, 2, 3, 5, 8, 13, 21 };
            my_std::list<int> list_c_copy(list);
            my_std::list<int> list_c_move = std::move(list_c_copy);
            my_std::list<int> list_op_cas = list_c_move;
            std::cout << "" << std::endl;
        }

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
        void push_front(const T& val) {
            Node<T>* newNode = new Node<T>{ val, nullptr, nullptr };
            if (head == nullptr) {
                head = newNode;
                tail = head;
            }
            else {
                newNode->next = head;
                head = newNode;
            }
            display();
        }
        void pop_front() {
            if (head != nullptr) {
                if (head->next != nullptr) {
                    Node<T>* old_head = head;
                    Node<T>* new_head = head->next;
                    head = new_head;
                    delete old_head;
                    new_head->prev = nullptr;
                }
                else {
                    delete head;
                    head = nullptr;
                    tail = nullptr;
                }
                display();
                size--;
            }
            else {
                return;
            }
        }
        // push_back() and pop_back() in std::list have O(1) while std::forward_list - 0(N)
        void push_back(const T& val) {
            Node<T>* newNode = new Node<T>{ val, nullptr, nullptr };
            if (head == nullptr) {
                tail = newNode;
                head = newNode;
            }
            else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            size++;
            display();
        }
        void pop_back() {
            if (head == nullptr) {
                return;
            }
            else {
                if (head->next == nullptr) {
                    delete head;
                    head = nullptr;
                    tail = nullptr;
                }
                else {
                    Node<T>* oldNode = tail;
                    tail = tail->prev;
                    delete oldNode;
                    tail->next = nullptr;
                }
                size--;
            }
            display();
        }
        void insert_after(Node<T>* firstNode, const T& val) {
            if (head == nullptr) {
                head = new Node<T>{ val, nullptr, nullptr };
                tail = head;
            }
            else {
                Node<T>* newNode = new Node<T>{ val, firstNode->next, firstNode };
                firstNode->next = newNode;
                if (newNode->next != nullptr) {
                    newNode->next->prev = newNode;
                }
                else {
                    tail = newNode;
                }
            }
            size++;
            display();
        }
        void erase_after(Node<T>* it) {
            if (it->next == nullptr) {
                return;
            }
            Node<T>* oldNode = it->next;
            it->next = oldNode->next;
            if (oldNode->next != nullptr) {
                Node<T>* prev_upd = oldNode->next;
                prev_upd->prev = it;
            }
            else {
                tail = it;
            }
            delete oldNode;
            size--;
            display();
        }
        void resize(const size_t size) {
            size_t old_size = this->size;
            size_t new_size = size;
            if (old_size < new_size) {
                Node<T>* current = tail;
                for (size_t i = old_size; i < new_size; i++) {
                    Node<T>* newNode = new Node<T>{ T(), nullptr, current };
                    current->next = newNode;
                    current = newNode;
                }
                tail = current;
            }
            else if (old_size > new_size) {
                Node<T>* lastNode = find_Node(new_size - 1);
                Node<T>* current_for_prev = nullptr;
                for (Node<T>* current = tail; current != lastNode; current = current_for_prev) {
                    current_for_prev = current->prev;
                    delete current;
                }
                tail = lastNode;
                tail->next = nullptr;
            }
            else if (new_size == 0) {
                clear();
            }
            else {
                return;
            }
            this->size = new_size;
            display();
        }
        bool empty() {
            return !head;
        }

        Node<T>* begin() {
            return head;
        }

        void display() {
            forward_display();
            backward_display();
        }

    };
}

int main()
{
    std::cout << "Rule of five for std::list:" << std::endl;
    my_std::list<int>::ruleoffive_list_demo();

    my_std::list<int> init_list = { 5, 15, 20, 25, 30, 35 };
    init_list.display();

    std::cout << "my_std::list.push_front(404)" << std::endl;
    init_list.push_front(404);
    std::cout << "my_std::list.pop_front()." << std::endl;
    init_list.pop_front();
    std::cout << "my_std::list.insert_after(123)." << std::endl;
    auto it_insert = init_list.begin();
    init_list.insert_after(it_insert, 123);
    std::cout << "my_std::list.erase_after()." << std::endl;
    auto it_erase = init_list.begin();
    init_list.erase_after(it_erase);
    std::cout << "my_std::MyForwardList.resize() (to bigger size)." << std::endl;
    init_list.resize(10);
    std::cout << "my_std::MyForwardList.resize() (to smaller size)." << std::endl;
    init_list.resize(3);
    std::cout << "my_std::MyForwardList.clear()." << std::endl;
    init_list.clear();
    init_list.display();
    std::cout << "my_std::MyForwardList.empty()." << std::endl;
    if (init_list.empty()) {
        std::cout << "forward list is empty." << std::endl;
    }
    else {
        std::cout << "forward list is NOT empty." << std::endl;
    }

    return 0;
}