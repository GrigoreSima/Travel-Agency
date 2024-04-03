//
// Created by Grigore Sima on 06.04.2023.
//

#include "controller.h"

Controller::Controller(RepositoryAbstract& repository, Wishlist& wishlist) :
        _repository(repository), _wishlist(wishlist){}

/*
 * Adds an offer to the repository
 * name: string reference, name of the offer
 * destination: string reference, destination of the offer
 * type: string reference (of an int), type of the offer
 * price: string reference (of an int), price of the offer
 */
void Controller::add(const string& name, const string& destination,
                     const string& type, const string& price)
{
    Validate::validateDestination(destination, "Add");
    Validate::validateType(type, "Add");
    Validate::validatePrice(price, "Add");

    Offer offer;
    search(offer, name);
    if (offer.getName() == name ||
    ( offer.getDestination() == destination && offer.getType() == stoi(type) && offer.getPrice() == stoi(price)))
        throw Exception("is already in the list", "Add", "This offer");

    Offer added_offer = Offer(name, destination, stoi(type), stoi(price));
    _repository.addOffer(added_offer);
    _undo_list.push_back(new UndoAdd(this->getRepository(), added_offer));
}

/*
 * Removes an offer from the repository
 * position: int reference, position to remove
 */
void Controller::remove(const string& position)
{
    Validate::validatePosition(position, "Remove", (int)_repository.getSize());

    Offer offer = this->getRepository().getOffer(stoi(position));
    _repository.removeOffer(stoi(position));
    _undo_list.push_back(new UndoRemove(this->getRepository(), offer));
}

/*
 * Modifies an offer of the repository
 * field: string reference, field to modify
 * updated_value: string reference, new value of the field
 * position: int reference, position to modify
 */
void Controller::modify(const string& field, const string& updated_value, const string& position)
{
    int integer_field = -1;
    string temporary_field = field;

    Validate::strToLower(temporary_field);
    Validate::noWhitespaces(temporary_field);

    if(temporary_field == "name") integer_field = 1;
    if(temporary_field == "destination") integer_field = 2;
    if(temporary_field == "type") integer_field = 3;
    if(temporary_field == "price") integer_field = 4;

    Validate::validateModify(integer_field, updated_value, position, (int)_repository.getSize());

    _undo_list.push_back(
            new UndoModify(this->getRepository(),
           this->getRepository().getOffer(stoi(position))));

    if(integer_field <= 2)
        _repository.modifyOfferStr(integer_field, updated_value, stoi(position));
    else
    {
        int value = stoi(updated_value);
        _repository.modifyOfferInt(integer_field, value, stoi(position));
    }
}

/*
 * Searches for an offer in the repository
 * offer: Offer reference, the resulted offer
 * name: string reference, name of the offer
 */
void Controller::search(Offer &offer, const string &name)
{
    auto it = find_if(_repository.getAgency().begin(),
                      _repository.getAgency().end(),
                      [&name](auto& x){
                return  x.getName() == name; });

    if(it == this->_repository.getAgency().end()) return;

    offer.setName(it->getName());
    offer.setDestination(it->getDestination());
    offer.setType(it->getType());
    offer.setPrice(it->getPrice());
}

/*
 * Gets the index of an offer by name
 * name: string reference, name of the offer
 * return: the index of the offer
 */
int Controller::getOfferIndex(const string &name)
{
    int i = 0;
    auto it = find_if(_repository.getAgency().begin(),
                      _repository.getAgency().end(),
                      [&name, &i](auto& x){
                            i++;
                            return  x.getName() == name; });

    if(it == this->_repository.getAgency().end()) return -1;

    return i;
}

/*
 * Filters the list of offers by destination or price
 * filtered_repository: Repository reference, the resulted list (filtered)
 * field: string reference, field to modify
 * searched_value: string reference, new value of the field
 */
void Controller::filter(RepositoryAbstract& filtered_repository, const string &field, const string &searched_value)
{
    int integer_field;
    string temporary_field = field;
    Validate::strToLower(temporary_field);
    Validate::noWhitespaces(temporary_field);

    if(temporary_field == "destination") integer_field = 1;
    if(temporary_field == "price") integer_field = 2;

    Validate::validateFilter(integer_field, searched_value);
    vector<Offer> vector;

    switch (integer_field)
    {
        case 1:

            copy_if(this->_repository.getAgency().begin(), this->_repository.getAgency().end(),
                    back_inserter(vector),
                    [&searched_value](auto& x)
                    { return x.getDestination() == searched_value;});

            break;

        case 2:

            copy_if(this->_repository.getAgency().begin(), this->_repository.getAgency().end(),
                    back_inserter(vector),
                    [&searched_value](auto x)
                    { return x.getPrice() == stoi(searched_value);});

            break;
    }

    filtered_repository.getAgency() = vector;
}

