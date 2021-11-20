#ifndef NormalizPointScale_h__
#define NormalizPointScale_h__
#include "laspoint.hpp"
void normalizPointScale(LASpoint & point,const double scaleX,const double scaleY,const double scaleZ,const double nor)
{
	point.X = point.X*scaleX / nor;
	point.Y = point.Y*scaleY / nor;
	point.Z = point.Z*scaleX / nor;
}

double getRealValueByNormal(I32 intValue,double offset)
{
	return intValue *0.0001 + offset;
}

I32 getUnrealValue(double realValue, double offset)
{

}
#endif // NormalizPointScale_h__
