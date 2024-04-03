//
// Created by Grigore Sima on 06.04.2023.
//

#include "domain.h"

//Constructors
Offer::Offer()
{
    this->name = "";
    this->destination = "";
    this->type = -1;
    this->price = -1;
}

Offer::Offer(const Offer &offer)
{
    this->setName(offer.getName());
    this->setDestination(offer.getDestination());
    this->setType(offer.getType());
    this->setPrice(offer.getPrice());

    //if(!Offer::test_mode) printf("Copied!\n");
}


Offer::Offer(const string& name, const string& destination,
             const int& type, const int& price)
{
    this->name = name;
    this->destination = destination;
    this->type = type;
    this->price = price;
}

// Getters

/*
 *  Gets the name of an offer
 *
 *  return: string, name of the offer
 */
string Offer::getName() const
{
    return this->name;
}

/*
 *  Gets the destinatin of an offer
 *
 *  return: string, destinatin of the offer
 */
string Offer::getDestination() const
{
    return this->destination;
}

/*
 *  Gets the type of an offer
 *
 *  return: int, type of the offer
 */
int Offer::getType() const
{
    return this->type;
}

/*
 *  Gets the price of an offer
 *
 *  return: int, price of the offer
 */
int Offer::getPrice() const
{
    return this->price;
}

// Setters

/*
 *  Sets the name of an offer
 *
 *  str: string, new name of the offer
 */
void Offer::setName(const string& str)
{
    this->name = str;
}

/*
 *  Sets the destinatin of an offer
 *
 *  str: string, destinatin of the offer
 */
void Offer::setDestination(const string& str)
{
    this->destination = str;
}

/*
 *  Sets the type of an offer
 *
 *  integer: int, type of the offer
 */
void Offer::setType(const int& integer)
{
    this->type = integer;
}

/*
 *  Sets the price of an offer
 *
 *  integer: int, price of the offer
 */
void Offer::setPrice(const int& integer)
{
    this->price = integer;
}


/* /Destructor
Offer::~Offer()
{
    this->name = "";
    this->destination = "";
    this->type = -1;
    this->price = -1;
}
 */

/*
 *  Compares to offer objects (operator== override)
 *
 *  return: bool, true if offers are the same (all fields are equal), false otherwise
 */
bool Offer::operator==(const Offer& other) const
{
    if(this->getName() != other.getName()) return false;
    if(this->getDestination() != other.getDestination()) return false;
    if(this->getType() != other.getType()) return false;
    if(this->getPrice() != other.getPrice()) return false;

    return true;
}

bool Offer::operator!=(const Offer &offer) const
{
    return !(*this == offer);
}

// tests

void Offer::testMode()
{
    Offer::test_mode = !test_mode;
}

void Offer::tests()
{
    testCreate();

    testGetName();
    testGetDestination();
    testGetType();
    testGetPrice();

    testSetName();
    testSetDestination();
    testSetType();
    testSetPrice();

}

void Offer::testCreate()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);

    assert(offer.name == "One");
    assert(offer.destination == "Dubai");
    assert(offer.type == 1);
    assert(offer.price == 2591);
}


void Offer::testGetName()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    assert(offer.getName() == "One");
}

void Offer::testGetDestination()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    assert(offer.getDestination() == "Dubai");
}

void Offer::testGetType()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    assert(offer.getType() == 1);
}

void Offer::testGetPrice()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    assert(offer.getPrice() == 2591);
}


void Offer::testSetName()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    offer.setName("Two");
    assert(offer.getName() == "Two");
}

void Offer::testSetDestination()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    offer.setDestination("Bucharest");
    assert(offer.getDestination() == "Bucharest");
}

void Offer::testSetType()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    offer.setType(2);
    assert(offer.getType() == 2);
}

void Offer::testSetPrice()
{
    Offer offer = Offer("One", "Dubai", 1, 2591);
    offer.setPrice(3000);
    assert(offer.getPrice() == 3000);
}

// DTO

Destination::Destination()
{
    this->destination = "";
    this->count = 0;
}

Destination::Destination(const string &destination)
{
    this->destination = destination;
    this->count = 1;
}

/*
 *  Gets the destination of the object
 *
 *  str: string, new name of the offer
 */
string Destination::getDestination() const
{
    return this->destination;
}

/*
 *  Gets the count of the object
 *
 *  str: string, new name of the offer
 */
int Destination::getCount() const
{
    return this->count;
}

/*
 *  Compares two destination objects
 *
 *  dest: Destination reference, destination to be compared to
 */
bool Destination::operator==(const Destination &dest) const
{
    return this->destination ==  dest.getDestination();
}

/*
 *  Increase the count for a destination
 */
void Destination::increaseCount()
{
    this->count++;
}

// tests

void Destination::tests()
{
    testGetDestination();
    testGetCount();
    testIncreaseCount();
    testEqual();
}

void Destination::testGetDestination()
{
    Destination destination("Bucharest");
    assert(destination.getDestination() == "Bucharest");

    Destination destination2("Dubai");
    assert(destination2.getDestination() == "Dubai");
}

void Destination::testGetCount()
{
    Destination null_destination;
    assert(null_destination.getCount() == 0);

    Destination destination("Bucharest");
    assert(destination.getCount() == 1);

    destination.increaseCount();
    destination.increaseCount();
    destination.increaseCount();

    assert(destination.getCount() == 4);
}

void Destination::testIncreaseCount()
{
    Destination destination("Bucharest");
    assert(destination.getCount() == 1);

    destination.increaseCount();
    assert(destination.getCount() == 2);

    destination.increaseCount();
    assert(destination.getCount() == 3);

    destination.increaseCount();
    assert(destination.getCount() == 4);
}

void Destination::testEqual()
{
    Destination destination("Bucharest");
    Destination destination2("Bucharest");
    assert(destination == destination2);

    Destination destination3("Dubai");
    bool result = (destination == destination3);
    assert(!result);
}

