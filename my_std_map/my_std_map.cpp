#include <iostream>
#include <initializer_list>

template<typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::pair<const Key, Value>& pair) {
    return os << pair.first << ": " << pair.second << ";" << std::endl;
}

namespace my_std {

    template<typename Key, typename Value>
    class RedBlackTree {
    public:
        class Iterator;
        using value_type = std::pair<const Key, Value>;
    protected:
        enum Color {
            black,
            red
        };
    private:
        struct Node {
            value_type data;
            Node* right;
            Node* left;
            Node* parent;
            Color color;
        };
        Node* root;
        size_t tree_size = 0;

        friend void swap(RedBlackTree& first, RedBlackTree& second) {
            using std::swap;
            swap(first.root, second.root);
        }
        Node* preorder_copy_recursive(Node* other_node, Node* new_parent) {
            if (other_node == nullptr) return nullptr;

            Node* newNode = new Node{ other_node->data, other_node->left, other_node->right, new_parent, other_node->color };

            newNode->left = preorder_copy_recursive(other_node->left, newNode);
            newNode->right = preorder_copy_recursive(other_node->right, newNode);

            return newNode;
        }






        void inorder_traversal_recursive(Node* current) {
            if (current == nullptr) {
                return;
            }
            inorder_traversal_recursive(current->left);
            std::cout << current->data << ". ";
            inorder_traversal_recursive(current->right);
        }
        void postorder_traversal_recursive(Node* current) {
            if (current == nullptr) {
                return;
            }
            postorder_traversal_recursive(current->left);
            postorder_traversal_recursive(current->right);
            std::cout << current->data << ". ";
        }
        void preorder_traversal_recursive(Node* current) {
            if (current == nullptr) {
                return;
            }
            std::cout << current->data << ". ";
            preorder_traversal_recursive(current->left);
            preorder_traversal_recursive(current->right);
        }




        Node* inorder_predecesor(Node* current) {
            while (current != nullptr && current->right != nullptr) {
                current = current->right;
            }
            return current;
        }
        Node* inorder_succesor(Node* current) {
            if (current->left == nullptr) {
                return current;
            }
            inorder_succesor(current->left);
        }






        Node* search_recursive(Node* current, const Key& pair) {
            if (current == nullptr) {
                return nullptr;
            }
            if (current->data.first != pair.first) {
                if (current->data.first > pair.first) {
                    return search_recursive(current->left, pair);
                }
                else {
                    return search_recursive(current->right, pair);
                }
            }
            return current;
        }



        Node* fixup_properties_insert(Node* current) {
            // left rotation
            if ((current->right != nullptr && current->right->right != nullptr) && (current->right->color == Color::red && current->right->right->color == Color::red)) {
                if (current->left == nullptr) {
                    current = left_rotation(current);
                    current->color = Color::black;
                    current->left->color = Color::red;
                    current->right->color = Color::red;
                }
                else if (current->left->color == Color::black) {
                    current = left_rotation(current);
                    recoloring(current->left);
                }
                else {
                    recoloring(current->left);
                }
            }
            // right rotation
            else if ((current->left != nullptr && current->left->left != nullptr) && (current->left->color == Color::red && current->left->left->color == Color::red)) {
                if (current->right == nullptr) {
                    current = right_rotation(current);
                    current->color = Color::black;
                    current->right->color = Color::red;
                    current->left->color = Color::red;
                }
                else if (current->left->color == Color::black) {
                    current = right_rotation(current);
                    recoloring(current->right);
                }
                else {
                    recoloring(current->right);
                }
            }
            // RL
            else if ((current->right != nullptr && current->right->left != nullptr) && (current->right->color == Color::red && current->right->left->color == Color::red)) {
                if (current->left == nullptr) {
                    current = RL_rotation(current);
                    current->color = Color::black;
                    current->left->color = Color::red;
                    current->right->color = Color::red;
                }
                else if (current->left->color == Color::black) {
                    current = RL_rotation(current);
                    current->left->color = Color::red;
                    current->left->parent->color = Color::black;
                }
                else {
                    recoloring(current->left);
                }
            }
            // LR
            else if ((current->left != nullptr && current->left->right != nullptr) && (current->left->color == Color::red && current->left->right->color == Color::red)) {
                if (current->right == nullptr) {
                    current = LR_rotation(current);
                    current->color = Color::black;
                    current->right->color = Color::red;
                    current->left->color = Color::red;
                }
                else if (current->right->color == Color::black) {
                    current = LR_rotation(current);
                    recoloring(current->right);
                }
                else {
                    recoloring(current->right);
                }
            }
            return current;
        }

