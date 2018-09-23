#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

void rungekutta(void odefun(double *, double *, double *),double * time, double stepsize, int order, double * CurSt, double * param); //四阶龙格库塔法 基尔公式

void flywheel_control(double * CurSt, double * diff, double T,double err);

void throttle_control(double * CurSt, double * diff, double T,double err);

void sat_expr(double * CurSt, double * diff, double * param);

void exe_expr(double * CurSt, double * diff, double * param);
