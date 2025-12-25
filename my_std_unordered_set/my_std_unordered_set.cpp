#include <iostream>
#include <utility>
#include <functional>

namespace my_std {
    template<typename T>
    class Hashmap {
    private:

        struct Bucket {
            T data;
            Bucket* next;
            size_t hash_code;
        };
        Bucket** buckets;
        size_t hm_size;
        size_t hm_capacity;

        friend void swap(Hashmap& first, Hashmap& second) {
            using std::swap;
            swap(first.buckets, second.buckets);
            swap(first.hm_size, second.hm_size);
            swap(first.hm_capacity, second.hm_capacity);
        }


        size_t hash_function(const T& bucket_key) const {
            return std::hash<std::remove_const_t<T>>{}(bucket_key) % hm_capacity;
        }



        void rehashing() {
            Bucket** old_buckets = buckets;
            size_t old_capacity = hm_capacity;

            hm_capacity *= 1.5;
            buckets = new Bucket * [hm_capacity]();

            for (size_t i = 0; i < old_capacity; i++) {
                Bucket* current = old_buckets[i];

                while (current != nullptr) {
                    Bucket* next_node = current->next;
                    current->next = nullptr;

                    size_t new_index = hash_function(current->data);
                    current->hash_code = new_index;

                    if (buckets[new_index] == nullptr) {
                        buckets[new_index] = current;
                    }
                    else {
                        Bucket* tail = buckets[new_index];
                        while (tail->next != nullptr) {
                            tail = tail->next;
                        }
                        tail->next = current;
                    }
                    current = next_node;
                }
            }
            delete[] old_buckets;
        }

        Bucket* check_duplication(const T& val) {
            Bucket* current = buckets[hash_function(val)];
            while (current != nullptr) {
                if (current->data == val) {
                    return current;
                }
                current = current->next;
            }
            return nullptr;
        }
    public:
        Hashmap() : hm_capacity(3), hm_size(0) {
            buckets = new Bucket*[hm_capacity]();
        }
        Hashmap(std::initializer_list<T> hm) : hm_capacity(3), hm_size(0) {
            buckets = new Bucket * [hm_capacity]();
            for (const auto& item : hm) {
                insert(item);
            }
        }
        Hashmap(const Hashmap& other) : hm_capacity(other.hm_capacity), hm_size(0) {
            this->buckets = new Bucket*[this->hm_capacity]();
            for (size_t i = 0; i < other.hm_capacity; i++) {
                Bucket* current = other.buckets[i];
                while (current != nullptr) {
                    this->insert(current->data);
                    current = current->next;
                }
            }
        }
        Hashmap(Hashmap&& other) noexcept {
            this->buckets = other.buckets;
            this->hm_size = other.hm_size;
            this->hm_capacity = other.hm_capacity;
            other.buckets = nullptr;
            other.hm_size = 0;
            other.hm_capacity = 0;
        }

        Hashmap& operator=(Hashmap other) {
            using std::swap;
            swap(*this, other);
            return *this;
        }

        ~Hashmap() {
            clear();
        }

        void insert(const T& val) {
            if (hm_capacity <= hm_size) {
                rehashing();
            }

            Bucket* duplicate = check_duplication(val);
            if (duplicate != nullptr) {
                 return nullptr;
            }
            else {
                size_t i = hash_function(val);
                Bucket* newBucket = new Bucket{ val, buckets[i], i };
                buckets[i] = newBucket;
                hm_size++;
            }
        }
        void remove(const T& val) {
            size_t index = hash_function(val);
            Bucket* current = buckets[index];
            if (current == nullptr) {
                return;
            }

            if (current->data == val) {
                buckets[index] = current->next;
                delete current;
                hm_size--;
            }
            else {
                while (current->next != nullptr) {
                    if (current->next->data == val) {
                        Bucket* oldBucket = current->next;
                        current->next = oldBucket->next;
                        delete oldBucket;
                        hm_size--;
                        break;
                    }
                    current = current->next;
                }
            }
        }



        class Iterator {
        private:
            friend class Hashmap;
            Bucket* ptr;
            Bucket** buckets_ptr;
            size_t bucket_index;
            size_t capacity;
            Iterator(Bucket* node, Bucket** b_ptr, size_t idx, size_t cap)
                : ptr(node), buckets_ptr(b_ptr), bucket_index(idx), capacity(cap) {
            }

        public:
            Iterator() : ptr(nullptr), buckets_ptr(nullptr), bucket_index(0), capacity(0) {}

