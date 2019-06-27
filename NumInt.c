#include "NumInt.h"

void rungekutta(void odefun(double *, double *, double *),double * time, double stepsize, int order, double * CurSt, double * param)
{
  int i=0;
  double * K1 = (double *)malloc(sizeof(double) * order);
  double * K2 = (double *)malloc(sizeof(double) * order);
  double * K3 = (double *)malloc(sizeof(double) * order);
  double * K4 = (double *)malloc(sizeof(double) * order);
  double * tmp = (double *)malloc(sizeof(double) * order);
  double * diff = (double *)malloc(sizeof(double) * order);
  odefun(CurSt,diff,param);
  for (i=0;i<order;++i) K1[i] = diff[i],tmp[i] = CurSt[i] + K1[i]*0.5*stepsize;
  odefun(tmp,diff,param);
  for (i=0;i<order;++i) K2[i] = diff[i],tmp[i] = CurSt[i] + K1[i]*0.207106781187*stepsize + K2[i]*0.292893218813*stepsize;
  odefun(tmp,diff,param);
  for (i=0;i<order;++i) K3[i] = diff[i],tmp[i] = CurSt[i] - K2[i]*0.707106781187*stepsize + K3[i]*1.471404520791*stepsize;
  odefun(tmp,diff,param);
  for (i=0;i<order;++i) K4[i] = diff[i],CurSt[i] += (K1[i] + K2[i]*0.585786437627 + K3[i]*3.414213562373 + K4[i])*stepsize/6; 
  * time += stepsize;
  free(K1);K1 = NULL;
  free(K2);K2 = NULL;
  free(K3);K3 = NULL;
  free(K4);K4 = NULL;
  free(tmp);tmp = NULL; 
}

void flywheel_control(double * CurSt, double * diff, double T, double err)
{
  diff[8] = fabs(CurSt[8]) > 523 ? 0 : - CurSt[7] /  0.0382;
  diff[7] = 10*(T - CurSt[7]);
  diff[0] = CurSt[3];
  diff[1] = CurSt[4];
  diff[2] = CurSt[5];
  diff[3] =  4.60683761e-02*CurSt[1] + 4.10256410e-01*CurSt[2] + 6.58119658e-04*CurSt[4] + 1.02564103e-03*CurSt[5] + 0.08547009*CurSt[7];
  diff[4] = -5.40675214e-01*CurSt[1] - 4.51282051e-01*CurSt[2] - 7.72393162e-03*CurSt[4] - 1.12820513e-03*CurSt[5] - 0.09401709*CurSt[7];
  diff[5] = -1.38205128e-02*CurSt[1] - 1.61230769e+01*CurSt[2] - 1.97435897e-04*CurSt[4] - 4.03076923e-02*CurSt[5] - 0.02564103*CurSt[7];
  diff[6] = err;
}

void throttle_control(double * CurSt, double * diff, double T, double err)
{
  diff[8] = 0;
  diff[7] = 0;CurSt[7] = 0;
  diff[0] = CurSt[3];
  diff[1] = CurSt[4];
  diff[2] = CurSt[5];
  diff[3] =  4.60683761e-02*CurSt[1] + 4.10256410e-01*CurSt[2] + 6.58119658e-04*CurSt[4] + 1.02564103e-03*CurSt[5] + 0.08547009*T;
  diff[4] = -5.40675214e-01*CurSt[1] - 4.51282051e-01*CurSt[2] - 7.72393162e-03*CurSt[4] - 1.12820513e-03*CurSt[5] - 0.09401709*T;
  diff[5] = -1.38205128e-02*CurSt[1] - 1.61230769e+01*CurSt[2] - 1.97435897e-04*CurSt[4] - 4.03076923e-02*CurSt[5] - 0.02564103*T;
  diff[6] = err;
}

void sat_expr(double * CurSt, double * diff, double * param)
{
  double Kp = param[0],Ki = param[1], Kd = param[2], Theta_C = param[3];
  double err = error(Theta_C, CurSt[0]);
  double disturb = param[4]!=0 ? 0.1*cos(param[5]) : param[4];
  double u = Kp * err - Kd * CurSt[3] + Ki * CurSt[6];
  if (u > 0.14) {
    param[6] = 0;
    throttle_control(CurSt,diff,  0.14 + disturb, err);
  }
  else if (u > 0.07) {
    param[6] = 1;
    flywheel_control(CurSt,diff,  0.07 + disturb, err);
  }
  else if (u > -0.07) {
    param[6] = 1;
    flywheel_control(CurSt,diff,     u + disturb, err);
  }
  else if (u > -0.14) {
    param[6] = 1;
    flywheel_control(CurSt,diff, -0.07 + disturb, err);
  }
  else {
    param[6] = 0;
    throttle_control(CurSt,diff, -0.14 + disturb, err);
  }
}

void exe_expr(double * CurSt, double * diff, double * param)
{
  double Ang_Acc = param[0], direction = param[1], disturb = param[3];
  double Tfl = 0, T;
  double Tth = direction > 0 ? 0.14 : (direction < 0 ? -0.14 : 0);
  if (fabs(CurSt[6]) < 523.6) Tfl = - Ang_Acc * 0.0382;
  Tfl = fabs(Tfl)<0.07?Tfl:(Tfl>0?0.07:-0.07);
  T = Tfl + Tth + disturb;
  diff[0] = CurSt[3];
  diff[1] = CurSt[4];
  diff[2] = CurSt[5];
  diff[3] =  4.60683761e-02*CurSt[1] + 4.10256410e-01*CurSt[2] + 6.58119658e-04*CurSt[4] + 1.02564103e-03*CurSt[5] + 0.08547009*T;
  diff[4] = -5.40675214e-01*CurSt[1] - 4.51282051e-01*CurSt[2] - 7.72393162e-03*CurSt[4] - 1.12820513e-03*CurSt[5] - 0.09401709*T;
  diff[5] = -1.38205128e-02*CurSt[1] - 1.61230769e+01*CurSt[2] - 1.97435897e-04*CurSt[4] - 4.03076923e-02*CurSt[5] - 0.02564103*T;
  diff[6] = fabs(CurSt[6])>523.6?0:Ang_Acc;
}

double error(double dest, double cur)
{
    const double pi = 3.1415926535;
    double c,d,e;
    c = floor(fabs((dest - cur)/2/pi));
    d = dest > cur ? (dest - cur) - c*2*pi : (dest - cur) + c*2*pi;
    c = floor(fabs(d/pi));
    e = d > 0 ? d - 2*c*pi : d + 2*c*pi;
    return e;
}
