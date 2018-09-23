#include "NumInt.h"

void expr1()
{
  double * time = 0;
  const double stepsize = 0.001;//时间步长
  int order = 7;//开环实验方程为7阶
  double * CurSt = (double *)malloc(sizeof(double) * order);
  double * param = (double *)malloc(sizeof(double) * 2);
  param[0] = 0.333; // 飞轮的角加速度 可以取一定范围内的浮点数，具体范围待定，当值为零时不采用飞轮； 从用户界面获取
  param[1] = 1; //推力器喷气方向， 大小无所谓，为正时正向喷气，为负时负向喷气，为零时不采用推力器 从用户界面获取
  while (1){
    rungekutta(exe_expr,time,stepsize,order,CurSt,param);
    //处理与显示
    //可以在循环迭代中动态修改param参数实现立即改变显示效果
    /*
      CurSt[0] 台体角位置
      CurSt[3] 台体角速度
      CurSt[6] 飞轮角速度
      飞轮角加速度 为用户输入量，开环实验中一般为定值
     */
  }
}

void expr2()
{
  double * time = 0;
  const double stepsize = 0.001;//时间步长
  int order = 9;//闭环实验阶数为8
  double * CurSt = (double *)malloc(sizeof(double) * order);
  double * param = (double *)malloc(sizeof(double) * 7);
  param[0] = 0.333; //Kp 从用户界面获取
  param[1] = 1; //Ki 从用户界面获取
  param[2] = 0.5; //Kd 从用户界面获取
  param[3] = 0.75; //Theta_C 角度指令 从用户界面获取
  param[4] = 0; //当param[4]为零时，不增加干扰，为其他任意值时增加干扰
  param[5] = 0; //当param[5]为常数时施加常值干扰，当为一个随time均匀变化的值时施加周期干扰
  param[6] = 0; //param[6]无需主动设置，程序通过读取此位确定哪一执行机构在工作
  // 当param[6] 为0，推力器工作，当param[6]为1，飞轮工作；
  while (1){
    rungekutta(sat_expr,time,stepsize,order,CurSt,param);
    //处理与显示
    //可以在循环迭代中动态修改param参数实现立即改变显示效果
    /*
      CurSt[0] 台体角位置
      CurSt[3] 台体角速度
      CurSt[8] 飞轮角速度
      飞轮角加速度 = fabs(CurSt[8]) > 523 ? 0 : - CurSt[7] /  0.0382
     */
  }
}
int main()
{
  expr1();
  //通过配置不同的param来实现在expr2中实现多组实验
  expr2();
  return 0;
}
