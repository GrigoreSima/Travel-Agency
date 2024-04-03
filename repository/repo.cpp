//
// Created by Grigore Sima on 06.04.2023.
//

#include "repo.h"

// RepositoryLab

RepositoryLab::RepositoryLab(float probability) : _probability{probability} {}


RepositoryLab::RepositoryLab(RepositoryLab &repo)
{
    for(auto & i : repo.getAgency())
        this->agency.emplace(i.getName(), i);

    for(auto & i : repo.getAgency())
        this->agency_vector.emplace_back(i);

    this->_probability = repo._probability;
}

/*
 * Adds an offer to the RepositoryLab
 * offer: Offer reference, offer to add
 */
void RepositoryLab::addOffer(const Offer & offer)
{
    this->error();
    this->agency.emplace(offer.getName(), offer);
}

/*
 * Adds an offer to the RepositoryLab
 * name: string reference, name of the offer
 * destination: string reference, destination of the offer
 * type: int reference, type of the offer
 * price: int reference, price of the offer
 */
void RepositoryLab::addOffer(const string &name, const string &destination,
                          const int &type, const int &price)
{
    this->error();
    this->agency.emplace(name, Offer(name, destination, type, price));
}

/*
 * Removes an offer from the RepositoryLab
 * position: int reference, position to remove
 */
void RepositoryLab::removeOffer(const int &position)
{
    this->error();

    auto iterator = agency.begin();
    for (unsigned long i = 0; i < agency.size() - position; i++)
    {
        iterator++;
    }
    this->agency.erase(iterator);
}

/*
 * Modifies an offer of the RepositoryLab (for the string fields)
 * field: int reference, field to modify (coded in int -> see in controller)
 * new_value: string reference, new value of the field
 * position: int reference, position to modify
 */
void RepositoryLab::modifyOfferStr(const int& field, const string& new_value,
                                const int& position)
{
    this->error();

    auto iterator = agency.begin();
    for (unsigned long i = 0; i < agency.size() - position; i++)
    {
        iterator++;
    }
    switch (field)
    {
        case 1: // Name
            this->agency[iterator->first].setName(new_value);
            break;

        case 2: // Destination
            this->agency[iterator->first].setDestination(new_value);
            break;
        default:
            break;
    }
}

/*
 * Modifies an offer of the RepositoryLab (for the int fields)
 * field: int reference, field to modify (coded in int -> see in controller)
 * new_value: int reference, new value of the field
 * position: int reference, position to modify
 */
void RepositoryLab::modifyOfferInt(const int& field, const int& new_value,
                                const int& position)
{
    this->error();

    auto iterator = agency.begin();
    for (unsigned long i = 0; i < agency.size() - position; i++)
    {
        iterator++;
    }

    switch (field)
    {
        case 3: // Type
            this->agency[iterator->first].setType(new_value);
            break;

        case 4: // Price
            this->agency[iterator->first].setPrice(new_value);
            break;

        default:
            break;
    }
}

void RepositoryLab::error() const
{
    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<> dist(1, 100);

    float random = (float)dist(mt)/100;

    if(random < this->_probability) throw Exception("No luck this time", "RepositoryLab", "");
}

/*
 * Clears the RepositoryLab (agency)
 */
void RepositoryLab::clear()
{
    this->agency.clear();
}


// Getters

/*
 * Gets the size of the RepositoryLab
 * return: int, size of RepositoryLab
 */
unsigned long RepositoryLab::getSize()
{
    this->error();
    return this->agency.size();
}

/*
 * Gets an offer from the RepositoryLab
 * position: int, position of the offer
 * return: Offer reference, offer searched for
 */
Offer& RepositoryLab::getOffer(int position)
{
    this->error();

    auto iterator = agency.begin();
    for (unsigned long i = 0; i < agency.size() - position; i++)
    {
        iterator++;
    }
     return iterator->second;
}

/*
 * Gets the agency
 * return: vector<Offer> reference, the agency
 */
vector<Offer>& RepositoryLab::getAgency()
{
    this->error();

    agency_vector.clear();
    for(auto & i : this->agency)
        agency_vector.insert(agency_vector.begin(), i.second);

    return agency_vector;
}

//tests

