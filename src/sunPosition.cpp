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

#include "sunPosition.h"

Date::Date()
{

}

Date::Date(int y, int m, int d, int h, int min, int s)
{
	init(y,m,d,h,min,s);
}

void Date::init(int y, int m, int d, int h, int min, int s)
{
	year = y;
	month = m;
	day = d;
	hour = h;
	minute = min;
	second = s;
}



SunPosition::SunPosition(){}

SunPosition::SunPosition(int latitude1[3], int longitude1[3], float timeZone1)
{
	setPosition(latitude1,longitude1,timeZone1);
}

void SunPosition::calculate()
{
	// Julian Date
	double julianDate=(double)dayOfTheYear(year,month,day);

	// Declination
	double temp = 2*PI*((julianDate-1)/365.0);
	declination = 0.322003 - 22.971 * cos(temp)- 0.357898 * cos(2*temp)- 0.14398 * cos(3*temp)+ 3.94638 * sin(temp)+ 0.019334 * sin(2*temp)+ 0.05928 * sin(3*temp);
	if (declination > 89.9) declination = 89.9;
	if (declination < -89.9) declination = -89.9;
	// Convert to radians.
	declination *= (PI/180.0);

	
	// Ecuation of time
	temp = (279.134 + 0.985647 * julianDate) * (PI / 180.0);
	equation = 5.0323- 100.976 * sin(temp)+ 595.275 * sin(2*temp)+ 3.6858 * sin(3*temp)- 12.47 * sin(4*temp)- 430.847 * cos(temp)+ 12.5024 * cos(2*temp)+ 18.25 * cos(3*temp);
	// convert the ecuation in hours
	equation /= 3600.0;

	// Solar Time
	double localTime = (double)hour + (double)minute/60.0 + (double)second/3600.0;
	double difference = (((longitude - timeZone) * 180/PI) * 4) / 60.0;
	double solarTime = localTime + equation + difference;
	double hourAngle = (15.0 * (solarTime - 12.0)) * (PI / 180.0);

	// Solar Elevation
	temp = (sin(declination) * sin(latitude)) + (cos(declination) * cos(latitude) * cos(hourAngle));
	elevation = asin(temp);

	// Solar Azimuth
	temp = (sin(declination) * cos(latitude)) - (cos(declination) * sin(latitude) *  cos(hourAngle));
	// TODO
	double sin1, cos2;
    if (elevation < (PI/2.0)) 
	{ 
        sin1 = (-cos(declination) * sin(hourAngle)) / cos(elevation);
        cos2 = temp / cos(elevation);
	}

    else {
        sin1 = 0.0;
        cos2 = 0.0;
        }

	// Some range checking.
	if (sin1 > 1.0) sin1 = 1.0;
	if (sin1 < -1.0) sin1 = -1.0;
	if (cos2 < -1.0) cos2 = -1.0;
	if (cos2 > 1.0) cos2 = 1.0;

	// Calculate azimuth subject to quadrant.
	if (sin1 < -0.99999) azimuth = asin(sin1);
	else if ((sin1 > 0.0) && (cos2 < 0.0)) {
		if (sin1 >= 1.0) azimuth = -(PI/2.0);
		else azimuth = (PI/2.0) + ((PI/2.0) - asin(sin1));
		}
	else if ((sin1 < 0.0) && (cos2 < 0.0)) {
		if (sin1 <= -1.0) azimuth = (PI/2.0);
		else azimuth = -(PI/2.0) - ((PI/2.0) + asin(sin1));
		}
	else azimuth = asin(sin1);
    if ((azimuth < 0.0) && (localTime < 10.0)) azimuth = -azimuth;

	// Write output values
	elevationOut = radToDeg(elevation);
	azimuthOut = radToDeg(azimuth);
}


double SunPosition::dateToJulian( Date inputDate)
{	
	// check date
	if( checkDate(inputDate) )
	{
		printf ("INVALID DATE: %i %i %i %i %i %i\n",inputDate.year,inputDate.month,inputDate.day,inputDate.hour,inputDate.minute,inputDate.second);
		return -1;
	}
	
	// Fliegel and van Flandern (1968) Algorithm
	
	int jd = inputDate.day-32075+1461*(inputDate.year+4800+(inputDate.month-14)/12)/4+367*(inputDate.month-2-(inputDate.month-14)/12*12) /12-3*((inputDate.year+4900+(inputDate.month-14)/12)/100)/4;
	double result = (double)jd + (double)inputDate.hour/24.0 + (double)inputDate.minute/(24.0*60.0) + (double)inputDate.second/(24.0*3600.0);
	
	//printf("julianDate= %f   %i %i %i %i %i %i\n",result,inputDate.year,inputDate.month,inputDate.day,inputDate.hour,inputDate.minute,inputDate.second);
	
	return result;
}

Date SunPosition::julianToDate(double input)
{
	// Fliegel and van Flandern (1968) Algorithm
	
	int temp = (int)input;
	int temp1= temp+68569;
	int temp2= 4*temp1/146097;
	temp1= temp1-(146097*temp2+3)/4;
	int year= 4000*(temp1+1)/1461001;
	temp1= temp1-1461*year/4+31;
	int month= 80*temp1/2447;
	int day= temp1-2447*month/80;
	temp1= month/11;
	month= month+2-12*temp1;
	year= 100*(temp2-49)+year+temp1;
	double temp3 = fmod(input,1.0);
	int hour = (int) (temp3*24.0);
	int minute = (int) (temp3*1440) %60;
	int second = (int) (temp3*24.0*3600.0) % 60;
	
	Date outputDate;
	outputDate.year=year;
	outputDate.month=month;
	outputDate.day=day;
	outputDate.hour=hour;
	outputDate.minute=minute;
	outputDate.second=second;
	
	//printf("julianDate= %f   %i %i %i %i %i %i\n",input,year,month,day,hour,minute,second);
	
	return outputDate;
}


bool SunPosition::checkDate( Date inputDate)
{
	// check if date is in correct format
	if( (inputDate.year <= 0) 	|| 
		(inputDate.month <= 0 	|| inputDate.month > 12) 	|| 
		(inputDate.day <= 0) 	|| 
		(inputDate.hour < 0 	|| inputDate.hour >= 24) 	|| 
		(inputDate.minute < 0 	|| inputDate.minute >= 60) 	|| 
		(inputDate.second < 0 	|| inputDate.second >= 60) )
	{
		return true;
	}
	else
	{
		// calculate if is leap year
		if((inputDate.year % 4 == 0 && inputDate.year % 100 != 0) || inputDate.year % 400 == 0)	
		{
			if(inputDate.day > daysInMonthLeapYear[inputDate.month -1])	return true;
		}
		else
		{
			if(inputDate.day > daysInMonth[inputDate.month -1])	return true;
		}
	}
	
	// all OK!
	return false;
}
