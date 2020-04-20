#ifndef UTILITY
#define UTILITY

#include "constants.h"

void gps2kal(int wkn,double wks,int *year,int *month,int *day,int *hour,int *min,double *sec);
void gps2jul(int wkn,double wks,long *mjd,double *mjs);
void jul2kal(long mjd,double mjs,int *year,int *month,int *day,int *hour,int *min,double *sec);
void kal2gps(int year,int month,int day,int hour,int min,double sec,int *wkn,double *wks);
void kal2jul(int year,int month,int day,int hour,int min,double sec,long int *mjd,double *mjs);
void jul2gps(long int mjd,double mjs,int *wkn,double *wks);
void gcproject(double lat1,double lon1,double tc1,double dist,double *lat2,double *lon2);

#endif // UTILITY

