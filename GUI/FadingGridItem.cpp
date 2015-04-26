#include "FadingGridItem.h"
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
#include <QPropertyAnimation>
#include <algorithm>
#include <vector>


using namespace std;

static FadingGridItem *lastItem;

FadingGridItem::FadingGridItem(ItemPtr &item, CustomTab *tab) : GridItem(item, tab) {
    initUi();
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
              this, SLOT(showContextMenu(const QPoint&)));
}

void FadingGridItem::initUi() {
    // Item hoverable
    setAttribute(Qt::WA_Hover);

    // Cfg the animation for title and price labels
    animation = new LabelAnimation(this);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setDuration(ANIM_DURATION);
    animation->setStartValue(INITIAL_OPACITY);
    animation->setEndValue(FINAL_OPACITY);
    connect(animation, SIGNAL(valueChanged(const QVariant&)),
              this, SLOT(animationStep(const QVariant&)));

    // Set label effects quietly
    setEffects();
    animationStep(INITIAL_OPACITY);
}

bool FadingGridItem::event(QEvent *e) {
    bool result = GridItem::event(e);
    if (window()->isEnabled()) {
        switch(e->type()) {
        case QEvent::Enter: case QEvent::HoverEnter:
            dimPrevious();
            lastItem = this;
            if (!isInCart()) animate(true);
            break;
        case QEvent::HoverLeave:
            if (!isInCart()) animate(false);
            break;
        case QEvent::HoverMove:
            if (!isInCart()) animate(true);
            break;
        case QEvent::Wheel:
            if (!isInCart()) animate(false);
            break;
        default:
            break;
        }
    }

    return result;
}

void FadingGridItem::setEffects() {
    imageLabel->setGraphicsEffect(new QGraphicsOpacityEffect());
    titleLabel->setGraphicsEffect(new QGraphicsOpacityEffect());
    priceLabel->setGraphicsEffect(new QGraphicsOpacityEffect());
}

void FadingGridItem::clearEffects() {
    delete titleLabel->graphicsEffect();
    titleLabel->setGraphicsEffect(NULL);
    delete imageLabel->graphicsEffect();
    imageLabel->setGraphicsEffect(NULL);
    delete priceLabel->graphicsEffect();
    priceLabel->setGraphicsEffect(NULL);
}

bool FadingGridItem::hasEffects() {
    if (!(titleLabel->graphicsEffect()) || !imageLabel->graphicsEffect() ||
            !priceLabel->graphicsEffect()) {
        return false;
    } else {
        return true;
    }
}

/* Animate enter of leave transitions. If forced, will ignore current animation and state */
void FadingGridItem::animate(bool reveal, bool force) {
    if (reveal) {
        if (!force && animation->currentValue() == FINAL_OPACITY &&
                animation->state() == QAbstractAnimation::Stopped) {
            // If not forced, and the implied animation has already been done, just leave
            return;
        }
        animation->setDirection(QPropertyAnimation::Forward);
    } else {
        if (!force && animation->currentValue() == INITIAL_OPACITY &&
                animation->state() == QAbstractAnimation::Stopped) {
            // If not forced, and the implied animation has already been done, just leave
            return;
        }
        animation->setDirection(QPropertyAnimation::Backward);
    }

    animation->start();
}

/* Quietly dim previous item (if any and if it's not a cart item) */
void FadingGridItem::dimPrevious() {
    if (lastItem && lastItem != this && !lastItem->isInCart()) {
        lastItem->animate(false);
    }
}

/* Perform default animation and clear effects */
void FadingGridItem::blinkAndFreeze() {
    animate(true, true);
    int timeLeft = animation->totalDuration() - animation->currentTime();
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, [this, timer]() {
        animation->stop();
        delete timer;
        clearEffects();
    });
    timer->start(timeLeft);
}

void FadingGridItem::addToCart() {
    cartTab->setSaved(false);
    cartTab->setUpdated(false);
    cartTab->cart.push_back(item);
}

void FadingGridItem::removeFromCart() {
    vector<ItemPtr> &cart = cartTab->cart;
    vector<ItemPtr>::iterator it = std::find(cart.begin(), cart.end(), item);
    // Check if item was found
    if (it != cart.end()) {
        cart.erase(it);
        cartTab->setSaved(false);
        cartTab->setUpdated(false);
    }
}

bool FadingGridItem::isInCart() {
    vector<ItemPtr> &cart = cartTab->cart;
    vector<ItemPtr>::iterator it = std::find(cart.begin(), cart.end(), item);
    if (it == cart.end()) {
        return false;
    } else {
        return true;
    }
}

bool FadingGridItem::idIsInCart() {
    vector<ItemPtr> &cart = cartTab->cart;
    vector<ItemPtr>::iterator it = std::find(cart.begin(), cart.end(), item);
    if (it == cart.end()) {
        return false;
    } else {
        return true;
    }
}

// Slots
void FadingGridItem::animationStep(const QVariant &value) {
    if (!hasEffects()) return;
    imageLabel->graphicsEffect()->setProperty("opacity", value);
    titleLabel->graphicsEffect()->setProperty("opacity", value);
    priceLabel->graphicsEffect()->setProperty("opacity", value);
}

void FadingGridItem::showContextMenu(const QPoint &pos) {
    QPoint globalPos = mapToGlobal(pos);
    QMenu menu;

    QString add("Add to cart");
    QString remove("Remove from cart");
    QString cancel("Cancel");

    if (isInCart()) {
        menu.addAction(remove);
    } else {
        // Quietly fade_in the item whose menu has been asked to open
        animate(true);
        dimPrevious();
        menu.addAction(add);
    }
    menu.addAction(cancel);

    QAction *selectedItem = menu.exec(globalPos);
    if (selectedItem) {
        if (selectedItem->text().compare(add) == 0) {
            addToCart();
            blinkAndFreeze();
        } else if (selectedItem->text().compare(remove) == 0) {
            removeFromCart();
            setEffects();
            animate(false, true);
        } else if (selectedItem->text().compare(cancel) == 0) {
//			qDebug() << "Cancel";
        }
    } else {
//		qDebug() << "Nothing selected";
    }
}
