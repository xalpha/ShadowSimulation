////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of ShadowSimulation, a C++ app for schadow simulation    //
//                                                                            //
// Copyright (C) 2006 Alexandru Duliu                                         //
//                                                                            //
// ShadowSimulation is free software; you can redistribute it and/or          //
// modify it under the terms of the GNU  General Public License               //
// as published by the Free Software Foundation; either version 3             //
// of the License, or (at your option) any later version.                     //
//                                                                            //
// ShadowSimulation is distributed in the hope that it will be useful,        //
// but WITHOUT ANY WARRANTY; without even the implied warranty of             //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              //
// GNU General Public License for more details.                               //
//                                                                            //
// You should have received a copy of the GNU General Public License          //
// along with ShadowSimulation. If not, see <http://www.gnu.org/licenses/>.   //
//                                                                            //
///////////////////////////////////////////////////////////////////////////////

/***************************************************************
 *                                                             *
 * program  : Sun Position Calculator                          *
 * author   : Alexandru Duliu                                  *
 * version  : 0.5                                             *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/
#ifndef SUNPOSITION_H_
#define SUNPOSITION_H_

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "mathutils.h"


// number of days in a year
static const int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
static const int daysInMonthLeapYear[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
// number of days in that year until this month
static const int dayOfYear[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
static const double PI = 3.1415926535898;

#ifndef CLASS_DATE
#define CLASS_DATE

class Date
{
public:
	Date();
	Date(int y, int m, int d, int h, int min, int s);
	
	void init(int y, int m, int d, int h, int min, int s);
	
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

#endif

/** 
 * Calculates the sun position in the sky
**/
class SunPosition
{
	
private:

	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;	
	
	double currentDateJulian;

	double longitude;
	double longitudeDeg;
	double longitudeArr[3];
	
    double latitude;
	double latitudeDeg;
	double latitudeArr[3];

	double timeZone;
	double declination;
	double equation;
	double azimuth;
	double elevation;
	
	bool checkDate( Date inputDate);
	
	inline double degToDouble(int deg[3]);
	inline bool isLeapYear(int year);
	inline int dayOfTheYear(int year,int month,int day);
	
	

public:
	
	double azimuthOut;
	double elevationOut;
	
	SunPosition();
	SunPosition(int latitude1[3], int longitude1[3], float timeZone1);
	
	double dateToJulian( Date inputDate);
	Date julianToDate(double inputDate);

	inline void setPosition(int latitude1[3], int longitude1[3], float timeZone1);
	inline void setDate( Date inputDate);

	void calculate();
};

#include "sunPosition.inl"

#endif
