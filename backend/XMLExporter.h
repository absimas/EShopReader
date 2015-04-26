#ifndef XMLExporter_H
#define XMLExporter_H

#include "Parser.h"
#include "Item.h"
#include <boost/property_tree/ptree.hpp>

class XMLExporter : public Parser {
private:
	 boost::property_tree::ptree xml;
    std::string filename;

public:
	 XMLExporter(std::vector<ItemPtr> &items, std::string filename);

public:
	 void saveFile();
	 void parse(); // implicitly virtual
	 void parseItem(boost::property_tree::basic_ptree<std::basic_string<char>, std::basic_string<char> > &item);
	 std::string toString() const; // implicitly virtual

private:
	 void downloadFile(std::string url);
	 void setFilename(std::string filename);
	 std::string getFilename() const;
};

#endif // XMLExporter_H