/*
 * Sorts a repository
 * sorted_repository: Repository reference, the resulted repository (sorted)
 * field: string reference, field to sort after
 * mode: string reference, ascendent or descendent
 */
void Controller::sorted(RepositoryAbstract& sorted_repository, const string &field, const string &mode)
{
    int integer_field = -1;

    string temporary_field = field;
    Validate::strToLower(temporary_field);
    Validate::noWhitespaces(temporary_field);

    if(temporary_field == "name") integer_field = 1;
    if(temporary_field == "destination") integer_field = 2;
    if(temporary_field == "type") integer_field = 3;

    int integer_mode = -1;

    string temporary_mode = mode;
    Validate::strToLower(temporary_mode);
    Validate::noWhitespaces(temporary_mode);

    if(temporary_mode == "ascendent") integer_mode = 0;
    if(temporary_mode == "descendent") integer_mode = 1;

    Validate::validateSort(integer_field, integer_mode);

    sorted_repository.getAgency() = this->getRepository().getAgency();

    switch(integer_field)
    {
        case 1:
            if(integer_mode) sort(sorted_repository.getAgency().begin(), sorted_repository.getAgency().end(), lowerName);
            else sort(sorted_repository.getAgency().begin(), sorted_repository.getAgency().end(), greaterName);
            break;

        case 2:
            if(integer_mode) sort(sorted_repository.getAgency().begin(), sorted_repository.getAgency().end(), lowerDestination);
            else sort(sorted_repository.getAgency().begin(), sorted_repository.getAgency().end(), greaterDestination);
            break;

        case 3:
            if(integer_mode) sort(sorted_repository.getAgency().begin(), sorted_repository.getAgency().end(), lowerTypePrice);
            else sort(sorted_repository.getAgency().begin(), sorted_repository.getAgency().end(), greaterTypePrice);
            break;
    }

}

/*
 * Compares two offers after the name (ascendent)
 * offer_1: Offer reference, first offer to compare
 * offer_2: Offer reference, second offer to compare
 */
bool Controller::greaterName(Offer& offer_1, Offer& offer_2)
{
    return offer_1.getName() < offer_2.getName();
}

/*
 * Compares two offers after the name (descendent)
 * offer_1: Offer reference, first offer to compare
 * offer_2: Offer reference, second offer to compare
 */
bool Controller::lowerName(Offer& offer_1, Offer& offer_2)
{
    return offer_1.getName() > offer_2.getName();
}

/*
 * Compares two offers after the destination (ascendent)
 * offer_1: Offer reference, first offer to compare
 * offer_2: Offer reference, second offer to compare
 */
bool Controller::greaterDestination(Offer& offer_1, Offer& offer_2)
{
    return offer_1.getDestination() < offer_2.getDestination();
}

/*
 * Compares two offers after the name (descendent)
 * offer_1: Offer reference, first offer to compare
 * offer_2: Offer reference, second offer to compare
 */
bool Controller::lowerDestination(Offer& offer_1, Offer& offer_2)
{
    return offer_1.getDestination() > offer_2.getDestination();
}

/*
 * Compares two offers after the price (ascendent)
 * offer_1: Offer reference, first offer to compare
 * offer_2: Offer reference, second offer to compare
 */
bool Controller::greaterTypePrice(Offer& offer_1, Offer& offer_2)
{
    if(offer_1.getType() < offer_2.getType()) return true;
    if(offer_1.getType() == offer_2.getType())
        if(offer_1.getPrice() < offer_2.getPrice()) return true;

    return false;
}

/*
 * Compares two offers after the price (descendent)
 * offer_1: Offer reference, first offer to compare
 * offer_2: Offer reference, second offer to compare
 */
bool Controller::lowerTypePrice(Offer& offer_1, Offer& offer_2)
{
    if(offer_1.getType() > offer_2.getType()) return true;
    if(offer_1.getType() == offer_2.getType())
        if(offer_1.getPrice() > offer_2.getPrice()) return true;

    return false;
}

/*
 * Gets a report of how many offers are for a specified destination
 * destination: string reference, needed destination
 */
int Controller::raport(const string &destination)
{
    unordered_map<string, Destination> map;
    for(const auto& it : this->getRepository().getAgency())
    {
        auto element = map.emplace(it.getDestination(), Destination(it.getDestination()));
        if(element.second) continue;

        element.first->second.increaseCount();
    }

    return map[destination].getCount();
}