        void fixup_properties_remove(Node* double_black_node, Node* deleted_parent, value_type& double_black_node_data, Color deleted_color) {
            if (deleted_color == Color::red) {
                return;
            }
            else if (deleted_color == Color::black && (double_black_node != nullptr && double_black_node->color == Color::red)) {
                double_black_node->color = Color::black;
                return;
            }


            if (double_black_node != nullptr && double_black_node == root) {
                double_black_node->color = Color::black;
                return;
            }


            Node* brother;
            if (double_black_node != root && deleted_parent != nullptr) {
                brother = (double_black_node == deleted_parent->left) ? deleted_parent->right : deleted_parent->left;
            }
            else {
                if (root != nullptr) root->color = Color::black;
                return;
            }


            if (brother->color == Color::red) {
                deleted_parent = left_rotation(deleted_parent);
                deleted_parent->color = brother->color;
                brother->color = Color::black;
            }
            else if (brother->color == Color::black && (brother->left != nullptr && brother->left->color == Color::red)) {
                Color sibling_color = brother->color;
                Color sibling_child_color = brother->left->color;
                brother->color = sibling_child_color;
                brother->left->color = sibling_color;
                brother = right_rotation(brother);
            }
            if (brother->color == Color::black && (brother->right != nullptr && brother->right->color == Color::red)) {
                deleted_parent = left_rotation(deleted_parent);
                deleted_parent->color = brother->color;
                deleted_parent->left->color = Color::black;
                deleted_parent->right->color = Color::black;
            }
            else if (brother->color == Color::black && (brother->left != nullptr && brother->right != nullptr && (brother->left->color == Color::black && brother->right->color == Color::black))) {
                if (deleted_parent->color == Color::red) {
                    brother->color = Color::red;
                    deleted_parent->color = Color::black;
                }
                else {
                    fixup_properties_remove(deleted_parent, deleted_parent->parent, double_black_node_data, Color::black);
                }
            }
            else if (brother->color == Color::black && (brother->left == nullptr && brother->right == nullptr)) {
                if (deleted_parent->color == Color::red) {
                    brother->color = Color::red;
                    deleted_parent->color = Color::black;
                }
                else {
                    brother->color = Color::red;
                    fixup_properties_remove(deleted_parent, deleted_parent->parent, double_black_node_data, Color::black);
                }
            }
        }

        Node* insert_recursive(Node* current, Node* current_parent, const value_type& pair) {
            if (current != nullptr && current->data.first == pair.first) {
                return nullptr;
            }
            else {
                if (current == nullptr) {
                    tree_size++;
                    return new Node{ pair, nullptr, nullptr, current_parent, Color::red };
                }
                else {
                    if (current->data.first > pair.first) {
                        current->left = insert_recursive(current->left, current, pair);
                    }
                    else {
                        current->right = insert_recursive(current->right, current, pair);
                    }
                }
                return fixup_properties_insert(current);
            }
        }
        void remove_iterative(Node* current, const Key& pair) {
            while (current != nullptr && current->data.first != pair.first) {
                if (pair.first < current->data.first) {
                    current = current->left;
                }
                else if (pair.first > current->data.first) {
                    current = current->right;
                }
            }


            if (current == nullptr) {
                return;
            }
            else if (current == root && (root->left == nullptr || root->right == nullptr)) {
                Node* new_root = (root->left == nullptr) ? root->right : root->left;
                if (new_root != nullptr) {
                    new_root->parent = nullptr;
                    new_root->color = Color::black;
                }
                delete root;
                root = new_root;
                tree_size = 0;
                return;
            }
            else {
                if (current->left != nullptr && current->right != nullptr) {
                    Node* temp = inorder_predecesor(current->left);
                    Node* current_parent = current->parent;
                    value_type temp_data = temp->data;
                    Color current_color = current->color;
                    remove_iterative(temp, temp->data);
                    current->data = temp_data;
                    tree_size--;
                }
                else if (current->left == nullptr || current->right == nullptr) {
                    Node* temp = (current->left == nullptr) ? current->right : current->left;
                    Color current_color = current->color;
                    Node* current_parent = current->parent;
                    value_type current_data = current->data;
                    if (current == current->parent->left) {
                        current->parent->left = temp;
                    }
                    else {
                        current->parent->right = temp;
                    }
                    if (temp != nullptr) temp->parent = current->parent;
                    delete current;
                    fixup_properties_remove(temp, current_parent, current_data, current_color);
                    tree_size--;
                }
            }
        }


