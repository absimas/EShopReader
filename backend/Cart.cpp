#include "Cart.h"
#include <utility>
#include <stdexcept>
#include <functional>
#include "helper.h"

using namespace std;

Cart::Cart(Cart::CartItem item) {
	requestItem(item);
}

Cart::Cart(vector<Cart::CartItem> items) {
	requestItems(items);
}

Cart::~Cart() {
	for (CartItem *item : items) delete item;
}

void Cart::requestItem(Cart::CartItem cartItem) {
	 int needed = cartItem.itemAmount;
	 ItemPtr item = cartItem.item;
	// Insert item to map
	 CartItem *newItem = new CartItem(item, 0);
    pair<set<CartItem*>::iterator, bool> result = add(newItem);
    // Use the inserted (or already present) item from the set
    newItem = *(result.first);

	 int stock = item->getQuantity();
	if (needed <= stock) {
	    // Stock is sufficent, decrease normally
		item->setQuantity(stock - needed);
		  newItem->itemAmount = newItem->itemAmount + needed;
    } else {
        // Stock is insufficent
        cerr << "Warning! Customer tried to fetch more items than there exists" << endl;
		  newItem->itemAmount = newItem->itemAmount + stock;
		  item->setQuantity(0);
	}
}

void Cart::requestItems(vector<Cart::CartItem> items) {
	for (auto &item : items) requestItem(item);
}

pair<set<Cart::CartItem*>::iterator, bool> Cart::add(CartItem *item) {
	return items.insert(item);
}

Cart::CartItem* Cart::getItem(unsigned int index) const {
	 if (index >= getItemCount()) {
        throw invalid_argument("Wrong index (" + to_string(index) + ")");
    }
    auto it = items.begin();
    advance(it, index);

    return *it;
}

unsigned int Cart::getItemCount() const {
	return (unsigned int) items.size();
}

string Cart::toString() const {
   return "Order has " + std::to_string(getItemCount()) + " items.";
}
