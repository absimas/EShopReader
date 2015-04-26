#ifndef PARSER_H
#define PARSER_H

#include "Item.h"

class Shop;
class Parser {
// Vars
    bool parsed = false;

protected:
	 std::vector<ItemPtr> &items;

// C-tors
public:
	 Parser(std::vector<ItemPtr> &items);
    virtual ~Parser();

private:
    Parser(const Parser& p);
    Parser& operator=(const Parser &importer);

// Methods
protected:
    void addItem(ItemPtr &item);
    virtual void parse() = 0;
    void setParsed(bool parsed);

public:
    bool isParsed() const;
    virtual std::string toString() const = 0;
    // Turi but pure virtual, nes naujo abstraktaus (Parser) objekto nesukursi
};

inline std::ostream& operator<<(std::ostream& o, const Parser &importer) {
    return o << importer.toString();
}

#endif // PARSER_H
