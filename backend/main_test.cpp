#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include "helper.h"
#include "Customer.h"
#include "Cart.h"
#include "Item.h"
#include "Shop.h"
#include "Parser.h"
#include "JSONImporter.h"
#include "XMLImporter.h"

using namespace std;

string failString = "FAILED";
string finishString = "finished";
bool failed = false;

void replaceAll(string &where, const string &what, const string &with) {
    for(size_t pos = 0; ; pos += with.length()) {
        // Locate the substring to replace
        pos = where.find(what, pos);
        if(pos == string::npos) break;
        // Replace by erasing and inserting
        where.erase(pos, what.length());
        where.insert(pos, with);
    }
}

void fail(string msg) {
    failed = true;
    replaceAll(msg, failString, "\033[31m" + failString + "\033[0m");
    cout << msg << endl;
    log_err(msg);
}

void finish(string msg) {
    if (failed) {
        replaceAll(msg, finishString, "\033[33m" + finishString + "\033[0m");
    } else {
        replaceAll(msg, finishString, "\033[32m" + finishString + "\033[0m");
    }
    failed = false;
    log_err(msg);
}

class ItemTest {
public:
    static void constructorTest() {
        // Constants to be used and checked
          const string name = "Name", category = "Category",
                  imageUrl = "http://test.com/image123.png", url = "http://test.com";
        const float price = 100.0f;
        const int stock = 111;
        const long id = 321;

        // Instantiate
          Item item(name, price, stock, id, imageUrl, url, category);

        if (item.getName().compare(name) != 0) {
            fail("Item constructor test, name setter/getter " + failString);
        }
          if (item.getCategory().compare(category) != 0) {
                fail("Item constructor test, category setter/getter " + failString);
          }
          if (item.getImageUrl().compare(imageUrl) != 0) {
                fail("Item constructor test, image URL setter/getter " + failString);
          }
          if (item.getUrl().compare(url) != 0) {
                fail("Item constructor test, URL setter/getter " + failString);
        }
        if (item.getPrice() != price) {
            fail("Item constructor test, price setter/getter " + failString);
        }
        if (item.getQuantity() != stock) {
            fail("Item constructor test, quantity setter/getter " + failString);
        }
    }

    static void constructorBadArgTest() {
        try {
            Item item("", 10.0f, 1, 123);
            // This shouldn't be reached
            fail("Item empty name test " + failString);
        } catch (invalid_argument &e) {}
        try {
            Item item("testItem", -10.0f, 1, 123);
            // This shouldn't be reached
            fail("Item negative price test FAILED.");
        } catch (invalid_argument &e) {}
        try {
            Item item("testItem", 10.0f, -1, 123);
            // This shouldn't be reached
            fail("Item negative amount test FAILED.");
        } catch (invalid_argument &e) {}
    }
};

class OrderTest {
public:
    static void constructorTest() {
        const unsigned int multipleItemCount = 15, amount = 3;
        const long baseId = 1000;
        const float price = 100.0f;
        // These were tested before (will work)
        ItemPtr itemPtr(new Item("testItem", price, amount, baseId));
        // Create an order with a single item
          Cart order(Cart::CartItem(itemPtr, amount));
        if (order.getItemCount() != 1) {
            fail("Single item Order test " + failString);
        }

        // Create multiple items
        vector<Cart::CartItem> items;
        for (unsigned int i=0; i<multipleItemCount; i++) {
                ItemPtr itemPtr(new Item("testItem" + to_string(i), 100.0f, amount, baseId + i));
                items.push_back(Cart::CartItem(itemPtr, amount));
        }

        // Create an order with multiple items
          Cart order2(items);
        // Check if the item count matches in the array, the constant and the order
        if (order2.getItemCount() != multipleItemCount || items.size() != order2.getItemCount()) {
            fail("Multiple item Order test " + failString);
        }
    }
};

class CustomerTest {
public:
    static void constructorTest() {
        const int customerCountBefore = Customer::getCount();
        // Create a new customer
        const string name = "Customer", surname = "Last Name";
        Customer customer(name, surname);

        if (customer.getName().compare(name) != 0) {
            fail("Customer constructor test, name setter/getter " + failString);
        }
        if (customer.getSurname().compare(surname) != 0) {
            fail("Customer constructor test, name setter/getter " + failString);
        }
        if (Customer::getCount() != customerCountBefore+1) {
            fail("Customer constructor test, customer count incrementation " + failString);
        }
    }

