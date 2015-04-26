#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Cart.h"

class Customer {
    static const int minPin = 1000;
    static const int maxPin = 9999;
    static int customerCount;
    std::string name;
    std::string surname;
    int pincode;
    std::vector<Cart*> orders;

public:
    Customer(const std::string &name, const std::string &surname);
    Customer();
    virtual ~Customer();
private:
    Customer(const Customer& customer);

public:
	 void placeOrder(Cart &order);
    void setName(const std::string &name);
    void setSurname(const std::string &surname);

    static int getCount();
    unsigned int getOrdersCount() const;
    std::string getName() const;
    std::string getSurname() const;
    Cart* getOrder(unsigned int index);
    bool removeOrder(unsigned int index);
    std::string toString() const;
	 std::string getItemName(unsigned int index) const;

private:
    void generatePin();
};

inline std::ostream& operator<<(std::ostream& o, const Customer &customer) {
    return o << customer.toString();
}

#endif // CUSTOMER_H
