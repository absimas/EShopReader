#include "GridItem.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QToolBar>
#include "CartTab.h"
#include <QPushButton>

using namespace std;

MainWindow::MainWindow(const string &title, QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(title));

    // TabHost
    tabHost = findChild<TabHost*>("tabWidget");

    actionSave = findChild<QAction*>("actionSave");
    actionExit = findChild<QAction*>("actionExit");
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    #ifndef QT_NO_DEBUG
        qDebug() << "Initial Window W:" << width() << "H:" << height();
    #endif
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setEnabled(bool enabled) {
    QMainWindow::setEnabled(enabled);
    if (enabled) {
        connect(tabHost, SIGNAL(currentChanged(int)), tabHost, SLOT(tabChanged(int)));
    } else {
        disconnect(tabHost, SIGNAL(currentChanged(int)), tabHost, SLOT(tabChanged(int)));
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    exit();
}

void MainWindow::save() {
    cartTab->save();
}

void MainWindow::exit() {
    if (!cartTab->saved) {
        QMessageBox msgBox;
        msgBox.setText("Your cart wasn't saved. Would you like to save it before exiting?");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setEscapeButton(QMessageBox::Cancel);
        int reply = msgBox.exec();

        switch (reply) {
        case QMessageBox::Yes:
            cartTab->save();
            break;
        case QMessageBox::No:
            break;
        default:
            qDebug() << "Uknown button clicked";
        case QMessageBox::Cancel:
            return;
        }
    }
    QApplication::quit();
}

void MainWindow::updateSize() {
    const CustomTab *tab = (CustomTab*) tabHost->currentWidget();
    const QGridLayout *gridLayout = (QGridLayout*) tab->widget()->layout();

    // Min width/height = fit 1 item
    const int scrollBarWidth = tab->verticalScrollBar()->sizeHint().width();
    const int extraSpace = 2;

    int windowWidth = width() - tab->width() + GridItem::WIDTH + scrollBarWidth + extraSpace +
            2 * gridLayout->margin();
    int windowHeight = height() - tab->height() + GridItem::HEIGHT - extraSpace +
            2 * gridLayout->margin();
    setMinimumWidth(windowWidth);
    setMinimumHeight(windowHeight);

    // Fix the width and height to fit items
    const int rows = floor(height() / (double) (GridItem::HEIGHT ));
    const int cols = floor(width() / (double) (GridItem::WIDTH));

    const int neededWidth = cols * GridItem::WIDTH + gridLayout->margin() * 2 + scrollBarWidth +
            (cols-1) * gridLayout->horizontalSpacing() + extraSpace;
    const int neededHeight = rows * GridItem::HEIGHT + gridLayout->margin() * 2 +
            (rows-1) * gridLayout->verticalSpacing() - extraSpace;

    windowWidth = width() - tab->width() + neededWidth;
    windowHeight = height() - tab->height() + neededHeight;

    // Act as an implicit resize
    resize(windowWidth, windowHeight);
    sizeUpdated = false;

    #ifndef QT_NO_DEBUG
        qDebug() << "Window W:" << width() << "H:"<< height();
    #endif
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    fitItemsToSize();
}

void MainWindow::fitItemsToSize() {
    if (!isEnabled()) return;

    CustomTab *tab = (CustomTab*) tabHost->currentWidget();
    QGridLayout *gridLayout = (QGridLayout*) tab->widget()->layout();

    int rowWidth = gridLayout->columnCount() * GridItem::WIDTH +
            (gridLayout->columnCount()-1) * gridLayout->horizontalSpacing() +
            2 * gridLayout->margin();

    // Add optional scrollbar width
    if (tab->height() < tab->widget()->height()) {
        rowWidth += tab->verticalScrollBar()->sizeHint().width();
    }

    const int freeSpace = tab->width() - rowWidth;
    const int extraSpace = 2;
    const int itemWidth = GridItem::WIDTH + gridLayout->horizontalSpacing();

    if (freeSpace >= itemWidth || freeSpace < 0) {
        // Modify column count
        tab->reAddItems(gridLayout->columnCount() + floor(freeSpace / (double) itemWidth));
        sizeUpdated = true;
    }
}
