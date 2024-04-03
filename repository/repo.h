//
// Created by Grigore Sima on 06.04.2023.
//

#ifndef TA_REPO_H
#define TA_REPO_H

#include <vector>
#include <random>
#include <fstream>
#include <unordered_map>

#include "./utils/validator.h"
#include "domain.h"

/*
One Dubai 1 2591
Two Bucharest 3 3000
Three Paris 3 5000
Six Dubai 1 5000
Five Bucharest 2 3000
Four Bucharest 2 9182
Seven Dublin 3 2500

*/

class RepositoryAbstract
{
public:
    //float _probability; // TODO: probab private
    virtual void addOffer(const Offer&) = 0;
    virtual void addOffer(const string&, const string&, const int&, const int&) = 0;
    virtual void removeOffer(const int&) = 0;
    virtual void modifyOfferStr(const int&, const string&, const int&) = 0;
    virtual void modifyOfferInt(const int&, const int&, const int&) = 0;

    virtual unsigned long getSize() = 0;
    virtual vector<Offer>& getAgency() = 0;
    virtual Offer& getOffer(int) = 0;

    virtual void refresh() = 0;
};

class RepositoryLab : public RepositoryAbstract{
private:
    unordered_map<string, Offer> agency;
    vector<Offer> agency_vector;
    inline static bool test_mode = false;
    float _probability;

    static void testConstructor();
    static void testAddOffer();
    static void testRemoveOffer();
    static void testModifyOfferStr();
    static void testModifyOfferInt();

    static void testGetSize();

public:

    explicit RepositoryLab(float);
    RepositoryLab(RepositoryLab&);

    // offers
    void addOffer(const Offer&) override;
    void addOffer(const string&, const string&, const int&, const int&) override;
    void removeOffer(const int&) override;
    void modifyOfferStr(const int&, const string&, const int&) override;
    void modifyOfferInt(const int&, const int&, const int&) override;

    void error() const;

    void refresh() override {};

    // Getters
    unsigned long getSize() override;
    vector<Offer>& getAgency() override;
    Offer& getOffer(int) override;

    void clear();
    static void testMode();
    static void tests();

    //~RepositoryLab();
};

class Repository : public RepositoryAbstract{
private:
    vector<Offer> agency;

    inline static bool test_mode = false;

    static void testAddOffer();
    static void testRemoveOffer();
    static void testModifyOfferStr();
    static void testModifyOfferInt();

    static void testGetSize();

public:

    Repository() = default;
    explicit Repository(RepositoryAbstract&);

    // offers
    void addOffer(const Offer&) override;
    void addOffer(const string&, const string&, const int&, const int&) override;
    void removeOffer(const int&) override;
    void modifyOfferStr(const int&, const string&, const int&) override;
    void modifyOfferInt(const int&, const int&, const int&) override;

    void refresh() override {};

    // Getters
    unsigned long getSize() override;
    vector<Offer>& getAgency() override;
    Offer& getOffer(int) override;

    void clear();
    static void testMode();
    static void tests();

    //~Repository();
};

class FileRepository : public Repository
{
private:
    string _filename;

    static void testCreate();

    static void testAddOffer();
    static void testRemoveOffer();
    static void testModifyOfferStr();
    static void testModifyOfferInt();

public:
    explicit FileRepository(const string&);

    void addOffer(const Offer&) override;
    void addOffer(const string&, const string&, const int&, const int&) override;
    void removeOffer(const int&) override;
    void modifyOfferStr(const int&, const string&, const int&) override;
    void modifyOfferInt(const int&, const int&, const int&) override;

    void saveToFile();
    void loadFromFile();
    void refresh() override
    {
        this->saveToFile();
        this->loadFromFile();
    }

    static void tests();
};

class Observer
{
public:
    virtual void update() = 0;
};

class Observable
{
private:
    vector<Observer*> list;
public:

    void addObserver(Observer* observer)
    {
        list.push_back(observer);
    };

    void removeObserver(Observer* observer)
    {
        list.erase(remove(begin(list),
                       end(list), observer),
                       list.end());
    };

protected:
    void notify()
    {
        for(auto it : list)
            it->update();
    };
};

class Wishlist : public Observable
{
private:
    vector<string> wishlist;
    RepositoryAbstract& _repository;

    static void testClearWishlist();
    static void testAddToWishlist();
    static void testGenerateWishlist();
    static void testGetSize();

public:
    explicit Wishlist(RepositoryAbstract&);

    void clearWishlist();
    void addToWishlist(const int&);
    void generateWishlist(const int&);

    void exportToCSV(const string&);
    void exportToHTLM(const string&);

    // Getters
    unsigned long getSize();
    vector<string>& getWishlist();

    static void tests();
};


#endif //TA_REPO_H
