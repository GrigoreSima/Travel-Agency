//
// Created by Grigore Sima on 16.05.2023.
//

#include "gui.h"

GUI::GUI(Controller &controller) : service{controller},
main_window{new QMainWindow}, dynamic_buttons{new QDockWidget},
upper_dock{new QDockWidget}
{}
//wishlist_window{new QTableWidget(0, 4)} {}

/*
 * the main widget will be the table with the offers
 * the last row is for adding a new entry (maybe a button for adding)
 * each row will have a delete button on the right / left
 * if you modify a cell, the offer will be modified
 * the table will be up to date every second ( after each modification )
 *
 * above each column of the table will be a field for
 * filtration (after you type it instanlty filters the table)
 * the field above the name column will be for search
 *
 * on the right ( dockwidget ) will be the dynamic buttons (lab 10)
 * others buttons on the left (dockwidget) or under the table
 *
 * undo in the menu bar
 * a button to acces a new window for wishlist functionalities
 */
void GUI::run()
{
    initMenu();
    initDynamicButtons();
    initUpperDock();
    initMainWindow();
}

void GUI::initMenu()
{
    auto menu = main_window->menuBar()->addMenu("Undo");
    menu->addAction("Do undo");

    menu = main_window->menuBar()->addMenu("Wishlist");
    menu->addAction("Show");
    menu->addAction("Read-only");
    menu->addAction("Clear");
    menu->addAction("Add");
    menu->addAction("Generate");
    menu->addAction("Export");

    QObject::connect(main_window->menuBar(), &QMenuBar::triggered,
                     [&](QAction* action){

                         if(action->text().toStdString() == "Do undo") undo();

                         if(action->text().toStdString() == "Show")
                         {
                             auto* wishlistCrudGui = new WishlistCRUDGUI(this->service);
                             service.getWishlist().addObserver(wishlistCrudGui);
                             wishlistCrudGui->run();
                         }
                         if(action->text().toStdString() == "Read-only")
                         {
                             auto* wishlistCrudGui = new WishlistROGUI(this->service);
                             service.getWishlist().addObserver(wishlistCrudGui);
                             wishlistCrudGui->run();
                         }
                         if(action->text().toStdString() == "Clear") clearWishlist();
                         if(action->text().toStdString() == "Add") addToWishlist();
                         if(action->text().toStdString() == "Generate") generateWishlist();
                         if(action->text().toStdString() == "Export") exportWishlist();
                     });
}

void GUI::initMainWindow()
{
    main_window->setCentralWidget(new QTableWidget(
            (int)service.getRepository().getSize(), 5));
    auto* table = (QTableWidget*) main_window->centralWidget();
    table->setHorizontalHeaderLabels(
            QStringList()<<"Name"<<"Destination"<<"Type"<<"Price"<<"Option");
    // modify
    QObject::connect(table,
                     &QTableWidget::cellChanged,
                     [&, table](int row, int column)
                     {
                         modify(row, column,
                                table->item(row, column)
                                 ->text().toStdString());
                     });

    // sort
    QObject::connect(table->horizontalHeader(),
                     &QHeaderView::sectionClicked,
                     [&](int index)
                     {
                         sort(index);
                     });
    showOffers(service.getRepository());

    main_window->addDockWidget(Qt::TopDockWidgetArea, upper_dock);
    main_window->addDockWidget(Qt::RightDockWidgetArea, dynamic_buttons);

    main_window->centralWidget()->adjustSize();
    main_window->adjustSize();
    main_window->setWindowTitle(QString("Travel agency"));
    main_window->setMinimumSize(width, height);
    main_window->show();
}

void GUI::initUpperDock()
{
    upper_dock->setWidget(new QWidget());
    upper_dock->widget()->setLayout(new QGridLayout);
    upper_dock->setWindowTitle(QString("Filter and search"));
    upper_dock->adjustSize();
    upper_dock->widget()->setMinimumSize(width, 80);
    upper_dock->setAllowedAreas(Qt::TopDockWidgetArea);
    filterAndSearch();
}

