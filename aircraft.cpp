#include "aircraft.h"

aircraft::aircraft(double startlat,double startlon,double startalt,
                   double startcrs,double startspeed,double startroc)
{
    lat = startlat*PI/180.0;
    lon = startlon*PI/180.0;
    alt = startalt;
    crs = startcrs*PI/180.0;
    speed = startspeed;
    roc = startroc;
}

void aircraft::setLat(double newlat)
{
    lat = newlat*PI/180.0;
}

void aircraft::setLon(double newlon)
{
    lon = newlon*PI/180.0;
}

void aircraft::setAlt(double newalt)
{
    alt = newalt;
}

void aircraft::slotSetCrs(int newcrs)
{
    crs = double(newcrs)*PI/180.0;
}

void aircraft::slotSetSpeed(int newspeed)
{
    speed = double(newspeed);
}

void aircraft::slotSetRoc(int newroc)
{
    roc = double(newroc);
}

double aircraft::getLat()
{
    return(lat*180.0/PI);
}

double aircraft::getLon()
{
    dtemp = lon*180.0/PI;
    while (dtemp<0.0)
        dtemp+=360.0;
    while (dtemp>=360.0)
        dtemp -= 360.0;
    return(dtemp);
}

double aircraft::getAlt()
{
    return(alt);
}

double aircraft::getCrs()
{
    return(crs*180.0/PI);
}

double aircraft::getSpeed()
{
    return(speed);
}

double aircraft::getRoc()
{
    return(roc);
}

void aircraft::update(double tint)
{

    // Propagate lat and lon
    dist = speed*tint/3600.0; //nautical miles
    dist *= NM2KM;  // km
    gcproject(lat,lon,crs,dist,&newlat,&newlon);
    lat = newlat;
    lon = newlon;

    // Propagate altitude
    newalt = alt+roc*tint/60.0;
    alt = newalt;

}
