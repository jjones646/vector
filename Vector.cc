// Implementation of the templated Vector class
// ECE4122/6122 Lab 5
// Jonathan Jones

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
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
    reserved(0),    // make sure we initialize reserved here
    elements(NULL)
{
    // allocate the correct amount of memory
    Reserve(count);

    // copy over the elements by it's own copy constructor
    for (size_t i = 0; i < count; ++i)
        new(&elements[i]) T(v[i]);

    clog << "Copy constructor called for size of " << count << endl;
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& v)
{
    clog << "ASSIGNMENT OPERATOR CALLED!!" << endl;
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
    reserved(0),     // make sure we initialize reserved here
    elements(NULL)
{
    // Initialize with reserved memory
    Reserve(n);
}

// Constructor that copies a given type n number of times
template <typename T>
Vector<T>::Vector(size_t n, const T& v) :
    count(n),
    reserved(0),     // make sure we initialize reserved here
    elements(NULL)
{
    // Initialize with "n" copies of "t"
    Reserve(n);

    // call each's constructor & assign the memory location's value
    for (size_t i = 0; i < count; ++i)
        new(&elements[i]) T(v);

    // debugging
    clog << "Init constructor called for " << n << " elements!" << endl;
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector(void)
{
    for (size_t i = 0; i < count; ++i)
        (&elements[i])->~T();

    free(elements);

    // debugging
    clog << "Deconstructor called for " << count << " elements!" << endl;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& e)
{
    // allocate more memory if needed
    Reserve(++count);
    // force call the copy constructor for the placed object
    new(&elements[count - 1]) T(e);

    // debugging
    clog << "Push_Back\tcount=" << count << endl;
}

template <typename T>
void Vector<T>::Push_Front(const T& e)
{
    // allocate more memory if needed
    Reserve(++count);

    // move everything up by 1 position
    // starting at the end and working our way to the front here
    for (size_t i = count - 1; i > 0; --i) {
        // construct the new object by using its copy constructor
        // for the new location
        new(&elements[i]) T(elements[i - 1]);
        // call the deconstructor for it's previous location
        (&elements[i - 1])->~T();
    }

    // force call the constructor for moving everything down by 1
    new(&elements[0]) T(e);

    // debugging
    clog << "Push_Front\tcount=" << count << endl;
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
    // starting at the front and working our way to the back here
    for (size_t i = 0; i < count - 1; ++i) {
        // call the deconstructor for the current location
        (&elements[i])->~T();
        // construct the new object by using its copy constructor
        // using the next indexed location
        new(&elements[i]) T(elements[i + 1]);
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

}

template <typename T>
void Vector<T>::Insert(const T& v, const VectorIterator<T>& it)
{

}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{}

template <typename T>
VectorIterator<T>::VectorIterator(T* c) :
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
VectorIterator<T>  VectorIterator<T>::operator++()
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
