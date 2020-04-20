#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleValidator>
#include <QDateTime>
#include <QDateTimeEdit>

#include "aircraft.h"


class setupGui : public QDialog
{
    Q_OBJECT

public:
    setupGui(aircraft *myplane, QDateTime *mydatetime,
             QWidget *parent, const char *name=0);
    ~setupGui();

private:
    QLineEdit *startlat;
    QLineEdit *startlon;
    QLineEdit *startht;
    QDoubleValidator *latval;
    QDoubleValidator *lonval;
    QDoubleValidator *htval;
    QDateTimeEdit *datetimeedit;
    aircraft *setupplane;
    QDateTime *setupdatetime;

private slots:
    void slotTestInputs();

};

#endif // SETUPGUI_H
