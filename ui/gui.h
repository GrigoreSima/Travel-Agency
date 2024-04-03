//
// Created by Grigore Sima on 16.05.2023.
//

#ifndef TA_GUI_H
#define TA_GUI_H

#include "./service/controller.h"

#include <set>

#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QLayout>
#include <QMainWindow>
#include <QListWidget>
#include <QDockWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QComboBox>
#include <QMenuBar>
#include <QSpinBox>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>

class GUI : public QWidget
{
//Q_OBJECT
private:
    Controller &service;

    QMainWindow *main_window;
    QDockWidget *dynamic_buttons;
    QDockWidget *upper_dock;

    int height = 300, width = 700;

public:
    explicit GUI(Controller &);
    //~GUI() override;

    void run();

    void initMenu();
    void initMainWindow();
    void initUpperDock();
    void initDynamicButtons();

//public slots:
    void add(RepositoryAbstract &);

    void remove(const string &);

    void modify(const int&, const int&, const string &);

    void filterAndSearch();

    void sort(const int&);

    void raport();

    void showOffers(RepositoryAbstract &);

    static void showException(Exception &);

    void undo();

    void clearWishlist();
    void addToWishlist();
    void generateWishlist();
    void exportWishlist();

    void clearLayout(QLayout *layout) {
        if (layout == nullptr)
            return;
        QLayoutItem *item;
        while((item = layout->takeAt(0))) {
            if (item->layout()) {
                clearLayout(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    }
};

class TableModel : public QAbstractTableModel
{
private:
    Controller& service;
public:
    explicit TableModel(QWidget*, Controller&);

    // void setVector(std::vector<Offer>&);

    void changed();

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &, int) const override;
    [[nodiscard]] QVariant headerData(int, Qt::Orientation, int) const override;
};

class WishlistCRUDGUI : public QWidget, public Observer
{
//Q_OBJECT
private:
    Controller &service;

    QMainWindow *wishlist_window;
    QDockWidget* buttons;
    int height = 320, width = 580;

public:

    explicit WishlistCRUDGUI(Controller &);

    void run();

    void initMainWindow();
    void initButtons();

    void showWishlist(Wishlist&);
    void clearWishlist();
    void addToWishlist();
    void generateWishlist();

    static void showException(Exception &);

    void update() override
    {
        //showWishlist(service.getWishlist());
        auto* table = (QTableView*) wishlist_window->centralWidget();
        auto* model = (TableModel*) table->model();
        model->changed();
        table->update();
        table->viewport()->update();
        QWidget::update();
    };

    void clearLayout(QLayout *layout) {
        if (layout == nullptr)
            return;
        QLayoutItem *item;
        while((item = layout->takeAt(0))) {
            if (item->layout()) {
                clearLayout(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    }

    ~WishlistCRUDGUI() override;
};

class WishlistROGUI : public QWidget, public Observer // READ ONLY
{
//Q_OBJECT
private:
    Controller &service;

    int height = 250, width = 450;
public:

    explicit WishlistROGUI(Controller &);

    void run();

    void paintEvent(QPaintEvent* event) override;

    void update() override
    {
        this->repaint();
        QWidget::update();
    };

    ~WishlistROGUI() override;
};

#endif //TA_GUI_H
