#ifndef CUSTOMTAB_H
#define CUSTOMTAB_H

#include <QDebug>
#include <QNetworkReply>
#include <QScrollArea>
#include <QtNetwork/QNetworkReply>
#include <set>

extern QNetworkAccessManager *netManager;
extern float requiredVisibility;
extern int imageFetchDelay;

class CartTab;
extern CartTab *cartTab;

class TabHost;
class CustomTab : public QScrollArea {
	// Fraction of the item's height that needs to be visible for the image to be fetched (0 - load all)
	static constexpr float REQUIRED_VISIBILITY = 0.25;
	// Delay between scroll events which invoke image fetcher (milliseconds)
	static const int IMAGE_FETCH_DELAY = 0;

	int lastVisibleRow, firstVisibleRow;
	long double lastScrollTime;
	Q_OBJECT

public:
	std::map<int, QNetworkReply*> replies;

public:
	explicit CustomTab(const std::string &name, TabHost *tabHost);
	virtual ~CustomTab();

public:
	void fetchImages();
	void wheelEvent(QWheelEvent *event);
	void reAddItems(const int newColumnCount);

protected:
	virtual void addItemContainers() = 0;

private:
	void fetchImagesInRange(int first, int last);
	std::pair<int, int> visibleRows();
	void abortImageFetchNotInRange(int first, int last);
};

#endif // CUSTOMTAB_H
