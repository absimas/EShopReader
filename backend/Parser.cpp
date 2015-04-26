#include "Parser.h"
#include "Shop.h"
#include <QDebug>


using namespace std;

Parser::Parser(vector<ItemPtr> &items) : items(items) {

}

Parser::~Parser() {

}

Parser& Parser::operator=(const Parser &) {
    assert(false);
}

void Parser::addItem(ItemPtr &item) {
	 items.push_back(item);
}

bool Parser::isParsed() const {
    return parsed;
}

void Parser::setParsed(bool parsed) {
    this->parsed = parsed;
}
