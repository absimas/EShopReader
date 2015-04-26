#ifndef CARTTAB_H
#define CARTTAB_H

#include "CustomTab.h"
#include "backend/Item.h"

class TabHost;
class CartTab : public CustomTab {
public:
    bool saved = true;
    std::vector<ItemPtr> cart;
    bool updated = true;
    static const std::string NAME;
    static const std::string FILENAME;

public:
    explicit CartTab(TabHost *tabHost);
    virtual ~CartTab();

public:
    void setUpdated(bool updated);
    void setSaved(bool saved);
    void save();
    void addItemContainers();
};

#endif // CARTTAB_H
