#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include "TabHost.h"

namespace Ui {
	class MainWindow;
}

class CartTab;
extern CartTab *cartTab;

class MainWindow : public QMainWindow {
    QAction *actionSave;
	QAction *actionExit;
	Ui::MainWindow *ui;
	Q_OBJECT

public:
    bool sizeUpdated = false;

public:
	TabHost *tabHost;

public:
	explicit MainWindow(const std::string &title, QWidget *parent = 0);
	~MainWindow();
	void setEnabled(bool enabled);

private slots:
	void save();
	void exit();

public:
    void fitItemsToSize();
	void updateSize();

protected:
    void closeEvent(QCloseEvent *event);

private:
	void resizeEvent(QResizeEvent *event);

friend class CartTab;
};

#endif // MAINWINDOW_H
