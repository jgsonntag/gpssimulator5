#include <QDebug>

#include "utility.h"

#define YR0  1972
#define MJD0 41317


void gps2kal(int wkn, double wks,int *year,int *month,int *day,int *hour,int *min,double *sec)
{
  long mjd;
  double mjs;

  /*  Convert first to MJD, then to calendar format  */
  gps2jul(wkn,wks,&mjd,&mjs);
  jul2kal(mjd,mjs,year,month,day,hour,min,sec);

}


void kal2gps(int year,int month,int day,int hour,int min,double sec,int *wkn,double *wks)
{
    long int mjd;
    double mjs;

    /* Convert first to MJD, then to GPS format  */
    kal2jul(year,month,day,hour,min,sec,&mjd,&mjs);
    jul2gps(mjd,mjs,wkn,wks);

}


void jul2gps(long int mjd,double mjs,int *wkn,double *wks)
{
  long int djd;
  double temp;

  djd = mjd - (long int)44244;
  *wkn = (int)(djd/7);
  temp = (double)djd/7.0 - (double)(*wkn);
  *wks = temp*7.0*86400.0 + mjs;
  if (*wks >= 604800.0)
    {
    *wks -= 604800.0;
    *wkn += 1;
    }
}



void kal2jul(int year,int month,int day,int hour,int min,double sec,long int *mjd,double *mjs)
{
  int nyrs,yr,i;
  int dmonth[12];
  long int ndays;
  double temp;

  /*  Define number of days per month  */
  dmonth[ 0] = 31;  dmonth[ 1] = 28;  dmonth[ 2] = 31;
  dmonth[ 3] = 30;  dmonth[ 4] = 31;  dmonth[ 5] = 30;
  dmonth[ 6] = 31;  dmonth[ 7] = 31;  dmonth[ 8] = 30;
  dmonth[ 9] = 31;  dmonth[10] = 30;  dmonth[11] = 31;

  /*  Count days due to past full years, including leap days  */
  nyrs = year - YR0;
  ndays = nyrs*365;
  for (yr=YR0; yr<year; yr++)
    {
    temp = (double)yr/4.0 - (double)(yr/4);
    if (temp == 0.0)
      ndays += 1;
    }

  /*  Add days due to past full months plus leap days  */
  for (i=0; i<(month-1); i++)
    ndays += dmonth[i];
  temp = (double)(year)/4.0 - (double)((year)/4);
  if (temp == 0 && month > 2)
    ndays += 1;

  /*  Add days due to day of month  */
  ndays += day - 1;

  /*  Compute mjd  */
  *mjd = MJD0 + ndays;

  /*  Compute mjs  */
  *mjs = hour*3600.0 + min*60.0 + sec;

}



void gps2jul(int wkn,double wks,long *mjd,double *mjs)
{
  long djd,sjd;

  sjd = (long)(wks/86400.0);
  djd = (long)wkn*7 + sjd;
  *mjd = (long)44244 + djd;
  *mjs = wks - (double)sjd*86400.0;
  if (*mjs >= 86400.0)
  {
    *mjs -= 86400.0;
    *mjd += 1;
  }

}


void jul2kal(long mjd,double mjs,int *year,int *month,int *day,int *hour,int *min,double *sec)
{
  int nyrs,i;
  int dmonth[12];
  long ndays;
  double temp;

  /*  Define number of days per month  */
  dmonth[ 0] = 31;  dmonth[ 1] = 28;  dmonth[ 2] = 31;
  dmonth[ 3] = 30;  dmonth[ 4] = 31;  dmonth[ 5] = 30;
  dmonth[ 6] = 31;  dmonth[ 7] = 31;  dmonth[ 8] = 30;
  dmonth[ 9] = 31;  dmonth[10] = 30;  dmonth[11] = 31;

  /*  Compute year  */
  ndays = mjd - MJD0;
  nyrs = int(double(ndays)/365.25);
  *year = YR0 + nyrs;
  temp = double(*year)/4.0 - double((*year)/4);

  /*  Adjust for leap day and compute month and day  */
  if (temp == 0.0)
  {
    dmonth[1] = 29;
    ndays += 1;
  }
  ndays -= long(nyrs*365.25);
  i = 0;
  while(ndays > dmonth[i])
  {
    ndays -= dmonth[i];
    i += 1;
  }
  *month = i+1;
  *day = ndays;

  /*  Compute hour, minute and second  */
  *hour = int(mjs/3600.0);
  *min = int((mjs - (*hour)*3600.0)/60.0);
  *sec = mjs - (*hour)*3600.0 - (*min)*60.0;

}

/*------------------------------------------------------------------------*
 NAME:     gcproject

 PURPOSE:  Computes ending waypoint lat2/lon2 given starting waypoint
           lat1/lon1, starting true course tc1 and distance dist. Lat/lon
           are in radians N latitude/E longitude, tc1 is in radians,
           and dist is in kilometers.

 AUTHOR:   John Gary Sonntag

 DATE:     July 1999
 *------------------------------------------------------------------------*/
void gcproject(double lat1,double lon1,double tc1,double dist,double *lat2,
               double *lon2)
{
  double templon;

  /*  Compute point 2  */
  lon1 = -lon1;
  dist /= RAD2KM;
  *lat2 = asin(sin(lat1)*cos(dist)+cos(lat1)*sin(dist)*cos(tc1));
  templon = atan2(sin(tc1)*sin(dist)*cos(lat1),cos(dist)-sin(lat1)*sin(*lat2));
  /* lonb = (lona-templon+PI)%(2.0*PI) - PI;  */
  *lon2 = -(lon1 - templon);

}
