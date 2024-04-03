//
// Created by Grigore Sima on 29.04.2023.
//

#ifndef TA_UNDO_H
#define TA_UNDO_H

#include "./repository/repo.h"

class Undo
{
public:
    virtual void doUndo()=0;

    virtual ~Undo() = default;
};

class UndoAdd : public Undo
{
private:
    RepositoryAbstract& _repository;
    Offer _offer;

public:
    UndoAdd(RepositoryAbstract&, const Offer&);
    void doUndo() override;

    static void tests();

    ~UndoAdd() override = default;
};

class UndoRemove : public Undo
{
private:
    RepositoryAbstract& _repository;
    Offer _offer;

public:
    UndoRemove(RepositoryAbstract&, const Offer&);
    void doUndo() override;

    static void tests();

    ~UndoRemove() override = default;
};

class UndoModify : public Undo
{
private:
    RepositoryAbstract& _repository;
    Offer _offer;

public:
    UndoModify(RepositoryAbstract&, const Offer&);
    void doUndo() override;

    static void tests();

    ~UndoModify() override = default;
};

#endif //TA_UNDO_H