/*
 * Does an undo
 */
void Controller::undo()
{
    if(this->_undo_list.empty())
        throw Exception("do undo anymore", "Undo", "Can't");

    unique_ptr<Undo> pointer(this->_undo_list.back());

    pointer->doUndo();
    _undo_list.pop_back();
    this->getRepository().refresh();
}


// wishlist

/*
 * Clears the wishlist
 */
void Controller::clearWishlist()
{
    _wishlist.clearWishlist();
}

/*
 * Adds an offer to the wishlist
 * position: string reference (of an int), index of the offer
 */
void Controller::addToWishlist(const string &position)
{
    Validate::validatePosition(position, "Add to wishlist", (int)_repository.getSize());
    string name = this->_repository.getOffer(stoi(position)).getName();

    for(const auto& it : _wishlist.getWishlist())
        if(name == it) throw Exception("is already in wishlist",
                                       "Wishlist", "The offer");

    _wishlist.addToWishlist(stoi(position));
}

/*
 * Gets a report of how many offers are for a specified destination
 * destination: string reference, needed destination
 */
void Controller::generateWishlist(const string &number)
{
    Validate::validatePosition(number, "Generate wishlist", (int)_repository.getSize());
    _wishlist.generateWishlist(stoi(number));
}

void Controller::exportWishlist()
{
    this->getWishlist().exportToCSV("../export/export.csv");
    this->getWishlist().exportToHTLM("../export/export.html");
}

//Getters

/*
 * Gets the repository
 * return: Repository reference, repository
 */
RepositoryAbstract& Controller::getRepository()
{
    return this->_repository;
}

/*
 * Gets the wishlist
 * return: Wishlist reference, wishlist
 */
Wishlist &Controller::getWishlist()
{
    return this->_wishlist;
}


//tests

void Controller::testMode()
{
    Controller::test_mode = !test_mode;
}

void Controller::tests()
{
    testAdd();
    testRemove();
    testModify();
    testSearch();
    testGetOfferIndex();
    testFilter();
    testSort();
    testRaport();
    testUndo();

    testClearWishlist();
    testAddToWishlist();
    testGenerateWishlist();
}

void Controller::testAdd()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "2591");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Three", "Paris", "3", "5000");


    try{
        service.add("Three", "Paris", "3", "5000");
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    assert(service.getRepository().getSize() == 3);
}

void Controller::testRemove()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "2591");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Three", "Paris", "3", "5000");

    service.remove("2");

    assert(service.getRepository().getSize() == 2);

    assert(service.getRepository().getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(service.getRepository().getOffer(2) == Offer("Three", "Paris", 3, 5000));
}

void Controller::testModify()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "2591");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Three", "Paris", "3", "5000");


    service.modify("name", "Four", "1");
    assert(service.getRepository().getOffer(1) == Offer("Four", "Dubai", 1, 2591));

    service.modify("destination", "NYC", "1");
    assert(service.getRepository().getOffer(1) == Offer("Four", "NYC", 1, 2591));

    service.modify("type", "5", "1");
    assert(service.getRepository().getOffer(1) == Offer("Four", "NYC", 5, 2591));

    service.modify("price", "9000", "1");
    assert(service.getRepository().getOffer(1) == Offer("Four", "NYC", 5, 9000));
}

void Controller::testFilter()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Two", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");
    service.add("Five", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    Repository filtered_repository;

    // Destination
    service.filter(filtered_repository, "destination", "Bucharest");

    assert(filtered_repository.getSize() == 3);

    assert(filtered_repository.getOffer(1) == Offer("Two", "Bucharest", 2, 1500));
    assert(filtered_repository.getOffer(2) == Offer("Five", "Bucharest", 2, 3000));
    assert(filtered_repository.getOffer(3) == Offer("Four", "Bucharest", 2, 9182));


    filtered_repository.clear();
    service.filter(filtered_repository, "destination", "Dubai");

    assert(filtered_repository.getSize() == 2);

    assert(filtered_repository.getOffer(1) == Offer("One", "Dubai", 1, 7000));
    assert(filtered_repository.getOffer(2) == Offer("Six", "Dubai", 1, 5000));


    // Price

    filtered_repository.clear();
    service.filter(filtered_repository, "price", "5000");

    assert(filtered_repository.getSize() == 2);

    assert(filtered_repository.getOffer(1) == Offer("Three", "Paris", 3, 5000));
    assert(filtered_repository.getOffer(2) == Offer("Six", "Dubai", 1, 5000));

}

