//
// Created by Grigore Sima on 29.04.2023.
//

#include "undo.h"

UndoAdd::UndoAdd(RepositoryAbstract &repository, const Offer& offer) :
_repository(repository), _offer(offer) {}

void UndoAdd::doUndo()
{
    unsigned long i = 1;
    while(i <= this->_repository.getSize())
    {
        if(this->_repository.getOffer((int)i) == this->_offer) break;
        i++;
    }

    this->_repository.removeOffer((int)i);
}

void UndoAdd::tests()
{
    Repository repo;
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));

    UndoAdd undo(repo, Offer("One", "Dubai", 1, 2591));
    UndoAdd undo2(repo, Offer("Two", "Bucharest", 2, 3000));

    assert(repo.getSize() == 2);

    undo.doUndo();
    assert(repo.getSize() == 1);

    undo2.doUndo();
    assert(repo.getSize() == 0);
}


UndoRemove::UndoRemove(RepositoryAbstract &repository, const Offer &offer) :
        _repository(repository), _offer(offer) {}

void UndoRemove::doUndo()
{
    this->_repository.addOffer(this->_offer);
}

void UndoRemove::tests()
{
    Repository repo;
    repo.addOffer(Offer("One", "Dubai", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 2, 3000));

    repo.clear();

    UndoRemove undo(repo, Offer("One", "Dubai", 1, 2591));
    UndoRemove undo2(repo, Offer("Two", "Bucharest", 2, 3000));

    assert(repo.getSize() == 0);

    undo.doUndo();
    assert(repo.getSize() == 1);
    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));

    undo2.doUndo();
    assert(repo.getSize() == 2);
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));

    FileRepository repo_file("./repository/test_database");
    repo_file.addOffer(Offer("One", "Dubai", 1, 2591));
    repo_file.addOffer(Offer("Two", "Bucharest", 2, 3000));

    repo_file.clear();

    UndoRemove undo4(repo_file, Offer("Two", "Bucharest", 2, 3000));
    UndoRemove undo3(repo_file, Offer("One", "Dubai", 1, 2591));

    assert(repo_file.getSize() == 0);

    undo4.doUndo();
    assert(repo_file.getSize() == 1);
    assert(repo_file.getOffer(1) == Offer("Two", "Bucharest", 2, 3000));

    undo3.doUndo();
    assert(repo_file.getSize() == 2);
    assert(repo_file.getOffer(2) == Offer("One", "Dubai", 1, 2591));
}

UndoModify::UndoModify(RepositoryAbstract &repository, const Offer& offer) :
        _repository(repository), _offer(offer) {}

void UndoModify::doUndo()
{

    unsigned long i = 1;
    while(i <= this->_repository.getSize())
    {
        if(this->_repository.getOffer((int)i).getName() == this->_offer.getName()
        || (this->_repository.getOffer((int)i).getDestination() == this->_offer.getDestination() &&
            this->_repository.getOffer((int)i).getType() == this->_offer.getType() &&
            this->_repository.getOffer((int)i).getPrice() == this->_offer.getPrice())) break;
        i++;
    }

    this->_repository.getOffer((int)i) = this->_offer;
}

void UndoModify::tests()
{
    Repository repo;
    repo.addOffer(Offer("One", "Paris", 1, 2591));
    repo.addOffer(Offer("Two", "Bucharest", 1, 3000));
    repo.addOffer(Offer("Three", "Roma", 3, 1000));

    UndoModify undo(repo, Offer("One", "Dubai", 1, 2591));
    UndoModify undo2(repo, Offer("Two", "Bucharest", 2, 3000));
    UndoModify undo3(repo, Offer("Three", "Roma", 3, 5000));

    undo.doUndo();
    assert(repo.getOffer(1) == Offer("One", "Dubai", 1, 2591));

    undo2.doUndo();
    assert(repo.getOffer(2) == Offer("Two", "Bucharest", 2, 3000));

    undo3.doUndo();
    assert(repo.getOffer(3) == Offer("Three", "Roma", 3, 5000));

}