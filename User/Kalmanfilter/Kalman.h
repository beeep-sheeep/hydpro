#ifndef Kalman_H
#define Kalman_H


typedef struct 
{
    double Last_P;//上次估算协方差 不可以为0 ! ! ! ! ! 
    double Now_P;//当前估算协方差
    double out;//卡尔曼滤波器输出
    double Kg;//卡尔曼增益
    double Q;//过程噪声协方差
    double R;//观测噪声协方差
}Kalman;

void Kalman_Init(void);
double KalmanFilter(Kalman *kfp,double input);

extern Kalman kfp;

#endif