void GUI::initDynamicButtons()
{
    dynamic_buttons->setWidget(new QWidget());
    dynamic_buttons->widget()->setLayout(new QVBoxLayout);
    dynamic_buttons->setWindowTitle(QString("Dynamic buttons"));
    dynamic_buttons->adjustSize();
    dynamic_buttons->widget()->setMinimumSize(180, height);
    dynamic_buttons->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    raport();
}

void GUI::showException(Exception &exception)
{
    auto* warning_msg = new QMessageBox;
    warning_msg->setText(exception.what());
    warning_msg->setIcon(QMessageBox::Warning);
    warning_msg->exec();
}

void GUI::showOffers(RepositoryAbstract &repository)
{
    auto* table = (QTableWidget*) main_window->centralWidget();
    table->blockSignals(true);
    repository.refresh();

    while(table->rowCount()) table->removeRow(0);

    for(int i = 0; i < (int)repository.getSize(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(
                QString(repository.getOffer(i+1).getName().c_str())));
        table->item(i,0)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 1, new QTableWidgetItem(
                QString(repository.getOffer(i+1).getDestination().c_str())));
        table->item(i,1)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 2, new QTableWidgetItem(
                QString(to_string(repository.getOffer(i+1).getType()).c_str())));
        table->item(i,2)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 3, new QTableWidgetItem(
                QString(to_string(repository.getOffer(i+1).getPrice()).c_str())));
        table->item(i,3)->setTextAlignment(Qt::AlignCenter);


        // remove
        auto* remove_button = new QPushButton("DELETE");
        table->setCellWidget(i, 4, remove_button);
        QObject::connect(remove_button,
                         &QPushButton::clicked,
                         [&, i, table]()
                         {
                            remove(table->item(i, 0)->text().toStdString());
                            service.getRepository().refresh();
                            table->removeRow(i);
                            raport();
                         });
    }
    // add
    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0,
                   new QTableWidgetItem(QString("")));
    table->setItem(table->rowCount() - 1, 1,
                   new QTableWidgetItem(QString("")));
    table->setItem(table->rowCount() - 1, 2,
                   new QTableWidgetItem(QString("")));
    table->setItem(table->rowCount() - 1, 3,
                   new QTableWidgetItem(QString("")));


    table->item(table->rowCount() - 1,0)->setTextAlignment(Qt::AlignCenter);
    table->item(table->rowCount() - 1,1)->setTextAlignment(Qt::AlignCenter);
    table->item(table->rowCount() - 1,2)->setTextAlignment(Qt::AlignCenter);
    table->item(table->rowCount() - 1,3)->setTextAlignment(Qt::AlignCenter);

    auto* add_button = new QPushButton("ADD");
    table->setCellWidget(table->rowCount() - 1, 4, add_button);
    QObject::connect(add_button,
                     &QPushButton::clicked,
                     [&]()
                     {
                         add(repository);
                     });

    table->blockSignals(false);
}

