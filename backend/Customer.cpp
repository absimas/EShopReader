#include "Customer.h"
#include "helper.h"
#include <time.h>
#include <cstdlib>
#include <stdexcept>

using namespace std;

int Customer::customerCount = 0;

Customer::Customer(const string &name, const string &surname) {
	setName(name);
	setSurname(surname);
	generatePin();
	++customerCount;
}

Customer::Customer() {
	generatePin();
	++customerCount;
}

Customer::~Customer() {
	--customerCount;
}

void Customer::generatePin() {
	// Generate random pin-code (minPin-maxPin)
	srand((unsigned int)time(NULL));
	pincode = rand() % maxPin + minPin;
}

void Customer::setName(const string &name) {
	if (name.empty()) throw invalid_argument("Customer's name cannot be empty.");
	this->name = name;
};

void Customer::setSurname(const string &surname) {
	if (surname.empty()) throw invalid_argument("Customer's surname cannot be empty.");
	this->surname = surname;
}

int Customer::getCount() {
	return customerCount;
}

string Customer::getName() const {
	return name;
};

string Customer::getSurname() const {
	return surname;
}

void Customer::placeOrder(Cart &order) {
	orders.push_back(&order);
}

unsigned int Customer::getOrdersCount() const {
	return (unsigned int) orders.size();
}

Cart* Customer::getOrder(unsigned int index) {
	if (index >= getOrdersCount()) {
		throw invalid_argument("Wrong index (" + to_string(index) + ")");
	}
	return orders[index];
}

bool Customer::removeOrder(unsigned int index) {
	if (index >= getOrdersCount()) {
		throw invalid_argument("Wrong index (" + to_string(index) + ")");
	}
	orders.erase(orders.begin() + index);

	return true;
}

string Customer::toString() const {
	return "Customer: [Name: " + getName() + ", surname: " + getSurname() + ", orders: " +
			to_string(orders.size()) + "]";
}