void RepositoryLab::testMode()
{
    RepositoryLab::test_mode = !test_mode;
}

void RepositoryLab::tests()
{
    testConstructor();
    testAddOffer();
    testRemoveOffer();
    testModifyOfferStr();
    testModifyOfferInt();

    testGetSize();

}

void RepositoryLab::testConstructor()
{
    RepositoryLab repo{0.5};
    assert(repo._probability == 0.5);
    repo._probability = 0;

    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));

    RepositoryLab repo2(repo);

    assert(repo2.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo2.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));

    assert(repo2.getAgency().size() == 2);

    repo.refresh();
}

void RepositoryLab::testAddOffer()
{
    RepositoryLab repo{0.0};
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));

    repo.clear();

    repo.addOffer("One", "Dubai", 1, 2591);
    repo.addOffer("Two", "Bucharest", 2, 3000);

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));
}

void RepositoryLab::testRemoveOffer()
{

    RepositoryLab repo{0.0};
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.removeOffer(2);

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Three", "Paris", 3, 5000));

}

void RepositoryLab::testModifyOfferStr()
{
    RepositoryLab repo{0.0};
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.modifyOfferStr(1, "Four", 1);
    assert(repo.getOffer(1) == Offer("Four", "Dubai", 1, 2591));

    repo.modifyOfferStr(2, "NYC", 1);
    assert(repo.getOffer(1) == Offer("Four", "NYC", 1, 2591));

    repo.modifyOfferStr(3, "NYC", 1);
    assert(repo.getOffer(1) == Offer("Four", "NYC", 1, 2591));

}

void RepositoryLab::testModifyOfferInt()
{
    RepositoryLab repo{0.0};
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.modifyOfferInt(3, 5, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 2591));

    repo.modifyOfferInt(4, 9000, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 9000));

    repo.modifyOfferInt(5, 9000, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 9000));
}

void RepositoryLab::testGetSize()
{
    RepositoryLab repo{0.0};
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    assert(repo.getSize() == 3);

    repo.removeOffer(1);
    assert(repo.getSize() == 2);
}


// Repository

Repository::Repository(RepositoryAbstract &repository)
{
    for(Offer& i : repository.getAgency())
        this->agency.emplace_back(i);
}

/*
 * Adds an offer to the repository
 * offer: Offer reference, offer to add
 */
void Repository::addOffer(const Offer & offer)
{
    this->agency.emplace_back(offer);
}

/*
 * Adds an offer to the repository
 * name: string reference, name of the offer
 * destination: string reference, destination of the offer
 * type: int reference, type of the offer
 * price: int reference, price of the offer
 */
void Repository::addOffer(const string &name, const string &destination,
                          const int &type, const int &price)
{
    this->agency.emplace_back(name, destination, type, price);
}

/*
 * Removes an offer from the repository
 * position: int reference, position to remove
 */
void Repository::removeOffer(const int &position)
{
    this->agency.erase(agency.begin() + position - 1);
}

/*
 * Modifies an offer of the repository (for the string fields)
 * field: int reference, field to modify (coded in int -> see in controller)
 * new_value: string reference, new value of the field
 * position: int reference, position to modify
 */
void Repository::modifyOfferStr(const int& field, const string& new_value,
                                const int& position)
{
    switch (field)
    {
        case 1: // Name
            this->agency[position-1].setName(new_value);
            break;

        case 2: // Destination
            this->agency[position-1].setDestination(new_value);
            break;
        default:
            break;
    }
}

/*
 * Modifies an offer of the repository (for the int fields)
 * field: int reference, field to modify (coded in int -> see in controller)
 * new_value: int reference, new value of the field
 * position: int reference, position to modify
 */
void Repository::modifyOfferInt(const int& field, const int& new_value,
                                const int& position)
{
    switch (field)
    {
        case 3: // Type
            this->agency[position-1].setType(new_value);
            break;

        case 4: // Price
            this->agency[position-1].setPrice(new_value);
            break;

        default:
            break;
    }
}

/*
 * Clears the repository (agency)
 */
void Repository::clear()
{
    while(1 <= this->getSize())
        this->agency.erase(agency.begin() + (int)this->getSize() - 1);
}


// Getters

/*
 * Gets the size of the repository
 * return: int, size of repository
 */
