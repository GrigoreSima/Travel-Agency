//
// Created by Grigore Sima on 06.04.2023.
//

#include "main.h"

void testMode()
{
    Offer::testMode(); // ON
    Repository::testMode();
    RepositoryLab::testMode();
    Controller::testMode();

    Offer::tests();
    RepositoryLab::tests();
    Repository::tests();
    FileRepository::tests();
    Controller::tests();
    Validate::tests();
    Destination::tests();
    Wishlist::tests();
    UndoAdd::tests();
    UndoRemove::tests();
    UndoModify::tests();

    Offer::testMode(); // OFF
    Repository::testMode();
    RepositoryLab::testMode();
    Controller::testMode();
}

int main(int argc, char **argv)
{
    testMode();

    Offer::testMode();
    FileRepository repository("../repository/database");
    Offer::testMode();

    Wishlist wishlist(repository);
    Controller service(repository, wishlist);

    QApplication app(argc, argv);
    GUI gui(service);

    //WishlistROGUI gui(service);
    gui.run();

    return QApplication::exec();
}
