#include "geogeometry.h"

#include <algorithm>


GeoPoint::GeoPoint() :
	x(0.0f), y(0.0f)
{
}

GeoPoint::GeoPoint(double xx, double yy) :
	x(xx), y(yy)
{
}

GeometryType GeoPoint::getGeometryType() const
{
	return kPoint;
}

const char* GeoPoint::getGeometryName() const
{
	return "POINT";
}

int GeoPoint::getNumPoints() const
{
	return 1;
}

GeoExtent GeoPoint::getExtent() const
{
	return { x, x, y, y };
}

bool GeoPoint::isEmpty() const
{
	return false;
}

void GeoPoint::swapXY()
{
	std::swap(x, y);
}


