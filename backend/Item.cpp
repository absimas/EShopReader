#include "Item.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include "helper.h"

using namespace std;

long Item::itemCount = 0;

Item::Item(const string &name, float price, int quantity, long id, const string url,
					  const string imageUrl, const string &category) {
	setName(name);
	setCategory(category);
	setPrice(price);
	setQuantity(quantity);
	setId(id);
	setUrl(url);
	setImageUrl(imageUrl);
}

Item::~Item() {
   // d-tor
}

Item::Item(const Item&) {
    assert(false);
}

Item& Item::operator=(const Item &) {
	 assert(false);
}

string Item::getName() const {
	return name;
}

string Item::getCategory() const {
	return category;
}

long Item::getId() const {
	return id;
}

float Item::getPrice() const {
	return price;
}

int Item::getQuantity() const {
	return quantity;
}

string Item::getImageUrl() const {
	return imageUrl;
}

string Item::getUrl() const {
	return url;
}

void Item::setQuantity(int quantity) {
	 if (quantity < 0) throw invalid_argument("Item quantity too low: " + to_string(quantity));
	this->quantity = quantity;
}

void Item::setName(const string &name) {
	if (name.empty()) throw invalid_argument("Item name cannot be empty.");
	this->name = name;
}

void Item::setCategory(const string &category) {
	this->category = category;
}

void Item::setPrice(float price) {
	if (price <= 0) throw invalid_argument("Item price too low: " + to_string(price));
	this->price = price;
}

void Item::setId(long id) {
	if (id <= 0) throw invalid_argument("Item price too low: " + to_string(id));
	this->id = id;
}

void Item::setImageUrl(string url) {
	imageUrl = url;
}

void Item::setUrl(string url) {
	this->url = url;
}

string Item::toString() const {
    stringstream ss;
    ss.setf(ios_base::fixed, ios_base::floatfield);
    ss.precision(2);
    ss << "Item: [Name: " << getName();
    if (!getCategory().empty()) {
        ss << ", Category: " << getCategory();
    }
    ss << ", Price: " << getPrice() << ", Quantity: " << getQuantity() << " ID: " << getId() << "]";
    return ss.str();
}
