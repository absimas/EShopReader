#include "CartTab.h"
#include "CartGridItem.h"
#include "TabHost.h"
#include "MainWindow.h"
#include <QDebug>
#include <QGridLayout>
#include <backend/XMLExporter.h>
#include <backend/XMLImporter.h>

using namespace std;

const string CartTab::NAME = "Cart";
const string CartTab::FILENAME = "cart.xml";

CartTab::CartTab(TabHost *tabHost) : CustomTab(NAME, tabHost) {
    try {
        XMLImporter importer(cart, FILENAME);
        addItemContainers();
        fetchImages();
    } catch (...) {
        // If errors encountered while parsing, prevent populating the cart
    }
    // Switch to another tab
    int index = tabHost->indexOf(this) - 1;
    if (index < 0) index += 2;
    tabHost->setCurrentIndex(index);
}

CartTab::~CartTab() {

}

void CartTab::addItemContainers() {
    QWidget *gridWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(this);

    const int rows = height() / (double) CartGridItem::HEIGHT;
    const int columns = width() / (double) CartGridItem::WIDTH;

    #ifndef QT_NO_DEBUG
        qDebug() << "Initial tab W:" << width() << "H:" << height();
        qDebug() << "Item Rows:" << rows << "Cols:" << columns;
    #endif

    // Add items
    int row=-1;
    for (unsigned int i=0; i<cart.size(); i++) {
        // Inc row if cols filled
        if (i % columns == 0) row++;

        int col = i % columns;

        CartGridItem *item = new CartGridItem(cart[i], this);
//		item->setImageHint(to_string(i));

        gridLayout->addWidget(item, row, col);
    }
    gridWidget->setLayout(gridLayout);
    setWidget(gridWidget);
}

void CartTab::save() {
    XMLExporter exporter(cart, FILENAME);
    setSaved(true);
}

void CartTab::setSaved(bool saved) {
    MainWindow *window = (MainWindow*) CartTab::window();
    if (saved) {
        window->actionSave->setEnabled(false);
    } else {
        window->actionSave->setEnabled(true);
    }
    this->saved = saved;
}

void CartTab::setUpdated(bool updated) {
    this->updated = updated;
}
