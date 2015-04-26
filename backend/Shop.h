#ifndef SHOP_H
#define SHOP_H

#include "Item.h"

class Shop {
    std::string name;
    // Objects not copyable
    Shop(const Shop&) { assert(false); }
    Shop& operator=(const Shop&) { assert(false); return *this; }
public:
    std::vector<ItemPtr> items;

public:
    Shop(const std::string &name);
    virtual ~Shop();

public:
    std::string getName() const;
    void setName(const std::string &name);
    void listItems() const;
    std::string toString() const;

    template<class Compare>
    void sortItems(Compare cmp) {
        sort(items.begin(), items.end(), cmp);
    }

    template<class Compare>
    ItemPtr findItem(Compare cmp) const {
        std::vector<ItemPtr>::const_iterator result = find_if(items.begin(), items.end(), cmp);
        // If not found return uninitialized (NULL) smart_ptr
        return ((result == items.end()) ? ItemPtr() : *result);
    }

    template<class SortCompare, class SearchCompare>
    std::vector<ItemPtr> findItems(SortCompare sortCmp, SearchCompare searchCmp, unsigned int amount) {
        if (!is_sorted(items.begin(), items.end(), sortCmp)) {
            std::cout << "Not sorted, sorting..." << std::endl;
            sortItems(sortCmp);
        }

        std::vector<ItemPtr> results;
        std::vector<ItemPtr>::const_iterator it = find_if(items.begin(), items.end(), searchCmp);
        while (results.size() != amount) {
            if (it == items.end()) break;
            results.push_back(*it);
            it++;
        }
        return results;
    }
};

inline std::ostream& operator<<(std::ostream& o, const Shop *shop) {
    return o << shop->toString();
}

#endif // SHOP_H
