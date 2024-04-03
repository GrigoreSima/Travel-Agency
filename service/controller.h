//
// Created by Grigore Sima on 06.04.2023.
//

#ifndef TA_CONTROLLER_H
#define TA_CONTROLLER_H

#include <algorithm>
#include <unordered_map>

#include "./repository/repo.h"
#include "./utils/undo.h"

class Controller {
private:
    RepositoryAbstract& _repository;
    Wishlist& _wishlist;

    vector<Undo*> _undo_list;

    inline static bool test_mode = false;

    static void testAdd();
    static void testRemove();
    static void testModify();
    static void testSearch();
    static void testGetOfferIndex();
    static void testFilter();
    static void testSort();
    static void testRaport();
    static void testUndo();

    static void testClearWishlist();
    static void testAddToWishlist();
    static void testGenerateWishlist();

public:
    //Controller(Controller&);
    explicit Controller(RepositoryAbstract&, Wishlist&);

    //offers
    void add(const string&, const string&, const string&, const string&);
    void remove(const string&);
    void modify(const string&, const string&, const string&);
    void search(Offer&, const string&);
    int getOfferIndex(const string&);
    void filter(RepositoryAbstract&, const string&, const string&);

    void sorted(RepositoryAbstract&, const string&, const string&);
    static bool greaterName(Offer&, Offer&);
    static bool lowerName(Offer&, Offer&);
    static bool greaterDestination(Offer&, Offer&);
    static bool lowerDestination(Offer&, Offer&);
    static bool greaterTypePrice(Offer&, Offer&);
    static bool lowerTypePrice(Offer&, Offer&);

    int raport(const string&);
    void undo();

    // wishlist
    void clearWishlist();
    void addToWishlist(const string&);
    void generateWishlist(const string&);
    void exportWishlist();

    //Getters
    RepositoryAbstract& getRepository();
    Wishlist& getWishlist();

    static void testMode();
    static void tests();

    ~Controller();
};


#endif //TA_CONTROLLER_H
