#include "GridItem.h"
#include <QDebug>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <sstream>
#include <iomanip>
#include <QWheelEvent>
#include <QGraphicsOpacityEffect>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>
#include <QDesktopServices>

using namespace std;

GridItem::GridItem(ItemPtr &item, CustomTab *tab) : QFrame(tab), item(item) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    parentTab = tab;
    initUi();
    setTitle(item->getName());
    setPrice(item->getPrice());
    setImageUrl(item->getImageUrl());
}

void GridItem::initUi() {
    // Item
    ui.setupUi(this);
    setFixedSize(WIDTH, HEIGHT);
    QString style = "border: %1px solid gray; border-radius: %2px; background: #FFF;";
    style = style.arg(QString::number(BORDER_WIDTH), QString::number(BORDER_RADIUS));
    setStyleSheet(style);

    // Image
    imageLabel = findChild<QLabel*>("image");
    imageLabel->setFixedSize(IMAGE_SIZE - BORDER_WIDTH*2, IMAGE_SIZE);

    // Title
    titleLabel = findChild<QLabel*>("title");
    titleLabel->setFixedSize(WIDTH - BORDER_WIDTH*2, ADDITIONAL_HEIGHT / 3.0 * 2.0);

    // Price (uses remainder of height)
    priceLabel = findChild<QLabel*>("price");
    priceLabel->setFixedSize(WIDTH - BORDER_WIDTH*2,
                                     height() - imageLabel->height() - titleLabel->height());
}

GridItem::~GridItem() {

}

bool GridItem::event(QEvent *e) {
    bool result = QFrame::event(e);
    if (e->type() == QEvent::MouseButtonDblClick) {
            QUrl url(QString::fromStdString(item->getUrl()));
            QDesktopServices::openUrl(url);
    }

    return result;
}

void GridItem::setTitle(string title) {
    titleLabel->setText(QString::fromStdString(title));
}

void GridItem::setPrice(float price) {
    stringstream ss;
    ss << "$" << fixed << setprecision(2) << price;
    priceLabel->setText(QString::fromStdString(ss.str()));
}

void GridItem::setImageHint(string hint) {
    imageLabel->setText(QString::fromStdString(hint));
}

void GridItem::setImage(QPixmap image) {
    imageLabel->setPixmap(image);
}

void GridItem::setImageUrl(string imageUrl) {
    this->imageUrl = imageUrl;
}

const QPixmap* GridItem::getImage() const {
    return imageLabel->pixmap();
}

string GridItem::getImageUrl() const {
    return imageUrl;
}
