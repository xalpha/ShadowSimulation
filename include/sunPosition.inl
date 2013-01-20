inline double SunPosition::degToDouble(int deg[3])
{
	double result = (double)deg[0];
	if(result<0.0)	return (result - ((double)deg[1]*60.0 + (double)deg[2])/3600.0);
	else	return (result + ((double)deg[1]*60.0 + (double)deg[2])/3600.0);
}

inline bool SunPosition::isLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}


inline int SunPosition::dayOfTheYear(int year,int month,int day)
{
	int result = dayOfYear[month-1];
	result += day;
	if (isLeapYear(year) && (result<59)) result++;
	return result;
}

/*
inline int SunPosition::minuteOfTheYear(int year,int month,int day, int hour, int minute)
{
	return (dayOfTheYear(year,month,day)*1440 + (hour*60 + minute));
}
*/


inline void SunPosition::setPosition(int latitude1[3], int longitude1[3], float timeZone1)
{
	// convert longitude to float
	longitudeDeg = degToDouble(longitude1);
	longitudeArr[0]=longitude1[0];
	longitudeArr[1]=longitude1[1];
	longitudeArr[2]=longitude1[2];
	longitude = degToRad(longitudeDeg);

	// convert latitude to float
	latitudeDeg = degToDouble(latitude1);
	latitudeArr[0]=latitude1[0];
	latitudeArr[1]=latitude1[1];
	latitudeArr[2]=latitude1[2];
	latitude = degToRad(latitudeDeg);

	timeZone = timeZone1 * 15 * (PI/180.0);
}


inline void SunPosition::setDate( Date inputDate)
{
	year = inputDate.year;
	month = inputDate.month;
	day = inputDate.day;
	hour = inputDate.hour;
	minute = inputDate.minute;
	second = inputDate.second;
	
	currentDateJulian = dateToJulian(inputDate);
}