unsigned long Repository::getSize()
{
    return this->agency.size();
}

/*
 * Gets an offer from the repository
 * position: int, position of the offer
 * return: Offer reference, offer searched for
 */
Offer& Repository::getOffer(int position)
{
    return this->agency[position-1];
}

/*
 * Gets the agency
 * return: vector<Offer> reference, the agency
 */
vector<Offer>& Repository::getAgency()
{
    return this->agency;
}

//tests

void Repository::testMode()
{
    Repository::test_mode = !test_mode;
}

void Repository::tests()
{
    testAddOffer();
    testRemoveOffer();
    testModifyOfferStr();
    testModifyOfferInt();

    testGetSize();
}

void Repository::testAddOffer()
{
    Repository repo;
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));

    repo.clear();

    repo.addOffer("One", "Dubai", 1, 2591);
    repo.addOffer("Two", "Bucharest", 2, 3000);

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));
}

void Repository::testRemoveOffer()
{

    Repository repo;
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.removeOffer(2);

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Three", "Paris", 3, 5000));

}

void Repository::testModifyOfferStr()
{
    Repository repo;
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.modifyOfferStr(1, "Four", 1);
    assert(repo.getOffer(1) == Offer("Four", "Dubai", 1, 2591));

    repo.modifyOfferStr(2, "NYC", 1);
    assert(repo.getOffer(1) == Offer("Four", "NYC", 1, 2591));

    repo.modifyOfferStr(3, "NYC", 1);
    assert(repo.getOffer(1) == Offer("Four", "NYC", 1, 2591));

}

void Repository::testModifyOfferInt()
{
    Repository repo;
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.modifyOfferInt(3, 5, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 2591));

    repo.modifyOfferInt(4, 9000, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 9000));

    repo.modifyOfferInt(5, 9000, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 9000));
}

void Repository::testGetSize()
{
    Repository repository;
    repository.addOffer(Offer("One", "Dubai", 1, 2591));
    repository.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repository.addOffer(Offer("Three", "Paris", 3, 5000));

    assert(repository.getSize() == 3);

    repository.removeOffer(1);
    assert(repository.getSize() == 2);
}

// FILE REPOSITORY
FileRepository::FileRepository(const string &filename)
{
    this->_filename = filename;
    this->loadFromFile();
}

void FileRepository::addOffer(const Offer & offer)
{
    Repository::addOffer(offer);
    this->saveToFile();
}

void FileRepository::addOffer(const string &name, const string &destination,
                              const int &type, const int &price)
{
    Repository::addOffer(name, destination, type, price);
    this->saveToFile();
}

void FileRepository::removeOffer(const int &position)
{
    Repository::removeOffer(position);
    this->saveToFile();
}

void FileRepository::modifyOfferStr(const int& field, const string& new_value,
                                    const int& position)
{
    Repository::modifyOfferStr(field, new_value, position);
    this->saveToFile();
}

void FileRepository::modifyOfferInt(const int& field, const int& new_value,
                                    const int& position)
{
    Repository::modifyOfferInt(field, new_value, position);
    this->saveToFile();
}

void FileRepository::saveToFile()
{
    ofstream out(this->_filename);
    /*
    if(!out.is_open())
    {
        throw Exception("does not exist", "Database", "File");
    }
    */

    for(const auto& it : Repository::getAgency())
    {
        out<<it.getName()<<" "<<it.getDestination()<<" "<<
           it.getType()<<" "<<it.getPrice();
        if(Repository::getAgency()[Repository::getSize()-1] != it)
            out<<"\n";
    }
    out.close();
}

void FileRepository::loadFromFile()
{
    ifstream in(this->_filename);
    /*
    if(!in.is_open())
    {
        throw Exception("does not exist", "Database", "File");
    }
    */

    Repository::clear();

    while(!in.eof())
    {
        string name;
        in>>name;

        string destination;
        in>>destination;

        int type;
        in>>type;

        int price;
        in>>price;

        if(name.empty() || destination.empty() || type == -1 || price == -1)
            return;

        Repository::addOffer(name, destination, type, price);
    }

    in.close();
}

