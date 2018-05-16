//////////////////////////////////////////////////////////////
//
// Your #includes here; make sure you are allowed them ...
//

#include <stdlib.h>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <iostream>

//////////////////////////////////////////////////////////////
//
// #includes and function declarations; do not remove
//


#ifndef MARMOSET_TESTING
#include <iostream>
using namespace std;
#endif

#define NaN std::numeric_limits<float>::quiet_NaN()
#define isNaN(X) (X != X) // NaN is the only float that is not equal to itself

//////////////////////////////////////////////////////////////
//
// Function declarations; do not remove


bool hitTargetGivenAngle (const float h, const float m, const float theta,
              const float d, const float step, const float k,
              const float t, const float b, const float w, float& v);

//////////////////////////////////////////////////////////////
//
// Your code here ...
//

const float g = -9.8;


bool f(const float h, const float v, const float m,
                           const float theta, const float d,
                           const float t, const float b, const float w,
                           float& destX, float& destY)
{
  // Your code here ...

  if (h<0 || v<0 || theta<-90 || theta>270 || m<0 || b<0 || d<0 || w<0)
    return false;

  float ang = theta*3.14159265/180.0;
	float viy = v*sin(ang);
	float vx = v*cos(ang);

  if (theta>90)
  {
    float timey = ( (-viy+sqrt(pow(viy,2)-2*g*h))/g > (-viy-sqrt(pow(viy,2)-2*g*h))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*h))/g: (-viy-sqrt(pow(viy,2)-2*g*h))/g;
    destY=0;
    destX=v*timey*cos(ang);
    return true;
  }
  else if (theta==90 || v==0)
  {
    destY=h;
    destX=0;
    return true;
  }

  if (t>0)
  {
    float timetob = b/(vx);
    float dtoy = h+viy*timetob+(g/2)*timetob*timetob;
    if (0<dtoy && dtoy<=t)
    {
      destY=dtoy;
      destX=b;
      return true;
    }
    if (dtoy<0)
    {
      float timey = ( (-viy+sqrt(pow(viy,2)-2*g*h))/g > (-viy-sqrt(pow(viy,2)-2*g*h))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*h))/g: (-viy-sqrt(pow(viy,2)-2*g*h))/g;
      destY=0;
      destX=v*timey*cos(ang);
      return true;
    }
    if (dtoy>t)
    {
      float timesyt = ( (-viy+sqrt(pow(viy,2)-2*g*(h-t)))/g > (-viy-sqrt(pow(viy,2)-2*g*(h-t)))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*(h-t)))/g: (-viy-sqrt(pow(viy,2)-2*g*(h-t)))/g;
      float sx = v*timesyt*cos(ang);
      if (sx<=(b+w))
      {
        destY=t;
        destX=sx;
        return true;
      }
      else
      {
        float timey = ( (-viy+sqrt(pow(viy,2)-2*g*h))/g > (-viy-sqrt(pow(viy,2)-2*g*h))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*h))/g: (-viy-sqrt(pow(viy,2)-2*g*h))/g;
        destY=0;
        destX=v*timey*cos(ang);
        return true;
      }
    }
  }
  else if (t<0)
  {
    float timetox = (b+w)/(vx);
    float dtoy = h+viy*timetox+(g/2)*pow(timetox,2);
    if (dtoy>0)
    {
      float timey = ( (-viy+sqrt(pow(viy,2)-2*g*h))/g > (-viy-sqrt(pow(viy,2)-2*g*h))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*h))/g: (-viy-sqrt(pow(viy,2)-2*g*h))/g;
      destY=0;
      destX=v*timey*cos(ang);
      return true;
    }
    if (dtoy>=t && dtoy<0)
    {
      destY=h+v*timetox*sin(ang)+(g/2)*pow(timetox,2);
      destX=b+w;
      return true;
    }
    if (dtoy<t)
    {
      float timetob = b/(vx);
      float dtoy = h+viy*timetob+(g/2)*timetob*timetob;

      if (dtoy<=0)
      {
        float timey = ( (-viy+sqrt(pow(viy,2)-2*g*h))/g > (-viy-sqrt(pow(viy,2)-2*g*h))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*h))/g: (-viy-sqrt(pow(viy,2)-2*g*h))/g;
        destY=0;
        destX=v*timey*cos(ang);
        return true;
      }
      if (dtoy>0)
      {
        destY=t-h;
        float timesyt = ( (-viy+sqrt(pow(viy,2)-2*g*(2*h-t)))/g > (-viy-sqrt(pow(viy,2)-2*g*(2*h-t)))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*(2*h-t)))/g: (-viy-sqrt(pow(viy,2)-2*g*(2*h-t)))/g;
        destX = v*timesyt*cos(ang);
        return true;
      }
    }
  }
  else
  {
      float timey = ( (-viy+sqrt(pow(viy,2)-2*g*h))/g > (-viy-sqrt(pow(viy,2)-2*g*h))/g ) ? (-viy+sqrt(pow(viy,2)-2*g*h))/g: (-viy-sqrt(pow(viy,2)-2*g*h))/g;
      destY=0;
      destX=v*timey*cos(ang);
      return true;
  }
}

