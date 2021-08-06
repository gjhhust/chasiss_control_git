#include "main.h"


const double NUM[5] = {
   0.004824343357716,  0.01929737343086,   0.0289460601463,  0.01929737343086,
   0.004824343357716
};
const double DEN[5] = {
                   1,   -2.369513007182,    2.313988414416,   -1.054665405879,
     0.1873794923682
};
//double IIR_TICK_p(double in)
//{
//	const double NUM[5] = {
//			0.09398085143379,   0.3759234057352,   0.5638851086028,   0.3759234057352,
//			0.09398085143379
//	};

//	const double DEN[5] = {
//										 1,-2.760046438142e-16,   0.4860288220683,-5.788241261996e-17,
//			0.01766480087244
//	};
//  xin_p[0]=in;
//  y_p[0]=(NUM[0]*xin_p[0]+NUM[1]*xin_p[1]+NUM[2]*xin_p[2]+NUM[3]*xin_p[3]+NUM[4]*xin_p[4]
//              -DEN[1]*y_p[1]-DEN[2]*y_p[2]-DEN[3]*y_p[3]-DEN[4]*y_p[4])/DEN[0];
//  for(int i=3;i>=0;i--)
//  {
//    xin_p[i+1]=xin_p[i];
//    y_p[i+1]=y_p[i];
//  }
//  return y_p[0];
//}
double y_d[5]={0};
double xin_d[5]={0};

double IIR_TICK_d_R(double in)//ButterworthµÍÍ¨ÂË²¨£¬½ØÖ¹ÆµÂÊ10Hz
{

	
  xin_d[0]=in;
  y_d[0]=(NUM[0]*xin_d[0]+NUM[1]*xin_d[1]+NUM[2]*xin_d[2]+NUM[3]*xin_d[3]+NUM[4]*xin_d[4]
              -DEN[1]*y_d[1]-DEN[2]*y_d[2]-DEN[3]*y_d[3]-DEN[4]*y_d[4])/DEN[0];
  for(int i=3;i>=0;i--)
  {
    xin_d[i+1]=xin_d[i];
    y_d[i+1]=y_d[i];
  }
  return y_d[0];
}


double y_d_L[5]={0};
double xin_d_L[5]={0};

double IIR_TICK_d_L(double in)//ButterworthµÍÍ¨ÂË²¨£¬½ØÖ¹ÆµÂÊ10Hz
{

  xin_d_L[0]=in;
  y_d_L[0]=(NUM[0]*xin_d_L[0]+NUM[1]*xin_d_L[1]+NUM[2]*xin_d_L[2]+NUM[3]*xin_d_L[3]+NUM[4]*xin_d_L[4]
              -DEN[1]*y_d_L[1]-DEN[2]*y_d_L[2]-DEN[3]*y_d_L[3]-DEN[4]*y_d_L[4])/DEN[0];
  for(int i=3;i>=0;i--)
  {
    xin_d_L[i+1]=xin_d_L[i];
    y_d_L[i+1]=y_d_L[i];
  }
  return y_d_L[0];
}

double y_d_Pid[5]={0};
double xin_d_Pid[5]={0};

double IIR_TICK_d_Pid(double in)//ButterworthµÍÍ¨ÂË²¨£¬½ØÖ¹ÆµÂÊ10Hz
{

	
  xin_d_Pid[0]=in;
  y_d_Pid[0]=(NUM[0]*xin_d_Pid[0]+NUM[1]*xin_d_Pid[1]+NUM[2]*xin_d_Pid[2]+NUM[3]*xin_d_Pid[3]+NUM[4]*xin_d_Pid[4]
              -DEN[1]*y_d_Pid[1]-DEN[2]*y_d_Pid[2]-DEN[3]*y_d_Pid[3]-DEN[4]*y_d_Pid[4])/DEN[0];
  for(int i=3;i>=0;i--)
  {
    xin_d_Pid[i+1]=xin_d_Pid[i];
    y_d_Pid[i+1]=y_d_Pid[i];
  }
  return y_d_Pid[0];
}

