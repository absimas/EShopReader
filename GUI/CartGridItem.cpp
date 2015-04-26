#include "CartGridItem.h"
#include <QAction>
#include <QMenu>
#include <algorithm>

using namespace std;

CartGridItem::CartGridItem(ItemPtr &item, CartTab *cartTab) : GridItem(item, cartTab) {
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
              this, SLOT(showContextMenu(const QPoint&)));
}

void CartGridItem::removeFromCart() {
    CartTab *cartTab = (CartTab*) parentTab;
    auto &cart = cartTab->cart;
    // Remove image request
    cartTab->replies.erase(item->getId());
    // Remove item from cart
    cart.erase(std::find(cart.begin(), cart.end(), item));
    // Remove item from grid
    cartTab->widget()->layout()->removeWidget(this);

    // Re-create the layout
    cartTab->reAddItems(((QGridLayout*)cartTab->widget()->layout())->columnCount());
    cartTab->setSaved(false);
}

// Slots
void CartGridItem::showContextMenu(const QPoint &pos) {
    QPoint globalPos = mapToGlobal(pos);
    QMenu menu;

    QString remove("Remove from cart");
    QString cancel("Cancel");

    menu.addAction(remove);
    menu.addAction(cancel);

    QAction *selectedItem = menu.exec(globalPos);
    if (selectedItem) {
        if (selectedItem->text().compare(remove) == 0) {
            removeFromCart();
        } else if (selectedItem->text().compare(cancel) == 0) {
            // Cancel
        }
    }
}
