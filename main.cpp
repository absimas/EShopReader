#include "GUI/MainWindow.h"
#include "GUI/EbayTab.h"
#include "GUI/TabHost.h"
#include "GUI/GridItem.h"
#include "GUI/CartTab.h"
#include "GUI/FadingGridItem.h"
#include <QApplication>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <QDebug>
#include <QNetworkAccessManager>
#include <thread>
#include <QGridLayout>
#include <QFile>
#include <QProgressDialog>
#include <QProgressBar>
#include <QDesktopWidget>

// ToDo
    // QLabel sub class
        // DONE basket-able (click + drag to icon / right-click->add to basket->doh)
        // DONE dbl-clickable (ebay url)
        // DONE hoverable (dim/highlight/reveal)
    // Basket
        // DONE File->Save basket == write_xml items with a specific hierarchy, make a CartParser
    // Tabs
        // DONE Sutvarkyt tabu metodus.. populate populate populate ~.~
        // DONE-SORT-OF Item containers shift on window resize )))
        // DONE load cart first and then tabs, while also checking if items are in the cart
        // DONE Separate tab or somewhere else to display the basket's items
        // DONE Base class CustomTab or even Tab
        // DONE ShopTab rename to EbayTab
        // DONE CartTab : public Tab
    // Backendo fixai:
        // ABORT Customer has a single cart at a time
        // DONE Rename Order to Cart
        // DONE Get rid of Item class? If so rename Item to Item, Item struct rename to CartItem, doh
        // DONE Rename Parserius i Importerius
        // DONE Parseriai ima vektoriu o ne shopa (kad cartTab'as neturetu inheritint shopo)
    // Last fixai:
        // ABORT Animate border opacity too?
        // DONE Pakeist kur galima is public i private (geriau friend panaudot)
        // DONE Kai paleidziant programa uzvestas mausas (turbut ant itemo) crashinas
    // Finishing touches
        // DONE Detect maximize, close buttons/events
        // DONE Min W/H - window + single item
        // DONE Progress dialog
        // ABORT Sort
        // ABORT Portable app

using namespace std;

QNetworkAccessManager *netManager;
CartTab *cartTab;

void download(const string &from, const string &to, QObject &context);

//void task1(MainWindow *w) {
//// call with std::thread t1(task1);
//}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    netManager = new QNetworkAccessManager(&app);
    MainWindow w("Elektroninių parduotuvių katalogas");

    // Show a progress dialog
    QProgressDialog progressDialog(&w);
    progressDialog.setRange(0, 0);
    progressDialog.show();

    // Fetch ebay data-feed
    const string filename("ebay.xml");
    download("http://deals.ebay.com/feeds/xml", filename, app);


    // Prevent main window flickering and interaction
    w.setEnabled(false);
    // Set window size and center it before showing
    w.resize(673, 908);
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
    w.tabHost->setUpdatesEnabled(false);
    w.show();


    // Create tabs
    // CartTab always created first
    cartTab = new CartTab(w.tabHost);
    new EbayTab("eBay", filename, w.tabHost);

    // Update window size, display changes
    w.updateSize();
    w.setEnabled(true);
    w.tabHost->setUpdatesEnabled(true);


    // Hide the progress dialog
    progressDialog.close();

    return app.exec();
}

void download(const string &from, const string &to, QObject &context) {
    #ifndef QT_NO_DEBUG
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    #endif

    QString url = QString::fromStdString(from);
    QString filename = QString::fromStdString(to);
    QNetworkReply *reply = netManager->get(QNetworkRequest(url));
    QEventLoop loop;
    context.connect(reply, &QNetworkReply::finished, [&loop, reply, &url, &filename, t1]() {
        loop.quit();
        // Output reply to filename
        QByteArray result = reply->readAll();
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        file.write(result);
        file.close();
        #ifndef QT_NO_DEBUG
            chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
            qDebug() << "Downloading" << result.size() << "bytes from" << url << "to" << filename
                     << "took" << duration << "ms";
        #endif
        reply->deleteLater();
    });
    loop.exec();
}
