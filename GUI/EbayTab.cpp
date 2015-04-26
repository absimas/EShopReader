#include "EbayTab.h"
#include "FadingGridItem.h"
#include <QDebug>
#include <QGridLayout>
#include <backend/XMLImporter.h>
#include <algorithm>

using namespace std;

EbayTab::EbayTab(const string &name, const string &xmlSrc, TabHost *tabHost)
		: CustomTab(name, tabHost), Shop(name) {
	XMLImporter importer(items, xmlSrc);
	shuffle(items.begin(), items.end(), default_random_engine((unsigned int)time(0)));
	addItemContainers();
	fetchImages();
}

EbayTab::~EbayTab() {

}

void EbayTab::updateItems() {
	if (!cartTab) return;
	QGridLayout *gridLayout = (QGridLayout*) widget()->layout();
	for (int i=0; i<gridLayout->count(); ++i) {
		FadingGridItem *item = (FadingGridItem*) gridLayout->itemAt(i)->widget();
		if (!item->hasEffects() && !item->isInCart()) {
			item->setEffects();
			item->animationStep(FadingGridItem::INITIAL_OPACITY);
		}
	}
}

void EbayTab::addItemContainers() {
	QWidget *gridWidget = new QWidget(this);
	QGridLayout *gridLayout = new QGridLayout(this);

    const int rows = height() / (double) FadingGridItem::HEIGHT;
	const int columns = width() / (double)FadingGridItem::WIDTH;

    #ifndef QT_NO_DEBUG
		qDebug() << "Initial tab W:" << width() << "H:" << height();
		qDebug() << "Item Rows:" << rows << "Cols:" << columns;
	#endif

	// Add items
	int row=-1;
	for (unsigned int i=0; i<items.size(); i++) {
		// Inc row if cols filled
		if (i % columns == 0) row++;

		int col = i % columns;

		FadingGridItem *item = new FadingGridItem(items[i], this);
//		item->setImageHint(to_string(i));
		if (item->isInCart()) {
			item->clearEffects();
		}

		gridLayout->addWidget(item, row, col);
	}
	gridWidget->setLayout(gridLayout);
	setWidget(gridWidget);
}
