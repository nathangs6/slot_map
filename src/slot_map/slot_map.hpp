#ifndef SLOT_MAP_H
#define SLOT_MAP_H

#include <iostream>
#include <cstddef>
#include <stdexcept>

class key {
    public:
        size_t ind;
        size_t gen;
        key();
        key(size_t ind, size_t gen);
        key& operator*() {
            return *this;
        }
        void update_gen();
        void set_ind(size_t ind);
};
key::key() : ind(-1), gen(0) {}
key::key(size_t ind, size_t gen) : ind(ind), gen(gen) {}
void key::update_gen() {
    gen++;
}
void key::set_ind(size_t new_index) {
    ind = new_index;
}

template <typename slot_map>
class slot_map_iterator {
    public:
        using value_type = size_t;
        using pointer_type = size_t*;
        using reference_type = size_t&;
        using T = key;
        using Tptr = key*;
        using Tref = key&;
    public:
        slot_map_iterator(size_t* ptr, slot_map* sm) : ptr(ptr), sm(sm) {};
        slot_map_iterator& operator++() {
            ptr++;
            return *this;
        }
        slot_map_iterator operator++(int) {
            slot_map_iterator iterator = *this;
            ++(*this);
            return iterator;
        }
        slot_map_iterator& operator--() {
            ptr--;
            return *this;
        }
        slot_map_iterator operator--(int) {
            slot_map_iterator iterator = *this;
            --(*this);
            return iterator;
        }
        Tref operator[](size_t index) {
            return sm->get_key(*(ptr + index));
        }
        Tptr operator->() {
            return &sm->get_key(*ptr);
        }
        Tref operator*() {
            return *(sm->get_key(*ptr));
        }
        bool operator==(const slot_map_iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const slot_map_iterator& other) const {
            return !(*this == other);
        }
        
    private:
        pointer_type ptr;
        slot_map* sm;
};



template <typename T>
class slot_map {
    public:
        using iterator = slot_map_iterator<slot_map<T>>;
    private:
        key* indices;
        T* data;
        size_t* erase;
        size_t head;
        size_t tail;
        size_t capacity;
        size_t size;
        void expand();
    public:
        slot_map() : indices(nullptr), data(nullptr), erase(nullptr), head(0), tail(0), capacity(0), size(0) {}
        ~slot_map() {
            delete[] indices;
            delete[] data;
            delete[] erase;
        }
        // getter methods for testing
        size_t get_capacity() const {
            return capacity;
        }
        size_t get_size() const {
            return size;
        }
        key* get_indices() const {
            return indices;
        }
        T* get_data() const {
            return data;
        }
        size_t* get_erase() const {
            return erase;
        }
        size_t get_head() const {
            return head;
        }
        size_t get_tail() const {
            return tail;
        }
        // Iterator functions
        iterator begin() {
            return iterator(erase, this);
        }
        iterator end() {
            return iterator(erase + size, this);
        }
        // Usage operations
        key insert(T val); // insert pops free head of indices
        void remove(const key& val); // remove adds to tail of indices
        T operator[](const key& val);
        key get_key(const size_t val);
};

template <typename T>
void slot_map<T>::expand() {
    size_t new_capacity;
    key* new_indices;
    if (capacity == 0) {
        new_capacity = 1;
        new_indices = new key[new_capacity];
        head = 0;
        tail = 0;
    } else {
        new_capacity = 2 * capacity;
        new_indices = new key[new_capacity];
        head = size;
        tail = new_capacity-1;
    }
    T* new_data = new T[new_capacity];
    size_t* new_erase = new size_t[new_capacity];
    for (size_t i = 0; i < size; i++) {
        new_indices[i] = indices[i];
        new_data[i] = data[i];
        new_erase[i] = erase[i];
    }
    new_indices[size].set_ind(size);
    for (size_t i = size; i < new_capacity-1; i++) {
        new_indices[i].set_ind(i+1);
    }
    new_indices[new_capacity-1].set_ind(new_capacity-1);
    delete[] indices;
    delete[] data;
    delete[] erase;
        
    indices = new_indices;
    data = new_data;
    erase = new_erase;
    capacity = new_capacity;
}

template <typename T>
key slot_map<T>::insert(T val) {
    if (size == capacity) {
        slot_map<T>::expand();
    }
    data[size] = val;
    erase[size] = head;
    // construct key to be handed to user
    key* head_key = &indices[head];
    key ret_key = key(head, head_key->gen);
    // update head and old head key
    size_t new_head = head_key->ind;
    head_key->ind = size;
    head = new_head;
    size++;
    return ret_key;
}

template <typename T>
void slot_map<T>::remove(const key& val) {
    // val.ind contains indices index
    // with index = indices[val.ind], index.ind contains data index
    if (val.gen != indices[val.ind].gen) { // check generations are the same
        throw std::invalid_argument("Invalid generation value.");
    }
    // get indices entry for val
    key* index = &indices[val.ind];
    if (size >= 2) {
        // delete object from data and erase by shifting end of array back
        // Note: we will require the user to handle deletion of T if it's needed
        data[index->ind] = data[size-1];
        erase[index->ind] = erase[size-1];
        indices[erase[size-1]].ind = index->ind;
    } else {
        erase[index->ind] = -1;
    }
    // cleanup end of array
    erase[size-1] = -1;
    // update index to refer to itself
    index->ind = val.ind;
    index->update_gen(); // to ensure current val can't refer to this again
    // update tail and size
    key* tail_key = &indices[tail];
    tail_key->ind = index->ind;
    tail = index->ind;
    size--;
}

template <typename T>
T slot_map<T>::operator[](const key& val) {
    key index = indices[val.ind];
    if (index.gen != val.gen) {
        throw std::invalid_argument("Invalid generation value.");
    }
    return data[index.ind];
}
template <typename T>
key slot_map<T>::get_key(const size_t val) {
    if (val >= capacity) {
        throw std::invalid_argument("Invalid index.");
    }
    return indices[val];
}

#endif
