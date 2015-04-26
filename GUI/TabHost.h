#ifndef TABHOST_H
#define TABHOST_H

#include <QTabWidget>
#include "CartTab.h"
#include "CustomTab.h"

class TabHost : public QTabWidget {
	Q_OBJECT
public:
	explicit TabHost(QWidget *parent = 0);

signals:

public slots:


private Q_SLOTS:
	void tabChanged(int index);
};

#endif // TABHOST_H
