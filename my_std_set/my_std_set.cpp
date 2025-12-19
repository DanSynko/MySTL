#include <iostream>
#include <initializer_list>

namespace my_std {

    template <typename T>
    class RedBlackTree {
    protected:
        enum Color {
            black,
            red
        };
    private:
        struct Node {
            T data;
            Node* right;
            Node* left;
            Node* parent;
            Color color;
        };
        Node* root;
        size_t tree_size = 0;

        /*friend void swap(RedBlackTree& first, RedBlackTree& second) {
            using std::swap;
            swap(first.root, second.root);
        }*/
        /*Node* preorder_copy_recursive(Node* other_original, Node* other, Node* prev) {
            if (other_original->left == nullptr && other_original->right == nullptr) return nullptr;

            Node* current = other;
            Node* new_root = new Node{ current->data, nullptr, nullptr, prev, current->color };
            prev = new_root;
            new_root->left = preorder_copy_recursive(other_original, new_root->left, prev);
            new_root->right = preorder_copy_recursive(other_original, new_root->right, prev);
            return new_root;
        }*/






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




        Node* inorder_preccesor(Node* current) {
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






        Node* search_recursive(Node* current, const T& val) {
            if (current == nullptr) {
                return nullptr;
            }
            if (current->data != val) {
                if (current->data > val) {
                    return search_recursive(current->left, val);
                }
                else {
                    return search_recursive(current->right, val);
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

        void fixup_properties_remove(Node* double_black_node, Node* deleted_parent, T& double_black_node_data, Color deleted_color) {
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
                brother = (double_black_node_data < deleted_parent->data) ? deleted_parent->right : deleted_parent->left;
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

        Node* insert_recursive(Node* current, Node* current_parent, const T& val) {
            if (current->data == val) {
                return nullptr;
            }
            else {
                if (current == nullptr) {
                    return new Node{ val, nullptr, nullptr, current_parent, Color::red };
                }
                else {
                    if (current->data > val) {
                        current->left = insert_recursive(current->left, current, val);
                    }
                    else {
                        current->right = insert_recursive(current->right, current, val);
                    }
                }
                tree_size++;
                return fixup_properties_insert(current);
            }
        }
        void remove_iterative(Node* current, const T& val) {
            while (current != nullptr && current->data != val) {
                if (val < current->data) {
                    current = current->left;
                }
                else if (val > current->data) {
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
                    Node* temp = inorder_preccesor(current->left);
                    Node* current_parent = current->parent;
                    int temp_data = temp->data;
                    Color current_color = current->color;
                    remove_iterative(temp, temp->data);
                    current->data = temp_data;
                    tree_size--;
                }
                else if (current->left == nullptr || current->right == nullptr) {
                    Node* temp = (current->left == nullptr) ? current->right : current->left;
                    Color current_color = current->color;
                    Node* current_parent = current->parent;
                    int current_data = current->data;
                    if (current->data < current->parent->data) {
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
                if (uncle->data < uncle->parent->data) {
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

        RedBlackTree(std::initializer_list<T> tree) : root(nullptr) {
            for (const T& i : tree) {
                if (root == nullptr) {
                    root = new Node{ i, nullptr, nullptr, nullptr, Color::black };
                }
                else {
                    insert(i);
                }
            }
        }
        /*RedBlackTree(const RedBlackTree& other) {
            root = preorder_copy_recursive(other, other.root, );
        }*/
        RedBlackTree(RedBlackTree&& other) noexcept {
            this->root = other.root;
            other.root = nullptr;
        }
        /*RedBlackTree& operator =(RedBlackTree other) {
            using std::swap;
            swap(*this, other);
            return *this;
        }*/




        ~RedBlackTree() {
            postorder_delete_recursive(root);
            root = nullptr;
        }




        Node* search(const T& val) {
            return search_recursive(root, val);
        }
        void insert(const T& val) {
            Node* next_parent = root;
            root = insert_recursive(root, next_parent, val);
        }
        void remove(const T& val) {
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
            Node* ptr;
            Iterator(Node* current) : ptr(current) {}
        public:
            Iterator() : ptr(nullptr) {}

            bool operator==(const Iterator& other) const {
                return ptr == other.ptr;
            }
            bool operator!=(const Iterator& other) const {
                return ptr != other.ptr;
            }
            T& operator*() const {
                return ptr->data;
            }
            const T* operator->() {
                return &(ptr->data);
            }
        };




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
    };

    template<typename T>
    class set {
    private:
        RedBlackTree<T> rbt;
    public:
        set() {}
        ~set() {}

        void insert(const T& val) {
            rbt.insert(val);
        }
        void erase(const T& val) {
            rbt.remove(val);
        }
        bool contains(const T& val) {
            return (rbt.search(val)) ? true : false;
        }

        const size_t size() const {
            return rbt.size();
        }

        bool empty() {
            return rbt.empty();
        }
    };
}
int main()
{
    std::cout << "Welcome to my_std_set! There is a set<T> on red-black tree: " << std::endl;

    my_std::set<int> my_set = {};
    
    return 0;
}