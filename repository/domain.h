//
// Created by Grigore Sima on 06.04.2023.
//

#ifndef TA_DOMAIN_H
#define TA_DOMAIN_H

#include <iostream>
#include <cassert>

using namespace std;

class Offer {
private:
    string name;
    string destination;
    int type = -1;
    int price = -1;

    inline static bool test_mode = false;

    static void testCreate();

    static void testGetName();
    static void testGetDestination();
    static void testGetType();
    static void testGetPrice();

    static void testSetName();
    static void testSetDestination();
    static void testSetType();
    static void testSetPrice();

public:

    Offer();
    Offer(const string&, const string&, const int&, const int&);
    Offer(const Offer&);

    [[nodiscard]] string getName() const;
    [[nodiscard]] string getDestination() const;
    [[nodiscard]] int getType() const;
    [[nodiscard]] int getPrice() const;

    void setName(const string&);
    void setDestination(const string&);
    void setType(const int&);
    void setPrice(const int&);


    bool operator==(const Offer&) const;
    bool operator!=(const Offer&) const;
    //bool operator=(const Offer&);

    ~Offer() = default;

    static void testMode();
    static void tests();
};

class Destination { // OfferDTO
private:
    string destination;
    int count;

    static void testGetDestination();
    static void testGetCount();
    static void testIncreaseCount();
    static void testEqual();

public:
    Destination();
    explicit Destination(const string&);

    [[nodiscard]] string getDestination() const;
    [[nodiscard]] int getCount() const;

    void increaseCount();

    bool operator==(const Destination&) const;

    static void tests();

    ~Destination() = default;
};

#endif //TA_DOMAIN_H