            Iterator& operator++() {
                if (ptr != nullptr) {
                    ptr = ptr->next;
                }
                while (ptr == nullptr && ++bucket_index < capacity) {
                    ptr = buckets_ptr[bucket_index];
                }

                return *this;
            }

            bool operator==(const Iterator& other) const {
                return ptr == other.ptr;
            }

            bool operator!=(const Iterator& other) const {
                return ptr != other.ptr;
            }

            T& operator*() const {
                return ptr->data;
            }

            T* operator->() const {
                return &(ptr->data);
            }
        };



        Iterator search(const T& val) {
            size_t index = hash_function(val);
            Bucket* current = buckets[index];

            while (current != nullptr) {
                if (current->data == val) {
                    return Iterator(current, buckets, index, hm_capacity);
                }
                current = current->next;
            }
            return end();
        }
        bool contains(const T& val) const {
            Bucket* current = buckets[hash_function(val)];
            while (current != nullptr) {
                if (current->data == val) {
                    return true;
                }
                current = current->next;
            }
            return false;
        }
        Iterator begin() {
            for (size_t i = 0; i < hm_capacity; i++) {
                if (buckets[i] != nullptr) {
                    return Iterator(buckets[i], buckets, i, hm_capacity);
                }
            }
            return end();
        }

        Iterator end() {
            return Iterator(nullptr, buckets, hm_capacity, hm_capacity);
        }

        const size_t size() const {
            return hm_size;
        }

        void clear() {
            if (buckets == nullptr) return;

            for (size_t i = 0; i < hm_capacity; i++) {
                Bucket* current = buckets[i];
                while (current != nullptr) {
                    Bucket* temp = current;
                    current = current->next;
                    delete temp;
                }
                buckets[i] = nullptr;
            }
            hm_size = 0;
        }

        bool empty() {
            return !buckets;
        }

        static void ruleoffive_hashmap_demo() {
            my_std::Hashmap<std::string> hm_example = { "Google", "Apple", "Nvidia", "AMD"};
            std::cout << "" << std::endl;
            my_std::Hashmap<std::string> hm_example_c_copy(hm_example);
            std::cout << "" << std::endl;
            my_std::Hashmap<std::string> hm_example_c_move = std::move(hm_example_c_copy);
            std::cout << "" << std::endl;
            my_std::Hashmap<std::string> hm_example_op_cas;
            std::cout << "" << std::endl;
            hm_example_op_cas = hm_example_c_move;
            std::cout << "" << std::endl;
            std::cout << "" << std::endl;
        }
    };



    template<typename T>
    class unordered_set {
    private:
        using Iterator = typename Hashmap<T>::Iterator;
        Hashmap<T> hm;
    public:
        unordered_set(std::initializer_list<T> hm) {
            for (const auto& item : hm) {
                insert(item);
            }
        }

        void insert(const T& val) {
            hm.insert(val);
        }
        void erase(const T& val) {
            hm.remove(val);
        }

        T& operator[](const T& val) {
            if (contains(val)) {
                return (*find(val));
            }
            else {
                insert({ val, T() });
                return (*find(val));
            }
        }
        Iterator find(const T& val) {
            return hm.search(val);
        }

        bool contains(const T& val) const {
            return hm.contains(val);
        }
        Iterator begin() {
            return hm.begin();
        }
        Iterator end() {
            return hm.end();
        }

        const size_t size() const {
            return hm.size();
        }

        void clear() {
            hm.clear();
        }

        bool empty() {
            return hm.empty();
        }




        static void ruleoffive_unordered_map_demo() {
            Hashmap<std::string>::ruleoffive_hashmap_demo();
        }
    };
};


int main()
{
    std::cout << "Welcome to my_std_unordered_map! There is an example of unordered_map<const Key, Value> on hashmap with chaining." << std::endl;

    my_std::unordered_set<std::string>::ruleoffive_unordered_map_demo();

    my_std::unordered_set<std::string> my_unordered_set = { "Google", "Apple", "Nvidia", "AMD" };

    std::cout << "unordered_map.insert('GitHub');" << std::endl;
    my_unordered_set.insert("GitHub");
    std::cout << "unordered_map.insert('Google');  (create a duplicate)" << std::endl;
    my_unordered_set.insert("Google");
    std::cout << "unordered_map.insert('elgooG');  (create a collision)" << std::endl;
    my_unordered_set.insert("elgooG");
    return 0;
}