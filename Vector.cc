// Implementation of the templated Vector class
// ECE4122/6122 Lab 5
// Jonathan Jones

#include <iostream>
#include "Vector.h"

using namespace std;


// Default constructor
template <typename T>
Vector<T>::Vector() :
    count(0),
    reserved(0),
    elements(NULL) {}


// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& v) :
    count(v.Size()),
    reserved(0),
    elements(NULL)
{
    *this = v;
}


// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& v)
{
    // allocate enough space for the new vector
    Reserve(v.Size());

    // copy over the elements by it's own copy constructor
    for (size_t i = 0; i < count; ++i)
        new (&elements[i]) T(v[i]);

    return *this;
}


template <typename T>
void Vector<T>::Reserve(size_t n)
{
    if (n > reserved) {
        T* nelems = static_cast<T*>(realloc(elements, n * sizeof(T)));

        if (nelems != NULL) {
            elements = nelems;
            reserved = n;
        } else {
            free(elements);
            cerr << "Error reallocating memory!" << endl;
            exit(5);
        }
    }
}


#ifdef GRAD_STUDENT

// Other constructors
template <typename T>
Vector<T>::Vector(size_t n) :
    count(0),
    reserved(0),
    elements(NULL)
{
    // Initialize with reserved memory
    Reserve(n);
}


// Constructor that copies a given type n number of times
template <typename T>
Vector<T>::Vector(size_t n, const T& v) :
    count(n),
    reserved(0),
    elements(NULL)
{
    // Initialize with "n" copies of "t"
    Reserve(n);

    // call each's constructor & assign the memory location's value
    for (size_t i = 0; i < count; ++i)
        new (&elements[i]) T(v);
}

#endif


// Destructor
template <typename T>
Vector<T>::~Vector(void)
{
    for (size_t i = 0; i < count; ++i)
        (&elements[i])->~T();

    free(elements);
}


// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& e)
{
    // allocate more memory if needed
    Reserve(++count);
    // force call the copy constructor for the placed object
    new (&elements[count - 1]) T(e);
}


template <typename T>
void Vector<T>::Push_Front(const T& e)
{
    // allocate more memory if needed
    Reserve(++count);

    // move everything up by 1 position
    // starting at the end and working our way to the front
    for (size_t i = count - 1; i > 0; --i) {
        // construct the new object by using its copy constructor
        // for the new location
        new (&elements[i]) T(elements[i - 1]);
        // call the deconstructor for it's previous location
        (&elements[i - 1])->~T();
    }

    // force call the constructor for our added element
    new (&elements[0]) T(e);
}


template <typename T>
void Vector<T>::Pop_Back(void)
{
    // Remove last element by calling its deconstructor
    // and decrementing our count value
    (&elements[--count])->~T();
}


template <typename T>
void Vector<T>::Pop_Front(void)
{
    // move everything down by 1 position
    // starting at the front and working our way to the back
    for (size_t i = 0; i < count - 1; ++i) {
        // call the deconstructor for the current location
        (&elements[i])->~T();
        // construct the new object by using its copy constructor
        // using the next indexed location
        new (&elements[i]) T(elements[i + 1]);
    }

    // deconstruct the last object in the memory array
    // and decrement our count value
    (&elements[--count])->~T();
}


// Element Access
template <typename T>
T& Vector<T>::Front(void) const
{
    return elements[0];
}


// Element Access
template <typename T>
T& Vector<T>::Back(void) const
{
    return elements[count - 1];
}


// vector indexing
template <typename T>
const T& Vector<T>::operator[](size_t i) const
{
    return const_cast<T&>(elements[i]);
}


// vector indexing
template <typename T>
T& Vector<T>::operator[](size_t i)
{
    return elements[i];
}


// return's the number of elements that have been added
template <typename T>
size_t Vector<T>::Size(void) const
{
    return count;
}


// returns true if empty
template <typename T>
bool Vector<T>::Empty(void) const
{
    return count == 0;
}


// Implement clear
template <typename T>
void Vector<T>::Clear(void)
{
    for (size_t i = 0; i < count; ++i)
        (&elements[i])->~T();

    count = 0;
}


// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin(void) const
{
    // this points to the address after the last element
    return VectorIterator<T>(elements);
}


template <typename T>
VectorIterator<T> Vector<T>::End(void) const
{
    return VectorIterator<T>(&elements[count]);
}


#ifdef GRAD_STUDENT

// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
    VectorIterator<T> it_tmp(it);
    // decrement our element count
    count--;
    // move all of the elements down to fill in the gap
    while (it_tmp != End()) {
        T* elem = &*it_tmp;
        it_tmp++;
        (elem)->~T();
        new (elem) T(*it_tmp);
    }
}


template <typename T>
void Vector<T>::Insert(const T & v, const VectorIterator<T>& it)
{
    // get a temporary iterator that initially points to the end of the vector
    VectorIterator<T> it_tmp(it);

    // move everything up by 1 position
    // starting at the iterator and working our way to the back
    T prv(v);
    T nxt(*it_tmp);
    T* nxtP = &*it_tmp;
    while (it_tmp != End()) {
        nxt = *it_tmp;
        (nxtP)->~T();
        new (nxtP) T(prv);
        prv = nxt;
        // increment
        it_tmp++;
        // store next pointer
        nxtP = &*it_tmp;
    }
    // Make sure we have enough reserved space for the new element that was placed
    Reserve(++count);
    // Store the last element back making sure we use the elements member for finding out
    // the address here since the memory block could have changed with the previous call
    new (&elements[count - 1]) T(prv);
}

#endif


// Implement the iterators
// Constructors
template <typename T>
VectorIterator<T>::VectorIterator() {}


template <typename T>
VectorIterator<T>::VectorIterator(T * c) :
    current(c) {}


// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& it) :
    current(it.current) {}


// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
    return *current;
}


// Prefix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++()
{
    *current++;
    return *this;
}


// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
    VectorIterator<T> tmp = *this;
    ++*this;
    return tmp;
}


// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& it) const
{
    return current != it.current;
}


template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& it) const
{
    return current == it.current;
}
