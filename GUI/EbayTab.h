#ifndef SHOPTAB_H
#define SHOPTAB_H

#include "backend/Shop.h"
#include "CustomTab.h"
#include "TabHost.h"
#include "CartTab.h"

extern CartTab *cartTab;

class EbayTab : public CustomTab, public Shop {
	Q_OBJECT
public:
	explicit EbayTab(const std::string &name, const std::string &xmlSrc, TabHost *tabHost);
	virtual ~EbayTab();
	void addItemContainers();

private:
	void updateItems();

friend class TabHost;
};

#endif // SHOPTAB_H