void Controller::testGetOfferIndex()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Two", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Five", "Dubai", "1", "5000");
    service.add("Six", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");


    assert(service.getOfferIndex("Ten") == -1);
    assert(service.getOfferIndex("Two") == 2);
    assert(service.getOfferIndex("Six") == 5);
    assert(service.getOfferIndex("One") == 1);
    assert(service.getOfferIndex("Four") == 6);
    assert(service.getOfferIndex("Three") == 3);
}


void Controller::testSearch()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Two", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Five", "Dubai", "1", "5000");
    service.add("Six", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    Offer searched_offer;

    service.search(searched_offer, "Ten");
    assert(searched_offer == Offer("", "", -1, -1));

    service.search(searched_offer, "One");
    assert(searched_offer == Offer("One", "Dubai", 1, 7000));

    service.search(searched_offer, "Five");
    assert(searched_offer == Offer("Five", "Dubai", 1, 5000));

    service.search(searched_offer, "Three");
    assert(searched_offer == Offer("Three", "Paris", 3, 5000));

    service.search(searched_offer, "Four");
    assert(searched_offer == Offer("Four", "Bucharest", 2, 9182));

    service.search(searched_offer, "Six");
    assert(searched_offer == Offer("Six", "Bucharest", 2, 3000));

    service.search(searched_offer, "Two");
    assert(searched_offer == Offer("Two", "Bucharest", 2, 1500));

}

void Controller::testSort()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Five", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    Repository sorted_repository(service.getRepository());

    // Destination
    service.sorted(sorted_repository, "destination", "ascendent");

    assert(sorted_repository.getSize() == 6);

    assert(sorted_repository.getOffer(1).getDestination() == "Bucharest");
    assert(sorted_repository.getOffer(2).getDestination() == "Bucharest");
    assert(sorted_repository.getOffer(3).getDestination() == "Bucharest");
    assert(sorted_repository.getOffer(4).getDestination() == "Dubai");
    assert(sorted_repository.getOffer(5).getDestination() == "Dubai");
    assert(sorted_repository.getOffer(6).getDestination() == "Paris");

    service.sorted(sorted_repository, "destination", "descendent");

    assert(sorted_repository.getSize() == 6);

    assert(sorted_repository.getOffer(1).getDestination() == "Paris");
    assert(sorted_repository.getOffer(2).getDestination() == "Dubai");
    assert(sorted_repository.getOffer(3).getDestination() == "Dubai");
    assert(sorted_repository.getOffer(4).getDestination() == "Bucharest");
    assert(sorted_repository.getOffer(5).getDestination() == "Bucharest");
    assert(sorted_repository.getOffer(6).getDestination() == "Bucharest");



    // Name
    service.sorted(sorted_repository, "name", "ascendent");

    assert(sorted_repository.getSize() == 6);

    assert(sorted_repository.getOffer(1).getName() == "Five");
    assert(sorted_repository.getOffer(2).getName() == "Four");
    assert(sorted_repository.getOffer(3).getName() == "One");
    assert(sorted_repository.getOffer(4).getName() == "Six");
    assert(sorted_repository.getOffer(5).getName() == "Three");
    assert(sorted_repository.getOffer(6).getName() == "Two");

    service.sorted(sorted_repository, "name", "descendent");

    assert(sorted_repository.getSize() == 6);


    assert(sorted_repository.getOffer(1).getName() == "Two");
    assert(sorted_repository.getOffer(2).getName() == "Three");
    assert(sorted_repository.getOffer(3).getName() == "Six");
    assert(sorted_repository.getOffer(4).getName() == "One");
    assert(sorted_repository.getOffer(5).getName() == "Four");
    assert(sorted_repository.getOffer(6).getName() == "Five");



    // (type, price)

    service.sorted(sorted_repository, "type", "ascendent");

    assert(sorted_repository.getSize() == 6);


    assert(sorted_repository.getOffer(1).getName() == "Six");
    assert(sorted_repository.getOffer(2).getName() == "One");
    assert(sorted_repository.getOffer(3).getName() == "Five");
    assert(sorted_repository.getOffer(4).getName() == "Two");
    assert(sorted_repository.getOffer(5).getName() == "Four");
    assert(sorted_repository.getOffer(6).getName() == "Three");

    service.sorted(sorted_repository, "type", "descendent");

    assert(sorted_repository.getSize() == 6);


    assert(sorted_repository.getOffer(1).getName() == "Three");
    assert(sorted_repository.getOffer(2).getName() == "Four");
    assert(sorted_repository.getOffer(3).getName() == "Two");
    assert(sorted_repository.getOffer(4).getName() == "Five");
    assert(sorted_repository.getOffer(5).getName() == "One");
    assert(sorted_repository.getOffer(6).getName() == "Six");

}

