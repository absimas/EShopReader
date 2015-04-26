#include "EbayTab.h"
#include "MainWindow.h"
#include "TabHost.h"
#include <QDebug>

TabHost::TabHost(QWidget *parent) : QTabWidget(parent) {

}

void TabHost::tabChanged(int index) {
	CustomTab *tab = (CustomTab*) currentWidget();

    MainWindow* win = (MainWindow*) window();
    if (win->sizeUpdated) {
        qDebug() << "Updated on tab switch";
        win->fitItemsToSize();
        win->sizeUpdated = false;
    }

	if (tabText(index).compare(QString::fromStdString(CartTab::NAME)) == 0) {
        #ifndef QT_NO_DEBUG
			qDebug() << "Switched to cart tab" << index;
		#endif
		CartTab *cartTab = (CartTab*) tab;
		if (!cartTab->updated) {
			cartTab->setUpdated(true);
			cartTab->replies.clear();
			cartTab->addItemContainers();
			cartTab->fetchImages();
		}
	} else {
        #ifndef QT_NO_DEBUG
			qDebug() << "Switched to shop tab" << index;
		#endif
		// Try casting to an EbayTab
		EbayTab* ebayTab = dynamic_cast<EbayTab*>(tab);
		if(ebayTab != 0) {
			ebayTab->updateItems();
		} else {
            #ifndef QT_NO_DEBUG
				qDebug() << "Switched to a tab that is neither a cart nor eBay." << index;
			#endif
		}
	}
}
