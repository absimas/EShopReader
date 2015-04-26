#ifndef CARTGRIDITEM_H
#define CARTGRIDITEM_H

#include "CartTab.h"
#include "GridItem.h"
#include "backend/Item.h"
#include <QPoint>

class CartGridItem : public GridItem {
	Q_OBJECT
public:
	explicit CartGridItem(ItemPtr &item, CartTab *cartTab);

private:
	void removeFromCart();

signals:

private slots:
	void showContextMenu(const QPoint &pos);
};

#endif // CARTGRIDITEM_H