void FileRepository::tests()
{
    testCreate();

    testAddOffer();
    testRemoveOffer();
    testModifyOfferStr();
    testModifyOfferInt();

    FileRepository repository("../repository/test_database");
    repository.clear();

    repository.addOffer(Offer("One", "Dubai", 1, 2591));
    repository.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repository.addOffer(Offer("Three", "Paris", 3, 5000));

    repository.saveToFile();
    repository.loadFromFile();

    assert(repository.getSize() == 3);

    assert(repository.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repository.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));
    assert(repository.getOffer(3) == Offer("Three", "Paris", 3, 5000));

}

void FileRepository::testCreate()
{
    FileRepository repository("../repository/test_database");
    assert(repository._filename == "../repository/test_database");
}

void FileRepository::testAddOffer()
{
    FileRepository repo("../repository/test_database");

    repo.clear();
    repo.refresh();

    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));

    repo.clear();

    repo.addOffer("One", "Dubai", 1, 2591);
    repo.addOffer("Two", "Bucharest", 2, 3000);

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));
}

void FileRepository::testRemoveOffer()
{

    FileRepository repo("../repository/test_database");

    repo.clear();
    repo.refresh();

    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.removeOffer(2);

    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));
    assert(repo.getOffer(2) == Offer("Three", "Paris", 3, 5000));

}

void FileRepository::testModifyOfferStr()
{
    FileRepository repo("../repository/test_database");

    repo.clear();
    repo.refresh();

    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.modifyOfferStr(1, "Four", 1);
    assert(repo.getOffer(1) == Offer("Four", "Dubai", 1, 2591));

    repo.modifyOfferStr(2, "NYC", 1);
    assert(repo.getOffer(1) == Offer("Four", "NYC", 1, 2591));

    repo.modifyOfferStr(3, "NYC", 1);
    assert(repo.getOffer(1) == Offer("Four", "NYC", 1, 2591));

}

void FileRepository::testModifyOfferInt()
{
    FileRepository repo("../repository/test_database");

    repo.clear();
    repo.refresh();

    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repo.addOffer(Offer("Three", "Paris", 3, 5000));

    repo.modifyOfferInt(3, 5, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 2591));

    repo.modifyOfferInt(4, 9000, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 9000));

    repo.modifyOfferInt(5, 9000, 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 5, 9000));
}

// WISHLIST
Wishlist::Wishlist(RepositoryAbstract &repository) : _repository(repository) {}

/*
 * Clears the wishlist
 */
void Wishlist::clearWishlist()
{
    while (!this->wishlist.empty())
        this->wishlist.erase(this->wishlist.begin());
    notify();
}

/*
 * Adds an offer to the wishlist
 * position: int reference, index of the offer
 */
void Wishlist::addToWishlist(const int &position)
{
    this->wishlist.push_back(this->_repository.getOffer(position).getName());
    notify();
}

/*
 * Generates a wishlist with random offers
 * number: int reference, number of the offers
 */
void Wishlist::generateWishlist(const int &number)
{
    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<> dist(1, (int)this->_repository.getSize());

    this->clearWishlist();

    vector<int> last_randoms;
    int random;
    for(int i = 0; i < number; i++)
    {
        while (true)
        {
            random = dist(mt);
            int ok = 1;
            for (auto it: last_randoms) if (it == random) ok = 0;

            if (ok)
            {
                this->addToWishlist(random);
                last_randoms.push_back(random);
                break;
            }
        }

    }
    notify();
}

void Wishlist::exportToCSV(const string &filename)
{
    ofstream out(filename);

    /*
    if (!out.is_open()) {
        cout<<"ceva";
    }
    */

    for (const auto& it : this->wishlist)
    {

        unsigned long i=1;
        while(i < this->_repository.getSize())
        {
            if(this->_repository.getOffer((int)i).getName() == it) break;
            i++;
        }

        Offer offer = this->_repository.getOffer((int)i);
        out<< offer.getName() << "," << offer.getDestination() << "," << offer.getType() << "," << offer.getPrice() << '\n';
    }
    out.close();
}