    static void orderPlacementTest() {
        Customer customer("name", "surname");
          ItemPtr itemPtr(new Item("testItem", 100.0f, 1, 123));
          Cart order(Cart::CartItem(itemPtr, 1));
        customer.placeOrder(order);
        customer.placeOrder(order);

        try {
            customer.getOrder(1);
        } catch (invalid_argument &e) {
            fail("Customer order retrieval test " + failString);
        }
        try {
            customer.getOrder(2);
            // This shouldn't  be reached
            fail("Customer order retrieval test succeeded... Should have " + failString);
        } catch (invalid_argument &e) {
            // This should be caught
        } catch (exception &e) {
            stringstream ss;
            ss << "Customer order retrieval test FAILED, unknown exception:" << endl << e.what();
            fail(ss.str());
        }
    }

    static void orderRemovalTest() {
        // These were tested before
        Customer customer("name", "surname");
          ItemPtr itemPtr(new Item("testItem", 100.0f, 3, 123));
          Cart order1(Cart::CartItem(itemPtr, 1));
          Cart order2(Cart::CartItem(itemPtr, 2));
        customer.placeOrder(order1);
        customer.placeOrder(order2);
        // Fetch the orders from the vector
        Cart *firstOrder = customer.getOrder(0);
        Cart *secondOrder = customer.getOrder(1);
        if (firstOrder != &order1 || secondOrder != &order2) {
            failed = true;
            fail("Order addresses changed after being put to a vector. Test " + failString);
        }
        // Remove first order and check if successful
        customer.removeOrder(0);
        if (customer.getOrdersCount() != 1) {
            failed = true;
            fail("Customer order cancel test, removing order " + failString);
        }
        if (customer.getOrder(0) != &order2) {
            failed = true;
            fail("Second order's address changed after first one was deleted. Test " + failString);
        }
    }

    static void orderUpdateTest() {
        const int totalItems = 10;
        int itemsLeft = totalItems;
        // These were tested before
        Customer customer("name", "surname");
          ItemPtr itemPtr(new Item("testItem", 100.0f, totalItems, 123));
          Cart order(Cart::CartItem(itemPtr, 1));
        --itemsLeft; // 9
        customer.placeOrder(order);
        // Fetch the first order from the vector
        Cart *firstOrder = customer.getOrder(0);

        // Add more items, (with the same item)
        firstOrder->requestItem(Cart::CartItem(itemPtr, 1));
        --itemsLeft; // 8
        vector<Cart::CartItem> items;
        while (itemsLeft-- != 0) {
                items.push_back(Cart::CartItem(itemPtr, 1));
        } // 0
        firstOrder->requestItems(items);

        // Check if successfully added
        if (firstOrder->getItem(0)->itemAmount != totalItems) {
            fail("Adding more items to an existing item in an order " + failString + "! Item has " +
                    to_string(firstOrder->getItem(0)->itemAmount) + " item(s)");
        }
    }
};

class ShopTest {
public:
    static void constructorTest() {
        const string name = "My Shop";
        Shop shop(name);
        if (shop.getName() != name) {
            fail("Shop constructor test, setter/getter " + failString);
        }
    }
};

class JSONImporterTest {
public:
    static void constructorTest() {
         const string filename = "data.json";
        FILE *file = fopen(filename.c_str(), "r");
        if (!file) {
            fail("Opening JSON file \"" + filename + "\" " + failString);
            return;
        } else {
            fclose(file);
        }

        Shop shop("testShop");

          JSONImporter importer(shop.items, filename);
          if (shop.items != importer.items) {
                fail("Setting the json importer's shop " + failString);
        }
          if (filename != importer.getFilename()) {
                fail("Setting the json importer's filename " + failString);
        }
          if (!importer.isParsed()) {
            fail("Parsing the JSON file " + failString);
        }
        if (shop.items.size() <= 1) {
            fail("Consider enabling DEBUG definition as parsing the items " + failString);
        }
    }
};

int main5() {
    // Item tests
    log_err("Item tests started.");
    ItemTest::constructorTest();
    ItemTest::constructorBadArgTest();
    finish("Item tests " + finishString);

    // Order tests
    log_err("Order tests started.");
    OrderTest::constructorTest();
    finish("Order tests " + finishString);

    // Customer tests
    log_err("Customer tests started.");
    CustomerTest::constructorTest();
    CustomerTest::orderPlacementTest();
    CustomerTest::orderRemovalTest();
    CustomerTest::orderUpdateTest();
    finish("Customer tests " + finishString);

    // Shop tests
    log_err("Shop tests started.");
    ShopTest::constructorTest();
    finish("Shop tests " + finishString);

    // JSONImporter tests
    log_err("JSONImporter tests started.");
    JSONImporterTest::constructorTest();
    finish("JSONImporter tests " + finishString);

    return 0;
}
