#include <iostream>
#include <utility>
#include <functional>

template<typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::pair<const Key, Value>& pair) {
    return os << pair.first << ": " << pair.second << ";" << std::endl;
}

namespace my_std {
    template<typename Key, typename Value>
    class Hashmap {
    private:
        using value_type = std::pair<const Key, Value>;

        struct Bucket {
            value_type data;
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


        size_t hash_function(const Key& bucket_key) const {
            return std::hash<std::remove_const_t<Key>>{}(bucket_key) % hm_capacity;
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

                    size_t new_index = hash_function(current->data.first);
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

        Bucket* check_dublication(const Key& arg_key) {
            Bucket* current = buckets[hash_function(arg_key)];
            while (current != nullptr) {
                if (current->data.first == arg_key) {
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
        Hashmap(std::initializer_list<value_type> hm) : hm_capacity(3), hm_size(0) {
            buckets = new Bucket * [hm_capacity]();
            for (const auto& item : hm) {
                insert(item);
            }
        }
        Hashmap(const Hashmap& other) : hm_capacity(other.hm_capacity), hm_size(0) {
            this->buckets = new Bucket * [this->hm_capacity]();
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

        void insert(const value_type& pair) {
            if (hm_capacity <= hm_size) {
                rehashing();
            }

            Bucket* dublicat = check_dublication(pair.first);
            if (dublicat != nullptr) {
                dublicat->data.second = pair.second;
            }
            else {
                size_t i = hash_function(pair.first);
                Bucket* newBucket = new Bucket{ pair, buckets[i], i };
                buckets[i] = newBucket;
                hm_size++;
            }
        }
        void remove(const Key& arg_key) {
            size_t index = hash_function(arg_key);
            Bucket* current = buckets[index];
            if (current == nullptr) {
                return;
            }

            if (current->data.first == arg_key) {
                buckets[index] = current->next;
                delete current;
                hm_size--;
            }
            else {
                while (current->next != nullptr) {
                    if (current->next->data.first == arg_key) {
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
            using value_type = std::pair<const Key, Value>;
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

            value_type& operator*() const {
                return ptr->data;
            }

            value_type* operator->() const {
                return &(ptr->data);
            }
        };



        Iterator search(const Key& key) {
            size_t index = hash_function(key);
            Bucket* current = buckets[index];

            while (current != nullptr) {
                if (current->data.first == key) {
                    return Iterator(current, buckets, index, hm_capacity);
                }
                current = current->next;
            }
            return end();
        }
        Value& operator[](const Key& key) {
            if (contains(key)) {
                return (*find(key)).second;
            }
            else {
                insert({ key, Value() });
                return (*find(key)).second;
            }
        }
        bool contains(const Key& key) const {
            Bucket* current = buckets[hash_function(key)];
            while (current != nullptr) {
                if (current->data.first == key) {
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
            my_std::Hashmap<std::string, int> hm_example = {
                {"current year", 2025},
                {"current month", 12},
                {"hours in day", 24},
                {"Google company formation", 1998}
            };
            std::cout << "" << std::endl;
            my_std::Hashmap<std::string, int> hm_example_c_copy(hm_example);
            std::cout << "" << std::endl;
            my_std::Hashmap<std::string, int> hm_example_c_move = std::move(hm_example_c_copy);
            std::cout << "" << std::endl;
            my_std::Hashmap<std::string, int> hm_example_op_cas;
            std::cout << "" << std::endl;
            hm_example_op_cas = hm_example_c_move;
            std::cout << "" << std::endl;
            std::cout << "" << std::endl;
        }
    };



    template<typename Key, typename Value>
    class unordered_map {
    private:
        using Iterator = typename Hashmap<const Key, Value>::Iterator;
        using value_type = std::pair<const Key, Value>;
        Hashmap<const Key, Value> hm;
    public:
        unordered_map(std::initializer_list<value_type> hm) {
            for (const auto& item : hm) {
                insert(item);
            }
        }

        void insert(const value_type& pair) {
            hm.insert(pair);
        }
        void erase(const Key& pair) {
            hm.remove(pair);
        }

        Value& operator[](const Key& key) {
            if (contains(key)) {
                return (*find(key)).second;
            }
            else {
                insert({ key, Value() });
                return (*find(key)).second;
            }
        }
        Iterator find(const Key& pair) {
            return hm.search(pair);
        }

        bool contains(const Key& pair) const {
            return hm.contains(pair);
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
            Hashmap<std::string, std::string>::ruleoffive_hashmap_demo();
        }
    };
};


int main()
{
    std::cout << "Welcome to my_std_unordered_map! There is an example of unordered_map<const Key, Value> on hashmap with chaining." << std::endl;

    my_std::unordered_map<std::string, std::string>::ruleoffive_unordered_map_demo();

    my_std::unordered_map<std::string, std::string> my_unordered_map = {
        {"Car", "BMW"},
        {"Word", "C++"},
        {"Rac", "Volkswagen"}
    };

    std::cout << "unordered_map.insert('Website', 'GitHub');" << std::endl;
    my_unordered_map.insert({ "Website", "GitHub" });
    std::cout << "unordered_map.insert('Website', 'Google');  (create a dublicat)" << std::endl;
    my_unordered_map.insert({ "Website", "Google" });
    std::cout << "unordered_map.insert('Cra', 'Toyota');  (create a collision)" << std::endl;
    my_unordered_map.insert({ "Cra", "Toyota" });

    std::cout << "unordered_map.find('Cra'); " << std::endl;
    std::cout << "Address of this key: " << (*my_unordered_map.find("Cra")) << ". " << std::endl;
    return 0;
}