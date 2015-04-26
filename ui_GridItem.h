/********************************************************************************
** Form generated from reading UI file 'GridItem.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDITEM_H
#define UI_GRIDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GridItem
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *image;
    QLabel *title;
    QLabel *price;

    void setupUi(QFrame *GridItem)
    {
        if (GridItem->objectName().isEmpty())
            GridItem->setObjectName(QStringLiteral("GridItem"));
        GridItem->setEnabled(true);
        GridItem->resize(200, 266);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GridItem->sizePolicy().hasHeightForWidth());
        GridItem->setSizePolicy(sizePolicy);
        GridItem->setAutoFillBackground(false);
        GridItem->setStyleSheet(QStringLiteral(""));
        GridItem->setFrameShadow(QFrame::Raised);
        GridItem->setLineWidth(0);
        verticalLayout = new QVBoxLayout(GridItem);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        image = new QLabel(GridItem);
        image->setObjectName(QStringLiteral("image"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(200);
        sizePolicy1.setVerticalStretch(200);
        sizePolicy1.setHeightForWidth(image->sizePolicy().hasHeightForWidth());
        image->setSizePolicy(sizePolicy1);
        image->setLayoutDirection(Qt::LeftToRight);
        image->setStyleSheet(QStringLiteral("border: 0px;"));
        image->setAlignment(Qt::AlignCenter);
        image->setMargin(0);

        verticalLayout->addWidget(image);

        title = new QLabel(GridItem);
        title->setObjectName(QStringLiteral("title"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(title->sizePolicy().hasHeightForWidth());
        title->setSizePolicy(sizePolicy2);
        title->setStyleSheet(QStringLiteral("border: 0px; padding-left: 1px; padding-right: 1px;"));
        title->setScaledContents(false);
        title->setWordWrap(true);

        verticalLayout->addWidget(title);

        price = new QLabel(GridItem);
        price->setObjectName(QStringLiteral("price"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(price->sizePolicy().hasHeightForWidth());
        price->setSizePolicy(sizePolicy3);
        price->setLayoutDirection(Qt::LeftToRight);
        price->setStyleSheet(QStringLiteral("border: 0px; padding-left: 1px; padding-right: 1px;"));
        price->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        price->setMargin(0);

        verticalLayout->addWidget(price);

        price->raise();
        image->raise();
        title->raise();

        retranslateUi(GridItem);

        QMetaObject::connectSlotsByName(GridItem);
    } // setupUi

    void retranslateUi(QFrame *GridItem)
    {
        GridItem->setWindowTitle(QString());
        image->setText(QString());
        title->setText(QString());
        price->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GridItem: public Ui_GridItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDITEM_H
