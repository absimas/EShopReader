#include "JSONImporter.h"
#include "Item.h"
#include "helper.h"
#include "Shop.h"
#include <cstdlib>
#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <sstream>
#include <chrono>

using namespace std;

JSONImporter::JSONImporter(vector<ItemPtr> &items, string filename) : Parser(items) {
#ifndef QT_NO_DEBUG
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
#endif

    openFile(filename);
    parse();

#ifndef QT_NO_DEBUG
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
     auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    stringstream ss;
    ss << "Opening and parsing the JSON took " << duration << " microseconds";
    log_err(ss.str());
#endif
}

JSONImporter::JSONImporter(vector<ItemPtr> &items) : Parser(items) {

}

void JSONImporter::openFile(string filename) {
    setParsed(false);
    setFilename(filename);
    boost::property_tree::read_json(filename, json);
}

void JSONImporter::parse() {
//    http://deals.ebay.com/feeds/xml
//    boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char>>
//    boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char> >::value_type

     // Get root array
     auto &dailies = json.get_child("EbayDailyDeals");

    // Loop through first item
    for(auto &item : dailies.get_child("Item")) {
        parseItem(item.second);
    }

    // Fetch 2nd item
    auto &moreDeals = dailies.get_child("MoreDeals");
    // Loop sections
    for (auto &section : moreDeals.get_child("MoreDealsSection")) {
        // Loop item arrays inside of sections
        for(auto &item : section.second.get_child("Item")) {
                parseItem(item.second);
        }
    }

     shuffle(items.begin(), items.end(), default_random_engine((unsigned int)time(0)));
    setParsed(true);
}

void JSONImporter::parseItem(boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char>> &item) {
     string idStr, priceStr, quantityStr, title, pictureUrl, url;
    try {
        idStr = item.get<string>("ItemId");
        priceStr = item.get<string>("ConvertedCurrentPrice");
        quantityStr = item.get<string>("Quantity");
        title = item.get<string>("Title");
          pictureUrl = item.get<string>("PictureURL");
          url = item.get<string>("DealURL");
    } catch (boost::property_tree::ptree_bad_path &e) {
        #ifdef DEBUG
//				log_err(e.what());
        #endif
        return;
    }

     long id = stol(idStr);
    int quantity = stoi(quantityStr);
     float price;
     stringstream ss(priceStr);
     ss >> price;

     ItemPtr itemPtr(new Item(title, price, quantity, id, url, pictureUrl));
    addItem(itemPtr);
}

void JSONImporter::setFilename(string filename) {
    this->filename = filename;
}

string JSONImporter::getFilename() const {
    return filename;
}

string JSONImporter::toString() const {
    stringstream ss;
    ss << boolalpha << isParsed();
    return "Parser: [Type: JSON, File: " + getFilename() + ", Parsing done: " + ss.str() + "]";
}
