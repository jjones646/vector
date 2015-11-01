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

    // copy over the elements
    for (size_t i = 0; i < count; ++i)
        elements[i] = v[i];

    cout << "Copy constructor called for size of " << count << endl;
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& v)
{
    cout << "ASSIGNMENT OPERATOR CALLED!!" << endl;
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{
    if (n > reserved) {
        T* nelems = (T*)realloc(elements, n * sizeof(T));

        if (nelems != NULL) {
            cout << "Reserving " << n << " spaces." << endl;
            elements = nelems;
            reserved = n;
        } else {
            free(elements);
            reserved = 0;
            count = 0;
            cerr << "Error reallocating memory!" << endl;
            exit(10);
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
Vector<T>::Vector(size_t n, const T& t) :
    count(n),
    reserved(0),     // make sure we initialize reserved here
    elements(NULL)
{
    // Initialize with "n" copies of "t"
    Reserve(n);

    for (size_t i = 0; i < count; ++i)
        elements[i] = t;
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector(void)
{
    count = 0;
    reserved = 0;
    free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& e)
{
    // allocate more memory
    Reserve(++count);

    // set the value in memory
    elements[count - 1] = e;

    for (size_t i = 0; i < count; ++i)
        cout << elements[i] << endl;

    cout << endl;
}

template <typename T>
void Vector<T>::Push_Front(const T& e)
{
    count++;

    if (count > reserved) {
        // allocate more memory
        Reserve(count);
    }

    // move everything up by 1 position
    for (size_t i = 0; i < count; ++i)
        elements[i + 1] = elements[i];

    // set the front value in memory
    elements[0] = e;
}

template <typename T>
void Vector<T>::Pop_Back(void)
{
    // Remove last element by simply changing our max index value
    count--;
}

template <typename T>
void Vector<T>::Pop_Front(void)
{
    // Remove first element by decrementing the max index value
    count--;

    // then moving everything down by 1
    for (size_t i = 0; i < count; ++i)
        elements[i] = elements[i + 1];
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
    return VectorIterator<T>(&elements[count - 1]);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
}
