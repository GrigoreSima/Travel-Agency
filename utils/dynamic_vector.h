//
// Created by Grigore Sima on 26.04.2023.
//

#ifndef TA_DYNAMIC_VECTOR_H
#define TA_DYNAMIC_VECTOR_H

template<typename Element>
class DynamicVector {

private:
    Element* vector;
    int _size, _capacity;

    void redim();

public:

    DynamicVector();

    Element& operator[](const int&);

    void push_back(Element);
    void erase(Element*);
    [[nodiscard]] int size() const;
    Element* begin();
    Element* end();

    // destructorul DynamicVectori
    ~DynamicVector();

};

template<typename Element>
Element &DynamicVector<Element>::operator[](const int& position)
{
    return this->vector[position];
}

template<typename Element>
DynamicVector<Element>::DynamicVector(): _size(0), _capacity(5)
{
    this->vector = new Element[5];
}

template<typename Element>
int DynamicVector<Element>::size() const
{
    return this->_size;
}

template<typename Element>
void DynamicVector<Element>::redim()
{
    this->_capacity = 2 * this->_capacity;
    auto *new_vector = new Element[this->_capacity];

    for (int i = 0; i < this->size(); i++)
    {
        Element copied_element = this->vector[i];
        new_vector[i] = copied_element;
    }

    delete[] this->vector;
    this->vector = new_vector;
}

template<typename Element>
void DynamicVector<Element>::push_back(Element element)
{
    if (this->_size > 0 && this->_size == this->_capacity) this->redim();
    this->vector[this->_size] = element;
    this->_size++;
}

template<typename Element>
void DynamicVector<Element>::erase(Element* element)
{
    for(auto i = element; i < this->end()-1; i++)
        *i = *(i+1);
    this->_size--;
}

template<typename Element>
Element *DynamicVector<Element>::begin()
{
    return this->vector;
}

template<typename Element>
Element *DynamicVector<Element>::end()
{
    return this->vector + this->size();
}

template<typename Element>
DynamicVector<Element>::~DynamicVector()
{
    delete[] this->vector;
}

#endif //TA_DYNAMIC_VECTOR_H
