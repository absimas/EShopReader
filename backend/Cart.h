#ifndef CART_H
#define CART_H

#include "Item.h"
#include <set>
#include <vector>

class Cart {
public:
	 struct CartItem {
		  CartItem(ItemPtr item, int amount) : item(item) {
				itemAmount = amount;
        }
		  ItemPtr item;
		  int itemAmount;
    };

private:
	 Cart(const Cart&) {}
    // Items with unique ids
    struct ItemIdLess {
		  bool operator() (const CartItem *item1, const CartItem *item2) const {
				return item1->item->getId() < item2->item->getId();
        }
    };

	 std::set<CartItem*, ItemIdLess> items;

public:
	 Cart(Cart::CartItem item);
	 Cart(std::vector<Cart::CartItem> items);
	 virtual ~Cart();

public:
	 void requestItem(Cart::CartItem item);
	 void requestItems(std::vector<Cart::CartItem> items);
	 CartItem* getItem(unsigned int id) const;
    unsigned int getItemCount() const;
    std::string toString() const;
private:
    // rename add to insert
	 std::pair<std::set<Cart::CartItem*>::iterator, bool> add(CartItem *item);

};

inline std::ostream& operator<<(std::ostream& o, const Cart *order) {
    return o << order->toString();
}

#endif // CART_H