        void recoloring(Node* uncle) {
            if (uncle->color == Color::red) {
                if (uncle->parent == root) {
                    uncle->parent->color = Color::black;
                }
                else {
                    uncle->parent->color = Color::red;
                }
                uncle->color = Color::black;
                if (uncle == uncle->parent->right) {
                    uncle->parent->right->color = Color::black;
                }
                else {
                    uncle->parent->left->color = Color::black;
                }
            }
        }


        Node* left_rotation(Node* current) {
            Node* old_parent = current;
            Node* new_parent = current->right;
            Node* subtree = new_parent->left;
            if (subtree != nullptr) {
                subtree->parent = old_parent;
            }
            if (old_parent->parent != nullptr) {
                if (old_parent == old_parent->parent->left) {
                    old_parent->parent->left = new_parent;
                }
                else {
                    old_parent->parent->right = new_parent;
                }
            }
            else {
                root = new_parent;
            }
            new_parent->left = old_parent;
            old_parent->right = subtree;
            new_parent->parent = old_parent->parent;
            old_parent->parent = new_parent;
            return new_parent;
        }
        Node* right_rotation(Node* current) {
            Node* old_parent = current;
            Node* new_parent = current->left;
            Node* subtree = new_parent->right;
            if (subtree != nullptr) {
                subtree->parent = old_parent;
            }
            if (old_parent->parent != nullptr) {
                if (old_parent == old_parent->parent->left) {
                    old_parent->parent->left = new_parent;
                }
                else {
                    old_parent->parent->right = new_parent;
                }
            }
            else {
                root = new_parent;
            }
            new_parent->right = old_parent;
            old_parent->left = subtree;
            new_parent->parent = old_parent->parent;
            old_parent->parent = new_parent;
            return new_parent;
        }
        Node* RL_rotation(Node* current) {
            Node* old_parent = current;
            Node* new_parent = current->right->left;
            Node* subnode = current->right;
            subnode = right_rotation(subnode);
            old_parent = left_rotation(old_parent);
            return new_parent;
        }
        Node* LR_rotation(Node* current) {
            Node* old_parent = current;
            Node* new_parent = current->left->right;
            Node* subnode = current->left;
            subnode = left_rotation(subnode);
            old_parent->left = subnode;
            old_parent = right_rotation(old_parent);
            return new_parent;
        }


        void postorder_delete_recursive(Node* current) {
            if (current == nullptr) {
                return;
            }
            postorder_delete_recursive(current->left);
            postorder_delete_recursive(current->right);
            delete current;
        }
    public:

        RedBlackTree() : root(nullptr) {}

        RedBlackTree(std::initializer_list<value_type> tree) : root(nullptr) {
            for (const auto& item : tree) {
                if (root == nullptr) {
                    root = new Node{ item, nullptr, nullptr, nullptr, Color::black };
                }
                else {
                    insert(item);
                }
            }
        }
        RedBlackTree(const RedBlackTree& other) : root(nullptr) {
            this->tree_size = other.tree_size;
            root = preorder_copy_recursive(other.root, nullptr);
        }
        RedBlackTree(RedBlackTree&& other) noexcept {
            this->root = other.root;
            other.root = nullptr;
        }
        RedBlackTree& operator =(RedBlackTree other) {
            using std::swap;
            swap(*this, other);
            return *this;
        }




        ~RedBlackTree() {
            postorder_delete_recursive(root);
            root = nullptr;
        }




        Iterator search(const Key& val) {
            Iterator find_val(search_recursive(root, val));
            return find_val;
        }
        void insert(const value_type& pair) {
            Node* next_parent = root;
            root = insert_recursive(root, next_parent, pair);
        }
        void remove(const Key& val) {
            remove_iterative(root, val);
        }
        void inorder_traversal() {
            inorder_traversal_recursive(root);
        }
        void postorder_traversal() {
            postorder_traversal_recursive(root);
        }
        void preorder_traversal() {
            preorder_traversal_recursive(root);
        }




        class Iterator {
        private:
            friend class RedBlackTree;
            using value_type = std::pair<const Key, Value>;
            Node* ptr;
            Iterator(Node* current) : ptr(current) {}
        public:
            Iterator() : ptr(nullptr) {}