void Wishlist::exportToHTLM(const string &filename)
{
    ofstream out(filename);

    /*
    if (!out.is_open()) {
        cout<<"ceva";
    }
    */

    out << "<html><body>" << '\n';
    out << R"(<table border="1" style="width:100 % ">)" << '\n';
    for (const auto& it : this->wishlist)
    {
        unsigned long i=1;
        while(i < this->_repository.getSize())
        {
            if(this->_repository.getOffer((int)i).getName() == it) break;
            i++;
        }

        Offer offer = this->_repository.getOffer((int)i);

        out << "<tr>" << '\n';
        out << "<td>" << offer.getName() << "</td>" << '\n';
        out << "<td>" << offer.getDestination() << "</td>" << '\n';
        out << "<td>" << offer.getType() << "</td>" << '\n';
        out << "<td>" << offer.getPrice() << "</td>" << '\n';
        out << "</tr>" << '\n';
    }
    out << "</table>" << '\n';
    out << "</body></html>" << '\n';
    out.close();
}

/*
 * Gets the size of the wishlist
 * return: int, size of wishlist
 */
unsigned long Wishlist::getSize()
{
    return this->wishlist.size();
}


/*
 * Gets the wishlist
 * return: vector<string> reference, the wishlist
 */
vector<string> &Wishlist::getWishlist()
{
    return this->wishlist;
}

void Wishlist::tests()
{
    testClearWishlist();
    testAddToWishlist();
    testGenerateWishlist();

    testGetSize();
}

void Wishlist::testAddToWishlist()
{
    Repository repository;
    Wishlist wishlist(repository);

    repository.addOffer(Offer("One", "Dubai", 1, 2591));
    repository.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repository.addOffer(Offer("Three", "Paris", 3, 5000));

    wishlist.addToWishlist(2);
    assert(wishlist.wishlist.size() == 1);

    auto it = wishlist.getWishlist().end()-1;
    assert(*it == "Two");

    wishlist.addToWishlist(1);
    assert(wishlist.wishlist.size() == 2);

    it = wishlist.getWishlist().end()-1;
    assert(*it == "One");

    wishlist.addToWishlist(3);
    assert(wishlist.wishlist.size() == 3);

    it = wishlist.getWishlist().end()-1;
    assert(*it == "Three");
}

void Wishlist::testClearWishlist()
{
    Repository repository;
    Wishlist wishlist(repository);

    repository.addOffer(Offer("One", "Dubai", 1, 2591));
    repository.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repository.addOffer(Offer("Three", "Paris", 3, 5000));

    wishlist.addToWishlist(2);
    wishlist.addToWishlist(1);
    wishlist.addToWishlist(3);

    assert(wishlist.wishlist.size() == 3);

    wishlist.clearWishlist();

    assert(wishlist.wishlist.empty());

    wishlist.addToWishlist(2);
    wishlist.addToWishlist(1);

    assert(wishlist.wishlist.size() == 2);

    wishlist.clearWishlist();

    assert(wishlist.wishlist.empty());
}

void Wishlist::testGenerateWishlist()
{
    Repository repository;
    Wishlist wishlist(repository);

    repository.addOffer(Offer("One", "Dubai", 1, 2591));
    repository.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repository.addOffer(Offer("Three", "Paris", 3, 5000));

    wishlist.generateWishlist(1);
    assert(wishlist.wishlist.size() == 1);
    wishlist.clearWishlist();

    wishlist.generateWishlist(2);
    assert(wishlist.wishlist.size() == 2);
    wishlist.clearWishlist();

    wishlist.generateWishlist(3);
    assert(wishlist.wishlist.size() == 3);

    wishlist.exportToCSV("../export/export.csv");
    wishlist.exportToHTLM("../export/export.html");

    wishlist.clearWishlist();
}

void Wishlist::testGetSize()
{
    Repository repository;
    Wishlist wishlist(repository);

    repository.addOffer(Offer("One", "Dubai", 1, 2591));
    repository.addOffer(Offer("Two", "Bucharest", 2, 3000));
    repository.addOffer(Offer("Three", "Paris", 3, 5000));

    wishlist.addToWishlist(1);
    wishlist.addToWishlist(3);
    wishlist.addToWishlist(2);

    assert(wishlist.getSize() == 3);

    wishlist.clearWishlist();
    assert(wishlist.getWishlist().empty());

    wishlist.generateWishlist(2);
    assert(wishlist.getSize() == 2);
}
