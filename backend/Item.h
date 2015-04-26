#ifndef ITEM_H
#define ITEM_H

#include <boost/shared_ptr.hpp>

class Item {
    static long itemCount; // not to be used as an object counter
	 std::string name, category, imageUrl, url;
    long id;
    float price;
    int quantity;

public:
	 Item(const std::string &name, float price, int quantity, long id, const std::string url = "",
				const std::string imageUrl = "", const std::string &category = "");
	 virtual ~Item();

private:
	 Item(const Item &item);
	 Item& operator=(const Item &item);

public:
	 bool operator==(const Item &item) const;
    std::string getName() const;
    std::string getCategory() const;
    void decreaseStorage(int byAmount);
    float getPrice() const;
    int getQuantity() const;
    long getId() const;
	 std::string getImageUrl() const;
	 std::string getUrl() const;
    std::string toString() const;

private:
    void setName(const std::string &name);
    void setCategory(const std::string &category);
    void setPrice(float price);
    void setQuantity(int quantity);
    void setId(long id);
	 void setImageUrl(std::string url);
	 void setUrl(std::string url);

	 friend class Cart;
};

typedef boost::shared_ptr<Item> ItemPtr;

// ItemPtr comparator
inline bool operator==(const ItemPtr &item1, const ItemPtr &item2) {
	return item1->getId() == item2->getId();
}

inline std::ostream& operator<<(std::ostream& o, const ItemPtr &item) {
	 return o << item->toString();
}

#endif // ITEM_H
