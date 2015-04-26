#include "Shop.h"
#include <cassert>
#include <functional>
#include <algorithm>
#include "helper.h"


using namespace std;

Shop::Shop(const string &name) {
	setName(name);
}

Shop::~Shop() {
//   clearItems();
}

void Shop::listItems() const {
    for (ItemPtr item : items) cout << item << endl;
    cout << "Item count: " << items.size() << endl;
}

string Shop::getName() const {
	return name;
}

void Shop::setName(const string &name) {
	if (name.empty()) throw invalid_argument("Shop name cannot be empty.");
	this->name = name;
}

string Shop::toString() const {
	return "Shop: [Name: " + getName() + ", ItemCount: "+ to_string(items.size()) + "]";
}