            Iterator& operator++() {
                if (ptr->right != nullptr) {
                    ptr = ptr->right;
                    while (ptr != nullptr && ptr->left != nullptr) {
                        ptr = ptr->left;
                    }
                }
                else {
                    if (ptr->parent != nullptr && ptr->data.first < ptr->parent->data.first) {
                        ptr = ptr->parent;
                    }
                    else if (ptr->parent != nullptr && ptr->data.first > ptr->parent->data.first) {
                        while (ptr->parent != nullptr && ptr->data.first > ptr->parent->data.first) {
                            ptr = ptr->parent;
                        }
                        ptr = (ptr->parent != nullptr) ? ptr->parent : nullptr;
                    }
                }
                return *this;
            }
            Iterator& operator--() {
                if (ptr->left != nullptr) {
                    ptr = ptr->left;
                    while (ptr != nullptr && ptr->right != nullptr) {
                        ptr = ptr->right;
                    }
                }
                else {
                    if (ptr->parent != nullptr && ptr->data.first > ptr->parent->data.first) {
                        ptr = ptr->parent;
                    }
                    else if (ptr->parent != nullptr && ptr->data.first < ptr->parent->data.first) {
                        while (ptr->parent != nullptr && ptr->data.first < ptr->parent->data.first) {
                            ptr = ptr->parent;
                        }
                        ptr = (ptr->parent != nullptr) ? ptr->parent : nullptr;
                    }
                }
                return *this;
            }

            bool operator==(const Iterator& other) const {
                return ptr == other.ptr;
            }
            bool operator!=(const Iterator& other) const {
                return ptr != other.ptr;
            }
            value_type& operator*() const {
                return ptr->data;
            }
            value_type* operator->() {
                return &(ptr->data);
            }
        };

        Iterator begin() {
            Node* current = (root == nullptr) ? nullptr : root;
            while (current != nullptr && current->left != nullptr) {
                current = current->left;
            }
            return Iterator(current);
        }
        Iterator end() {
            return Iterator(nullptr);
        }


        void clear() {
            postorder_delete_recursive();
            root = nullptr;
            tree_size = 0;
        }
        bool empty() {
            return !root;
        }
        const size_t size() const {
            return tree_size;
        }
        bool contains(const Key& val) {
            if (root == nullptr) return false;

            Node* current = root;
            while (current != nullptr && current->data != val) {
                current = (current->data < val) ? current->right : current->left;
            }

            return current != nullptr;
        }

        static void ruleoffive_rbt_pair_demo() {
            my_std::RedBlackTree<std::string, int> rbt_example = {
                {"current year", 2025},
                {"current month", 12},
                {"hours in day", 24},
                {"Google company formation", 1998}
            };
            rbt_example.inorder_traversal();
            my_std::RedBlackTree<std::string, int> rbt_example_c_copy(rbt_example);
            rbt_example_c_copy.inorder_traversal();
            my_std::RedBlackTree<std::string, int> rbt_example_c_move = std::move(rbt_example_c_copy);
            rbt_example_c_move.inorder_traversal();
            my_std::RedBlackTree<std::string, int> rbt_example_op_cas;
            rbt_example_op_cas.inorder_traversal();
            rbt_example_op_cas = rbt_example_c_move;
            rbt_example_op_cas.inorder_traversal();
            std::cout << "" << std::endl;
        }
    };

    template<typename Key, typename Value>
    class map {
    private:
        using Iterator = typename RedBlackTree<Key, Value>::Iterator;
        using value_type = std::pair<const Key, Value>;
        RedBlackTree<Key, Value> rbt;
        friend void swap(map& first, map& second) {
            using std::swap;
            swap(first.rbt, second.rbt);
        }
    public:
        map(std::initializer_list<value_type> tree) {
            for (const auto& item : tree) {
                insert(item);
            }
        }
        map& operator=(map other) {
            using std::swap;
            swap(*this, other);
            return *this;
        }


        void insert(const value_type& pair) {
            rbt.insert(pair);
        }
        void erase(const Key& pair) {
            rbt.remove(pair);
        }
        Iterator find(const Key& pair) {
            rbt.search(pair);
        }

        bool contains(const Key& pair) {
            return rbt.contains(pair);
        }

        Iterator begin() {
            return rbt.begin();
        }
        Iterator end() {
            return rbt.end();
        }

        const size_t size() const {
            return rbt.size();
        }

        bool empty() {
            return rbt.empty();
        }


        static void ruleoffive_map_demo() {
            RedBlackTree<std::string, int>::ruleoffive_rbt_pair_demo();
        }

    };
}
int main()
{
    std::cout << "Welcome to my_std_set! There is a set<T> on red-black tree: " << std::endl;

    my_std::map<std::string, int>::ruleoffive_map_demo();

    my_std::map<std::string, int> my_map = { 
        {"current year", 2025},
        {"current month", 12},
        {"hours in day", 24},
        {"Google company formation", 1998}
    };

    auto it = my_map.begin();
    ++it;
    --it;

    return 0;
}