#include "CustomTab.h"
#include "GridItem.h"
#include "TabHost.h"
#include <QGridLayout>
#include <cassert>
#include <QScrollBar>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QScrollBar>

using namespace std;

CustomTab::CustomTab(const string &name, TabHost *tabHost) : QScrollArea(tabHost) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabHost->insertTab(0, this, QString::fromStdString(name));
    // Switch to this tab
    tabHost->setCurrentIndex(tabHost->indexOf(this));
}

CustomTab::~CustomTab() {

}

void CustomTab::fetchImages() {
    if (REQUIRED_VISIBILITY == 0.0) {
        fetchImagesInRange(0, ((QGridLayout*)widget()->layout())->rowCount());
    } else {
        pair<int, int> visibleRows = this->visibleRows();
        fetchImagesInRange(visibleRows.first, visibleRows.second);
    }
}

void CustomTab::wheelEvent(QWheelEvent *event) {
    QScrollArea::wheelEvent(event);

    // Don't fetch images when scrolling too fast
    const long double lastTime = lastScrollTime + IMAGE_FETCH_DELAY;
    const long double curTime = time(0)*1000;
    lastScrollTime = curTime;
    if (lastTime > curTime) return;

    // Don't fetch images when they're fetched automatically, in order
    if (REQUIRED_VISIBILITY == 0.0) return;

    // Check if new rows have been revealed
    pair<int, int> visibleRows = this->visibleRows();
    if (visibleRows.second > lastVisibleRow) {
        fetchImagesInRange(visibleRows.first, visibleRows.second);
    } else if (visibleRows.first < firstVisibleRow) {
        fetchImagesInRange(visibleRows.first, visibleRows.second);
    }

    firstVisibleRow = visibleRows.first;
    lastVisibleRow = visibleRows.second;
}

pair<int, int> CustomTab::visibleRows() {
    assert(0.0 < REQUIRED_VISIBILITY);

    QGridLayout *gridLayout = (QGridLayout*)widget()->layout();
    int rowHeight = (double)GridItem::HEIGHT + gridLayout->verticalSpacing();

    int topY = verticalScrollBar()->sliderPosition();
    int bottomY = verticalScrollBar()->sliderPosition() + height();

    int topRow = (topY + GridItem::HEIGHT*REQUIRED_VISIBILITY) / (double)rowHeight;
    int bottomRow = (bottomY - GridItem::HEIGHT*REQUIRED_VISIBILITY) / (double)rowHeight;

    return make_pair(topRow, bottomRow);
}

int a = 0;

void CustomTab::fetchImagesInRange(int first, int last) {
    QGridLayout *gridLayout = (QGridLayout*) widget()->layout();

    for (int row=first; row<=last; ++row) {
        for (int col=0; col<gridLayout->columnCount(); ++col) {
            // Check if there are enough columns on this row
            if (row*gridLayout->columnCount() + col >= gridLayout->count()) break;

            QLayoutItem *layoutItem = gridLayout->itemAtPosition(row, col);
            GridItem *item = (GridItem*) layoutItem->widget();

            // Check if image is set
            if (item->getImage()) continue;

            // Check if this image has already been fetched
            const int itemIndex = row * gridLayout->columnCount() + col;
            if (replies.find(itemIndex) != replies.end()) continue;

            // Make a new request
            QUrl url(QString::fromStdString(item->getImageUrl()));
            QNetworkReply *reply = netManager->get(QNetworkRequest(url));
            replies.insert({itemIndex, reply});
            connect(reply, &QNetworkReply::finished, [reply, item, this, &url, &itemIndex]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QPixmap image;
                    image.loadFromData(reply->readAll());
                    image = image.scaled(GridItem::IMAGE_SIZE - GridItem::IMAGE_PADDING,
                                                GridItem::IMAGE_SIZE - GridItem::IMAGE_PADDING,
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    item->setImage(image);
                } else {
                    qDebug() << "Fetching" << url << "returned error:" << reply->errorString();
                }
                replies.erase(itemIndex);
                reply->deleteLater();
            });
        }
    }
}

void CustomTab::reAddItems(const int newColumnCount) {
    QGridLayout *gridLayout = (QGridLayout*) widget()->layout();

//	const int newColumnCount = gridLayout->columnCount() + 1;
    // Fetch all item widgets
    const int itemCount = gridLayout->count();
    QWidget* items[itemCount];
    for (int i=0; i<gridLayout->count(); ++i) items[i] = gridLayout->itemAt(i)->widget();

    // Create a new tab widget-layout and populate with old items
    QWidget *newWidget = new QWidget(this);
    QGridLayout *newGrid = new QGridLayout(this);
    int row=-1;
    for (int i=0; i<itemCount; ++i) {
        if (i % newColumnCount == 0) ++row;
        int col = i % newColumnCount;
        newGrid->addWidget(items[i], row, col);
    }

    newWidget->setLayout(newGrid);
    setWidget(newWidget);
    fetchImages();
}