float fhelper(const float h, const float v, const float m,
                           const float theta, const float d,
                           const float t, const float b, const float w)
{
  float x=0, y=0;
  if (f(h,v,m,theta,d,t,b,w,x,y))
    return (d - x);
  else
    return std::numeric_limits<float>::quiet_NaN();
}




float bisectionHelper(const float left, const float right,
          const float precision, const int count, const float h, const float d, const float t,
          const float b, const float w, const float theta)
{

  float midpoint = (right + left)/2;
  float newP = fhelper(h,midpoint,0,theta,d,t,b,w);
  if(newP < 0)
      newP *= -1;

  if(newP <= precision || count == 0)
      return midpoint;
  if((fhelper(h,midpoint,0,theta,d,t,b,w)*fhelper(h,right,0,theta,d,t,b,w)) < 0)
      return bisectionHelper(midpoint, right, precision, count-1,h,d,t,b,w,theta);
  else
      return bisectionHelper(left, midpoint, precision, count-1,h,d,t,b,w,theta);

}



float bisection(const float left, const float right,
    const float precision, const float h, const float d, const float t, const float b, const float w, const float theta){

  float midpoint = (right + left)/2;

  if (left >= right)
      return std::numeric_limits<float>::quiet_NaN();

  if(precision < 0)
      return std::numeric_limits<float>::quiet_NaN();

  float check = fhelper(h,midpoint,0,left,d,t,b,w);

	if (check == 0) {
      return left;
  }
  else if(check != check){
      return std::numeric_limits<float>::quiet_NaN();
  }
  check = fhelper(h,midpoint,0,right,d,t,b,w);
  if(check != check){
      return std::numeric_limits<float>::quiet_NaN();
  }
  else if (check == 0) {
    return right;
  }

  float mid = bisectionHelper(left, right, precision, 12345, h, d, t, b, w, theta);
  return mid;

}


bool hitTargetGivenAngle (const float h, const float m, const float theta,
              const float d, const float step, const float k,
              const float t, const float b, const float w, float& v)
{


  if (h<0 || theta<-90 || theta>270 || m<0 || b<0 || d<0 || w<0)
    return false;

  float left = 0;
  float right = 101;

  float precision = 0.01;


  v = bisection(left,right,precision,h,d,t,b,w,theta);

  return true;
}


#ifndef MARMOSET_TESTING
int main() {

  // Some test driver code here ....

  float h = 50;
  float d = 60;
  float b = 40;
  float t = 70;
  float w = 30;
  float m = 1;

  float theta = 73.61; // Angle in degrees;
  float v = 25;

/*  cout << "Given ang: " << theta << endl;
  cout << "Target at (" << d << "," << t << ")" << endl;
  if (hitTargetGivenAngle (h, m, theta, d, t, b, w, v)) {
    cout << "required initial velocity: " << v << endl << endl;
  }
  else {
    cout << "cannot calculate the velocity\n\n";
  }

  cout << "Given initial velocity: " << v << endl;
  cout << "Target at (" << d << "," << t << ")" << endl;
*/

  return 0;
}
#endif
