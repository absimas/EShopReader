#ifndef GRIDITEM_H
#define GRIDITEM_H

#include "CustomTab.h"
#include "ui_GridItem.h"
#include <backend/Item.h>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPixmap>

class GridItem : public QFrame {
// CONFIGURATION
    // Whole item width and height
    static const int SIZE = 200;
    // Height for title and price, will split them in 2/3 and 1/3 accordingly
    static const int ADDITIONAL_HEIGHT = 66;
    // Item's border radius and stroke width
    static const int BORDER_RADIUS = 5;
    static const int BORDER_WIDTH = 1;
// CONFIGURATION
    std::string imageUrl;
    Ui::GridItem ui;
    Q_OBJECT

public:
    // Image
    static const int IMAGE_SIZE = SIZE;
    static const int IMAGE_PADDING = 10;
    // Item
    static const int HEIGHT = SIZE + ADDITIONAL_HEIGHT;
    static const int WIDTH = SIZE;

protected:
    QLabel *titleLabel;
    QLabel *imageLabel;
    QLabel *priceLabel;
    ItemPtr item;
    CustomTab *parentTab;

public:
    explicit GridItem(ItemPtr &item, CustomTab *tab);
    ~GridItem();

signals:

protected:
    bool event(QEvent *e);
    void initUi();
    void setTitle(std::string title);
    void setPrice(float price);
    void setImageUrl(std::string imageUrl);

private:
    std::string getImageUrl() const;
    void setImage(QPixmap image);
    const QPixmap* getImage() const;
    void setImageHint(std::string hint);

friend class CustomTab;
};

#endif // GRIDITEM_H
