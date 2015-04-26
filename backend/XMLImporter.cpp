#include "XMLImporter.h"
#include "Item.h"
#include "helper.h"
#include "Shop.h"
#include <cstdlib>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <chrono>

using namespace std;

XMLImporter::XMLImporter(vector<ItemPtr> &items, string filename) : Parser(items) {
    #ifndef QT_NO_DEBUG
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    #endif

    openFile(filename);
    parse();

    #ifndef QT_NO_DEBUG
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        stringstream ss;
        ss << "Opening and parsing the " << filename <<" took " << duration << " microseconds";
        log_err(ss.str());
    #endif
}

void XMLImporter::openFile(string filename) {
    setParsed(false);
    setFilename(filename);
    boost::property_tree::read_xml(filename, xml);
}

void XMLImporter::parse() {
    // Get root array
    boost::property_tree::ptree &dailies = xml.get_child("EbayDailyDeals");

    // Not all xmls have this part (e.g. saved cart xml)
    try {
        BOOST_FOREACH(boost::property_tree::ptree::value_type &item, dailies.get_child("")) {
            // Look for Item's
            if (boost::iequals(item.first, "Item")) {
                parseItem(item.second);
            }
        }
    } catch (...) {

    }

    // Loop sections
    BOOST_FOREACH(boost::property_tree::ptree::value_type &section, dailies.get_child("MoreDeals")) {
        // Loop section children
        BOOST_FOREACH(boost::property_tree::ptree::value_type &item, section.second.get_child("")) {
            // Look for Item's
            if (boost::iequals(item.first, "Item")) {
                parseItem(item.second);
            }
        }
    }
    setParsed(true);
}

void XMLImporter::parseItem(boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char>> &item) {
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

void XMLImporter::setFilename(string filename) {
    this->filename = filename;
}

string XMLImporter::getFilename() const {
    return filename;
}

string XMLImporter::toString() const {
    stringstream ss;
    ss << boolalpha << isParsed();
    return "Importer: [Type: XML, File: " + getFilename() + ", Import done: " + ss.str() + "]";
}
