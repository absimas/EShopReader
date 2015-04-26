#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "Parser.h"
#include <boost/property_tree/ptree.hpp>
#include "Item.h"

class JSONImporter : public Parser {
	 std::string filename;
protected:
    boost::property_tree::ptree json;

public:
	 JSONImporter(std::vector<ItemPtr> &items, std::string filename);
	 JSONImporter(std::vector<ItemPtr> &items);

public:
    void openFile(std::string filename);
    void parse(); // implicitly virtual
	 void parseItem(boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char> > &item);
    std::string toString() const; // implicitly virtual
	 std::string getFilename() const;

private:
    void setFilename(std::string filename);

friend class JSONImporterTest;
};

#endif // JSONPARSER_H
