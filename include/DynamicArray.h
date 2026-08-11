#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
#include <stdexcept>

using namespace std; 
template <typename T>
class DynamicArray {
    T* array;
    int currentSize;
    int capacity;

    
public:
    DynamicArray() : array(nullptr), currentSize(0), capacity(0) {}

    void push_back(const T& value) {
        if(currentSize == capacity) resize();
        array[currentSize++] = value;
    }

    void pop_back() {
        if(currentSize > 0) {
            currentSize--;
        }
    }

    ~DynamicArray() {
        delete[] array;
        array = nullptr;

    }

    void resize() {
        int newCapacity = (capacity == 0)? 1 : capacity * 2;
        T* temp = new T[newCapacity];
        for(int i = 0; i < currentSize; i++) 
            temp[i] = array[i];
        delete[] array;
        array = temp;
        capacity = newCapacity;
    }
    
    DynamicArray(const DynamicArray &other) {
        currentSize = other.currentSize;
        capacity = other.capacity;

        array = (capacity > 0) ? new T[capacity] : nullptr;
        for(int i = 0; i < currentSize; i++) {
            array[i] = other.array[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if(this == &other)
            return *this;
        
        delete[] array;
        currentSize = other.currentSize;
        capacity = other.capacity;

        array = new T[capacity];

        for(int i = 0 ; i < currentSize; i++) {
            array[i]= other.array[i];
        }

        return *this;
    }

    void clear() {
        delete[] array;
        array = nullptr;
        currentSize = 0;
        capacity = 0;
    }

    void erase(int index) {
        if(index < 0 || index >= currentSize) throw out_of_range("Index out of bounds");
        for(int i = index; i < currentSize - 1; i++) {
            array[i] = array[i + 1];
        }
        currentSize--;
    }

    void removeAt(int index) {
        if(index < 0 || index >= currentSize) throw out_of_range("Index out of bounds");
        for(int i = index; i < currentSize - 1; i++) {
            array[i] = array[i + 1];
        }
        currentSize--;
    }
    
    void insert(int pos, const T& value) {
        if(pos < 0 || pos > currentSize) return;

        if(currentSize >= capacity) resize();
        for(int i = currentSize; i > pos; i--) 
            array[i] = array[i-1];
        array[pos] = value;
        currentSize++;
    }

    
    T& operator[](int i) { if(!(i < 0 || i >= currentSize)) return array[i]; else throw out_of_range("Index out of bounds"); }
    const T& operator[](int i) const { if(!(i < 0 || i >= currentSize)) return array[i]; else throw out_of_range("Index out of bounds"); }

    void swap(DynamicArray<T>& other) {
        T* temp = array;
        array = other.array;
        other.array = temp;

        int tempSize = currentSize;
        currentSize = other.currentSize;
        other.currentSize = tempSize;

        int tempCapacity = capacity;
        capacity = other.capacity;
        other.capacity = tempCapacity;
    }

    T* data() { return array; }
const T* data() const { return array; }

    void shrink_to_fit() {
        if(currentSize == capacity) return;
        T* temp = new T[currentSize];
        for(int i = 0; i < currentSize; i++) 
            temp[i] = array[i];
        delete[] array;
        array = temp;
        capacity = currentSize;
    }

    int size() const { return currentSize; }
    bool empty() const { return currentSize == 0; }


};

#endif