void GUI::add(RepositoryAbstract &repository)
{
    auto* table = (QTableWidget*) main_window->centralWidget();

    table->blockSignals(true);
    service.getRepository().refresh();
    try
    {
        service.add(
                table->item(table->rowCount() - 1, 0)->text().toStdString(),
                table->item(table->rowCount() - 1, 1)->text().toStdString(),
                table->item(table->rowCount() - 1, 2)->text().toStdString(),
                table->item(table->rowCount() - 1, 3)->text().toStdString()
        );
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
    table->insertRow(table->rowCount()-1);

    // update the new row with data
    int row = table->rowCount()-2;
    auto* remove_button = new QPushButton("DELETE");
    table->setCellWidget(row, 4,
                         remove_button);

    table->setItem(row, 0, new QTableWidgetItem(
            QString(repository.getOffer(row+1).getName().c_str())));
    table->item(row,0)->setTextAlignment(Qt::AlignCenter);

    table->setItem(row, 1, new QTableWidgetItem(
            QString(repository.getOffer(row+1).getDestination().c_str())));
    table->item(row,1)->setTextAlignment(Qt::AlignCenter);

    table->setItem(row, 2, new QTableWidgetItem(
            QString(to_string(repository.getOffer(row+1).getType()).c_str())));
    table->item(row,2)->setTextAlignment(Qt::AlignCenter);

    table->setItem(row, 3, new QTableWidgetItem(
            QString(to_string(repository.getOffer(row+1).getPrice()).c_str())));
    table->item(row,3)->setTextAlignment(Qt::AlignCenter);


    QObject::connect(remove_button,
                     &QPushButton::clicked,
                     [&, row, table]()
                     {
                         remove(table->item(row, 0)->text().toStdString());
                         service.getRepository().refresh();
                         table->removeRow(row);
                         raport();
                     });

    // reset the text of the add row
    table->item(table->rowCount() - 1, 0)
            ->setText(QString(""));
    table->item(table->rowCount() - 1, 1)
            ->setText(QString(""));
    table->item(table->rowCount() - 1, 2)
            ->setText(QString(""));
    table->item(table->rowCount() - 1, 3)
            ->setText(QString(""));
    raport();
    table->blockSignals(false);
}

void GUI::remove(const string& name)
{
    try
    {
        service.remove(to_string(service.getOfferIndex(name)));
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
}

void GUI::modify(const int& row, const int& column, const string& updated_value)
{
    auto* table = (QTableWidget*) main_window->centralWidget();
    table->blockSignals(true);

    if(row >= table->rowCount() - 1) return;
    if(column >= table->rowCount() - 1) return;

    try
    {
        service.modify(table->horizontalHeaderItem(column)
                               ->text().toStdString(), updated_value, to_string(row + 1));
    }
    catch (Exception& exception)
    {
        showException(exception);
    }

    service.getRepository().refresh();
    raport();
    table->blockSignals(false);
}

void GUI::filterAndSearch()
{
    auto* filter_textbox = new QLineEdit;
    filter_textbox->setReadOnly(true);
    filter_textbox->setToolTip("Value for filtering");
    //filter_textbox->setPlaceholderText(" Filter");

    auto* search_textbox = new QLineEdit;
    search_textbox->setToolTip("Value for searching");
    //search_textbox->setPlaceholderText(" Search");

    auto* filter_popup_list = new QComboBox;
    filter_popup_list->addItems(QStringList()<<"No filter"<<"Destination"<<"Price");

    auto* layout = (QGridLayout*) upper_dock->widget()->layout();
    layout->addWidget(new QLabel("Filter: "), 0, 0);
    layout->addWidget(filter_textbox, 0, 1);
    layout->addWidget(filter_popup_list, 0, 2);

    layout->addWidget(new QLabel("Search: "), 1, 0);
    layout->addWidget(search_textbox, 1, 1);

    QObject::connect(filter_textbox,
                     &QLineEdit::textEdited,
                     [&, filter_popup_list](const QString& text)
                     {
                        Repository filtered_repository;
                        if(text.isEmpty())
                        {
                            showOffers(service.getRepository());
                            return;
                        }

                        try
                        {
                            service.filter(filtered_repository,
                                   filter_popup_list->currentText().toStdString(),
                                   text.toStdString());
                        }
                        catch (Exception& exception)
                        {
                            showException(exception);
                        }

                        showOffers(filtered_repository);
                     });

    QObject::connect(filter_popup_list,
                     &QComboBox::currentTextChanged,
                     [&, filter_textbox](const QString& text)
                     {
                         filter_textbox->clear();
                         filter_textbox->setReadOnly(false);
                         showOffers(service.getRepository());

                         if(text == "No filter") filter_textbox->setReadOnly(true);
                     });

    QObject::connect(search_textbox,
                     &QLineEdit::textEdited,
                     [&](const QString& text)
                     {
                         Repository filtered_repository;
                         if(text.isEmpty())
                         {
                             showOffers(service.getRepository());
                             return;
                         }

                         try
                         {
                             Offer offer;
                             service.search(offer, text.toStdString());
                             if(offer.getPrice() != -1) filtered_repository.addOffer(offer);
                         }
                         catch (Exception& exception)
                         {
                             showException(exception);
                         }

                         showOffers(filtered_repository);
                     });

    upper_dock->update();
}


void GUI::sort(const int& index)
{
    auto* table = (QTableWidget*) main_window->centralWidget();
    table->blockSignals(true);
    if(index >= table->columnCount() - 2)
    {
        table->horizontalHeader()->setSortIndicatorShown(false);
        service.getRepository().refresh();
        showOffers(service.getRepository());
        raport();
        return;
    }

    table->horizontalHeader()->setSortIndicatorShown(true);

    string order;
    if(table->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        order = "ascendent";
    else
        order = "descendent";

    table->blockSignals(false);

    try
    {
        Repository sorted_repository;
        service.sorted(sorted_repository,
                       table->horizontalHeaderItem(index)
                               ->text().toStdString(), order);
        showOffers(sorted_repository);
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
}

void GUI::raport()
{
    set<string> destinations; // List of destinations
    for(int i = 1; i <= (int)service.getRepository().getSize(); i++)
        destinations.insert(
                service.getRepository().getOffer(i).getDestination());

    auto* layout = dynamic_buttons->widget()->layout();
    clearLayout(layout);
    for(const auto& it : destinations)
    {
        auto* raport_button = new QPushButton(it.c_str());
        string lbl_text;
        if(service.raport(it) == 1) lbl_text = "There is " +
                to_string(service.raport(it)) + " offer for " + it + ".";
        else lbl_text = "There are " +
                        to_string(service.raport(it)) + " offers for " + it + ".";

        QObject::connect(raport_button,
                         &QPushButton::clicked,
                         [&, lbl_text]()
                         {
                             auto* raport_window = new QWidget;
                             raport_window->setWindowTitle("Raport");

                             auto label = new QLabel(lbl_text.c_str());
                             label->setAlignment(Qt::AlignCenter);

                             auto layout = new QHBoxLayout;
                             layout->addWidget(label);
                             layout->setAlignment(Qt::AlignCenter);

                             raport_window->setLayout(layout);
                             raport_window->adjustSize();
                             raport_window->setMinimumSize(250, 70);
                             raport_window->show();
                         });

        layout->addWidget(raport_button);
    }

    dynamic_buttons->update();
}

void GUI::undo()
{
    try
    {
        service.undo();
        showOffers(service.getRepository());
        raport();
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
}

void GUI::clearWishlist()
{
    try
    {
        service.clearWishlist();
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
}

void GUI::addToWishlist()
{
    auto window = new QWidget;
    auto layout = new QHBoxLayout;
    auto spin_box = new QSpinBox;
    auto button = new QPushButton("Add !");
    spin_box->setRange(1, (int)service.getRepository().getSize());
    spin_box->setMinimumWidth(50);
    spin_box->setAlignment(Qt::AlignCenter);

    layout->addWidget(spin_box);
    layout->addWidget(button);

    window->setLayout(layout);
    window->setMinimumSize(250, 70);
    window->adjustSize();

    window->setWindowTitle("Add to wishlist");
    window->show();
    QObject::connect(button, &QPushButton::clicked,
                     [&, spin_box](){

                         try
                         {
                             service.addToWishlist(spin_box->text().toStdString());
                         }
                         catch (Exception& exception)
                         {
                             showException(exception);
                         }
                     });
}

void GUI::generateWishlist()
{
    auto window = new QWidget;
    auto layout = new QHBoxLayout;
    auto spin_box = new QSpinBox;
    auto button = new QPushButton("Generate !");
    spin_box->setRange(1, (int)service.getRepository().getSize());
    spin_box->setMinimumWidth(50);
    spin_box->setAlignment(Qt::AlignCenter);

    layout->addWidget(spin_box);
    layout->addWidget(button);

    window->setLayout(layout);
    window->setMinimumSize(250, 70);
    window->adjustSize();

    window->setWindowTitle("Generate wishlist");
    window->show();
    QObject::connect(button, &QPushButton::clicked,
                     [&, spin_box](){

                         try
                         {
                             service.generateWishlist(spin_box->text().toStdString());
                         }
                         catch (Exception& exception)
                         {
                             showException(exception);
                         }
                     });
}

void GUI::exportWishlist()
{
    try
    {
        service.exportWishlist();
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
}

// Table Model
TableModel::TableModel(QWidget *parent, Controller& serv) :
        QAbstractTableModel(parent), service{serv}  {}

/*
void TableModel::setVector(Controller& serv)
{
    service = serv;
    auto topLeft = createIndex(0, 0);
    auto bottomR = createIndex(rowCount(), columnCount());
    emit dataChanged(topLeft, bottomR);
}
*/

int TableModel::rowCount(const QModelIndex &) const
{
    //return (int)service.getWishlist().getSize();
    return 10;
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.row() < (int)service.getWishlist().getSize())
    {
        Offer offer;
        service.search(offer, service.getWishlist().getWishlist()[index.row()]);
        switch(index.column())
        {
            case 0:
                return QString(offer.getName().c_str());

            case 1:
                return QString(offer.getDestination().c_str());

            case 2:
                return QString(to_string(offer.getType()).c_str());

            case 3:
                return QString(to_string(offer.getPrice()).c_str());

            default:
                return {};
        }
    }
    return {};
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation != Qt::Horizontal)
        {
            return QString("%1").arg(section + 1); // row
        }
        else
        {
            switch(section)
            {
                case 0:
                    return QString("Name");

                case 1:
                    return QString("Destination");

                case 2:
                    return QString("Type");

                case 3:
                    return QString("Price");

                default:
                    return {};
            }
        }
    }
    return {};
}

void TableModel::changed()
{
    auto topLeft = createIndex(0, 0);
    auto bottomR = createIndex(rowCount(), columnCount());
    emit dataChanged(topLeft, bottomR);
}

// WISHLIST

WishlistCRUDGUI::WishlistCRUDGUI(Controller &controller) :
        service{controller}, wishlist_window{new QMainWindow},
        buttons{new QDockWidget} {}

void WishlistCRUDGUI::run()
{
    initMainWindow();
    initButtons();
}

void WishlistCRUDGUI::initMainWindow()
{
    //wishlist_window->setCentralWidget(new QTableWidget(
    //        (int)service.getWishlist().getSize(), 4));
    //auto* table = (QTableWidget*) wishlist_window->centralWidget();
    //table->setHorizontalHeaderLabels(
    //        QStringList()<<"Name"<<"Destination"<<"Type"<<"Price");

    auto* table = new QTableView;
    auto* model = new TableModel(table, service);
    table->setModel(model);
    wishlist_window->setCentralWidget(table);


    wishlist_window->addDockWidget(Qt::RightDockWidgetArea, buttons);

    wishlist_window->centralWidget()->adjustSize();
    wishlist_window->adjustSize();
    wishlist_window->setWindowTitle(QString("Wishlist"));
    wishlist_window->setMinimumSize(width, height);
    wishlist_window->show();
}

void WishlistCRUDGUI::initButtons()
{
    buttons->setWidget(new QWidget());
    buttons->widget()->setLayout(new QVBoxLayout);
    buttons->setWindowTitle(QString("Buttons"));
    buttons->adjustSize();
    buttons->widget()->setMinimumSize(150, height);
    buttons->setAllowedAreas(Qt::RightDockWidgetArea);


    auto* layout = buttons->widget()->layout();

    auto* btn = new QPushButton("Clear");
    layout->addWidget(btn);
    connect(btn, &QAbstractButton::clicked,
            [&](){
        clearWishlist();
    });

    btn = new QPushButton("Add");
    layout->addWidget(btn);
    connect(btn, &QAbstractButton::clicked,
            [&](){
                addToWishlist();
            });

    btn = new QPushButton("Generate");
    layout->addWidget(btn);
    connect(btn, &QAbstractButton::clicked,
            [&](){
                generateWishlist();
            });
}

void WishlistCRUDGUI::showException(Exception &exception)
{
    auto *warning_msg = new QMessageBox;
    warning_msg->setText(exception.what());
    warning_msg->setIcon(QMessageBox::Warning);
    warning_msg->exec();
}


void WishlistCRUDGUI::showWishlist(Wishlist &wishlist)
{
    if(!wishlist_window->isVisible()) wishlist_window->show();
    auto table = (QTableWidget*) wishlist_window->centralWidget();

    while(table->rowCount()) table->removeRow(0);

    for(int i = 0; i < (int)wishlist.getSize(); i++)
    {
        Offer offer;
        service.search(offer, wishlist.getWishlist()[i]);

        table->insertRow(i);
        table->blockSignals(true);
        table->setItem(i, 0, new QTableWidgetItem(
                QString(offer.getName().c_str())));
        table->item(i, 0)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 1, new QTableWidgetItem(
                QString(offer.getDestination().c_str())));
        table->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 2, new QTableWidgetItem(
                QString(to_string(offer.getType()).c_str())));
        table->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        table->setItem(i, 3, new QTableWidgetItem(
                QString(to_string(offer.getPrice()).c_str())));
        table->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        table->blockSignals(false);
    }


    wishlist_window->update();
}


void WishlistCRUDGUI::clearWishlist()
{
    try
    {
        service.clearWishlist();
        //showWishlist(service.getWishlist());
        this->update();
    }
    catch (Exception& exception)
    {
        showException(exception);
    }
}

void WishlistCRUDGUI::addToWishlist()
{
    auto window = new QWidget;
    auto layout = new QHBoxLayout;
    auto spin_box = new QSpinBox;
    auto button = new QPushButton("Add !");
    spin_box->setRange(1, (int)service.getRepository().getSize());
    spin_box->setMinimumWidth(50);
    spin_box->setAlignment(Qt::AlignCenter);

    layout->addWidget(spin_box);
    layout->addWidget(button);

    window->setLayout(layout);
    window->setMinimumSize(250, 70);
    window->adjustSize();

    window->setWindowTitle("Add to wishlist");
    window->show();
    QObject::connect(button, &QPushButton::clicked,
                     [&, spin_box](){

                         try
                         {
                             service.addToWishlist(spin_box->text().toStdString());
                         }
                         catch (Exception& exception)
                         {
                             showException(exception);
                         }

                         //showWishlist(service.getWishlist());
                         this->update();
                     });
}

void WishlistCRUDGUI::generateWishlist()
{
    auto window = new QWidget;
    auto layout = new QHBoxLayout;
    auto spin_box = new QSpinBox;
    auto button = new QPushButton("Generate !");
    spin_box->setRange(1, (int)service.getRepository().getSize());
    spin_box->setMinimumWidth(50);
    spin_box->setAlignment(Qt::AlignCenter);

    layout->addWidget(spin_box);
    layout->addWidget(button);

    window->setLayout(layout);
    window->setMinimumSize(250, 70);
    window->adjustSize();

    window->setWindowTitle("Generate wishlist");
    window->show();
    QObject::connect(button, &QPushButton::clicked,
                     [&, spin_box](){
                        try
                        {
                            service.generateWishlist(spin_box->text().toStdString());
                        }
                        catch (Exception& exception)
                        {
                            showException(exception);
                        }

                        //showWishlist(service.getWishlist());
                         this->update();
    });
}

WishlistCRUDGUI::~WishlistCRUDGUI()
{
    service.getWishlist().removeObserver(this);
}

// WISHLIST READ ONLY

WishlistROGUI::WishlistROGUI(Controller &controller) :
        service{controller}{}

void WishlistROGUI::run()
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setWindowTitle("Wishlist Read Only");
    //this->adjustSize();
    this->setMinimumSize(width, height);
    this->show();
}

void WishlistROGUI::paintEvent(QPaintEvent *)
{
    QPainter painter{this};

    for(int i = 0; i < (int)service.getWishlist().getSize(); i++)
    {
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> distx(1, this->window()->width() - 51);
        std::uniform_int_distribution<> disty(1, this->window()->height() - 101);
        //std::uniform_int_distribution<> distrot(0, 360);

        int x = distx(mt);
        int y = disty(mt);
        //int rot = distrot(mt);

        QPolygonF polygon;
        polygon << QPointF(x+1.0, y+1.0)<<QPointF(x+1, y+100)
                <<QPointF(x+50, y+100)<<QPointF(x+1.0, y+1.0);

        QPainterPath path;
        path.addPolygon(polygon);

        QLinearGradient gradient(QPointF(x+1.0, y+1.0),
                                 QPointF(x+50, y+100));

        QColor color;
        color.setNamedColor(QString("#FF5F6D"));
        gradient.setColorAt(0, color);

        QColor color2;
        color2.setNamedColor(QString("#FFC371"));
        gradient.setColorAt(1, color2);

        QBrush brush(gradient);

        painter.fillPath(path, brush);
    }
}

WishlistROGUI::~WishlistROGUI()
{
    service.getWishlist().removeObserver(this);
}