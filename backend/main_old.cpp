#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <functional>
#include <algorithm>
#include "helper.h"
#include "Item.h"
#include "Shop.h"
#include "Customer.h"
#include "Cart.h"
#include "Parser.h"
#include "JSONImporter.h"
#include <QApplication>
#include <QTextEdit>

using namespace std;

namespace {
    struct ProductNameMore {
        bool operator()(const ProductPtr product1, const ProductPtr product2) {
            return product1->getName() > product2->getName();
        }
    };

    bool ProductQuantityMore(const ProductPtr product1, const ProductPtr product2) {
        return product1->getQuantity() > product2->getQuantity();
    }

    struct ProductPriceLess {
        bool operator()(const ProductPtr product1, const ProductPtr product2) {
            return product1->getPrice() < product2->getPrice();
        }
    };

    template<class Criteria>
    class ProductPriceEquals {
    Criteria criteria;
    public:
        ProductPriceEquals(Criteria criteria) {
            this->criteria = criteria;
        }

        // Comparator operator
        bool operator()(const ProductPtr product) {
            return product->getPrice() == criteria;
        }
    };

    template<class Criteria>
    class ProductPriceMoreEquals {
    Criteria criteria;
    public:
        ProductPriceMoreEquals(Criteria criteria) {
            this->criteria = criteria;
        }

        // Comparator operator
        bool operator()(const ProductPtr product) {
            return product->getPrice() >= criteria;
        }
    };
}

int main2() {

	return 0;
}
