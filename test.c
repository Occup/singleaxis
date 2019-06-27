#include "NumInt.h"

int main(int argc, char * argv[])
{
    if (argc < 3) {
        printf("Not enough arguments....\n");
        return 0;
    }
    double time = 0;
    const double stepsize = 0.001;//时间步长
    int order = 9;//闭环实验阶数为8
    double * CurSt = (double *)malloc(sizeof(double) * order);
    double * param = (double *)malloc(sizeof(double) * 7);
    memset(CurSt,0,order);
    param[0] = 6; //Kp 比例系数 从用户界面获取
    param[1] = 0; //Ki 积分系数 从用户界面获取
    param[2] = 36.75;//Kd 微分系数 从用户界面获取
    param[3] = 3.14159 / 2 * 3; //Theta_C 角度指令 从用户界面获取
    param[4] = 0; //当param[4]为零时，不增加干扰，为其他任意值时增加干扰
    param[5] = 0; //当param[5]为常数时施加常值干扰，当为一个随time均匀变化的值时施加周期干扰
    param[6] = 0; //param[6]无需主动设置，程序通过读取此位确定哪一执行机构在工作
    // 当param[6] 为0，推力器工作，当param[6]为1，飞轮工作；
    sscanf(argv[1], "%lf", CurSt);
    sscanf(argv[2], "%lf", param + 3);
    while (time<1000){
        /*
          if (time>200&&time<700){
          param[4] = 1;
          //param[5] = time - 200;
          }
          else{
          param[4] = 0;
          param[5] = 0;
          }
        */
        rungekutta(sat_expr,&time,stepsize,order,CurSt,param);
        printf("%lf\t%lf\t%lf\n",time,CurSt[0],CurSt[3]);
        //处理与显示
        //可以在循环迭代中动态修改param参数实现立即改变显示效果
        /*
          CurSt[0] 台体角位置
          CurSt[3] 台体角速度
          CurSt[8] 飞轮角速度
          飞轮角加速度 = fabs(CurSt[8]) > 523 ? 0 : - CurSt[7] /  0.0382
        */
    }
    free(CurSt);
    free(param);
}
