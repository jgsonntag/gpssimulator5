#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QObject>

#include "constants.h"
#include "utility.h"

class aircraft : public QObject
{
    Q_OBJECT

public:
    aircraft(double startlat,double startlon,double startalt,
             double startcrs,double startspeed,double startroc);
    void setLat(double newlat); // degrees
    void setLon(double newlon); // degrees
    void setAlt(double newalt); // ft
    //void setCrs(int newcrs); // degrees
    //void setSpeed(int newspeed); // knots
    //void setRoc(int newroc); // fpm
    double getLat(); // degrees
    double getLon(); // degrees
    double getAlt(); // ft
    double getCrs(); // degrees
    double getSpeed(); // knots
    double getRoc(); // fpm
    void update(double tint); // secs

public slots:
    void slotSetCrs(int newcrs); // degrees
    void slotSetSpeed(int newspeed); // knots
    void slotSetRoc(int newroc); // fpm

private:
    double lat; // radians
    double lon; // radians
    double alt; // ft
    double crs; // radians
    double speed; // knots
    double roc; // fpm
    double dist;
    double newlat;
    double newlon;
    double newalt;
    double dtemp;

};

#endif // AIRCRAFT_H
