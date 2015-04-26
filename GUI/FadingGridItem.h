#ifndef FADINGGRIDITEM_H
#define FADINGGRIDITEM_H

#include "CartTab.h"
#include "GridItem.h"
#include <backend/Item.h>
#include <QVariantAnimation>
#include <QVariant>
#include "CustomTab.h"

extern CartTab *cartTab;

class FadingGridItem : public GridItem {
    // Label animation cfg
    static const int ANIM_DURATION = 250;
    static constexpr double INITIAL_OPACITY = 0.55;
    static constexpr double FINAL_OPACITY = 1.0;

    class LabelAnimation : public QVariantAnimation {
        LabelAnimation();
    public:
        LabelAnimation(QObject *parent) : QVariantAnimation(parent) {};
        virtual ~LabelAnimation(){};

        void updateCurrentValue(const QVariant &value){
            emit valueChanged(value);
        }
    };
    LabelAnimation *animation;
    Q_OBJECT

protected:
    bool inCart = false;

public:
    explicit FadingGridItem(ItemPtr &item, CustomTab *tab);

private slots:
    void showContextMenu(const QPoint &pos);
    void animationStep(const QVariant &value);

protected:
    bool event(QEvent *e);

private:
    void initUi();
    bool hasEffects();
    void setEffects();
    void clearEffects();
    void animate(bool reveal, bool force = false);
    void dimPrevious();
    void blinkAndFreeze();
    bool isInCart();
    bool idIsInCart();
    void addToCart();
    void removeFromCart();

friend class EbayTab;
};

#endif // FADINGGRIDITEM_H
