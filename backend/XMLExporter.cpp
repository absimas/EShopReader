#include "XMLExporter.h"
#include "Item.h"
#include "helper.h"
#include "Shop.h"
#include <cstdlib>
#include <algorithm>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <chrono>

using namespace std;

XMLExporter::XMLExporter(vector<ItemPtr> &items, string filename) : Parser(items) {
    #ifndef QT_NO_DEBUG
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    #endif

    setFilename(filename);
    parse();

    #ifndef QT_NO_DEBUG
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        stringstream ss;
        ss << "Parsing and writing the XML took " << duration << " microseconds";
        log_err(ss.str());
    #endif
}

void XMLExporter::saveFile() {
    boost::property_tree::write_xml(getFilename(), xml);
    setParsed(true);
}

// ToDo update JSONImporter with this too
void XMLExporter::parse() {
    // Create the root array

    for (unsigned int i=0; i<items.size(); ++i) {
        // Create child
        boost::property_tree::ptree item;
        item.add("ItemId", items[i]->getId());
        item.add("ConvertedCurrentPrice", items[i]->getPrice());
        item.add("Quantity", items[i]->getQuantity());
        item.add("Title", items[i]->getName());
        item.add("PictureURL", items[i]->getImageUrl());
        item.add("DealURL", items[i]->getUrl());
        // Add the child
        xml.add_child("EbayDailyDeals.MoreDeals.MoreDealsSection.Item", item);
    }

    saveFile();
}

void XMLExporter::parseItem(boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char>> &item) {
    string idStr, priceStr, quantityStr, title, pictureUrl, url;
    try {
        idStr = item.get<string>("ItemId");
        priceStr = item.get<string>("ConvertedCurrentPrice");
        quantityStr = item.get<string>("Quantity");
        title = item.get<string>("Title");
        pictureUrl = item.get<string>("PictureURL");
        url = item.get<string>("DealURL");
    } catch (boost::property_tree::ptree_bad_path &e) {
        #ifndef QT_NO_DEBUG
//			log_err(e.what());
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

void XMLExporter::setFilename(string filename) {
    this->filename = filename;
}

string XMLExporter::getFilename() const {
    return filename;
}

string XMLExporter::toString() const {
    stringstream ss;
    ss << boolalpha << isParsed();
    return "Exporter: [Type: XML, File: " + getFilename() + ", Export done: " + ss.str() + "]";
}