void Controller::testRaport()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Five", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    assert(service.raport("Bucharest") == 3);
    assert(service.raport("Dubai") == 2);
    assert(service.raport("Paris") == 1);
}

void Controller::testClearWishlist()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Five", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    service.addToWishlist("1");
    service.addToWishlist("5");
    service.addToWishlist("6");
    service.addToWishlist("2");
    service.addToWishlist("4");

    assert(service.getWishlist().getWishlist().size() == 5);

    service.clearWishlist();

    assert(service.getWishlist().getWishlist().empty());

    service.addToWishlist("6");
    service.addToWishlist("2");
    service.addToWishlist("4");

    assert(service.getWishlist().getWishlist().size() == 3);

    service.clearWishlist();

    assert(service.getWishlist().getWishlist().empty());

}

void Controller::testAddToWishlist()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Five", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    service.addToWishlist("1");

    auto it = wishlist.getWishlist().end()-1;
    assert(*it == "One");

    service.addToWishlist("5");

    it = wishlist.getWishlist().end()-1;
    assert(*it == "Two");

    service.addToWishlist("3");

    it = wishlist.getWishlist().end()-1;
    assert(*it == "Three");

    try
    {
        service.addToWishlist("-1");
        assert(false);
    }
    catch(Exception& exception)
    {
        assert(true);
    }

    try
    {
        service.addToWishlist("10");
        assert(false);
    }
    catch(Exception& exception)
    {
        assert(true);
    }

    try
    {
        service.addToWishlist("0");
        assert(false);
    }
    catch(Exception& exception)
    {
        assert(true);
    }
}

void Controller::testGenerateWishlist()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    service.add("One", "Dubai", "1", "7000");
    service.add("Five", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");
    service.add("Two", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    service.generateWishlist("3");
    assert(service.getWishlist().getWishlist().size() == 3);
    service.clearWishlist();

    service.generateWishlist("1");
    assert(service.getWishlist().getWishlist().size() == 1);
    service.clearWishlist();

    service.generateWishlist("5");
    assert(service.getWishlist().getWishlist().size() == 5);
    service.clearWishlist();

    try
    {
        service.generateWishlist("-1");
        assert(false);
    }
    catch(Exception& exception)
    {
        assert(true);
    }

    try
    {
        service.generateWishlist("10");
        assert(false);
    }
    catch(Exception& exception)
    {
        assert(true);
    }

    try
    {
        service.generateWishlist("0");
        assert(false);
    }
    catch(Exception& exception)
    {
        assert(true);
    }

    service.exportWishlist();
}

void Controller::testUndo()
{
    Repository repository;
    Wishlist wishlist(repository);
    Controller service = Controller(repository, wishlist);

    try
    {
        service.undo();
        assert(false);
    }
    catch (Exception& exception)
    {
        assert(true);
    }

    service.add("One", "Dubai", "1", "7000");
    service.add("Five", "Bucharest", "2", "1500");
    service.add("Three", "Paris", "3", "5000");
    service.add("Six", "Dubai", "1", "5000");

    service.add("Two", "Bucharest", "2", "3000");
    service.add("Four", "Bucharest", "2", "9182");

    service.undo();
    assert(service.getRepository().getSize() == 5);

    service.undo();
    assert(service.getRepository().getSize() == 4);

    service.remove("3");
    service.remove("1");

    service.undo();
    assert(service.getRepository().getSize() == 3);

    service.undo();
    assert(service.getRepository().getSize() == 4);

    service.modify("name", "Four", "3");
    service.modify("destination", "NYC", "3");
    service.modify("type", "5", "3");
    service.modify("price", "9000", "3");

    service.undo();
    assert(service.getRepository().getOffer(3) == Offer("Four", "NYC", 5, 7000));

    service.undo();
    assert(service.getRepository().getOffer(3) == Offer("Four", "NYC", 1, 7000));

    service.undo();
    assert(service.getRepository().getOffer(3) == Offer("Four", "Dubai", 1, 7000));

    service.undo();
    assert(service.getRepository().getOffer(3) == Offer("One", "Dubai", 1, 7000));
}

Controller::~Controller()
{
    while(!this->_undo_list.empty())
    {
        delete this->_undo_list.back();
        this->_undo_list.pop_back();
    }
}
