#ifndef Kalman_H
#define Kalman_H


typedef struct 
{
    double Last_P;//�ϴι���Э���� ������Ϊ0 ! ! ! ! ! 
    double Now_P;//��ǰ����Э����
    double out;//�������˲������
    double Kg;//����������
    double Q;//��������Э����
    double R;//�۲�����Э����
}Kalman;

void Kalman_Init(void);
double KalmanFilter(Kalman *kfp,double input);

extern Kalman kfp;

#endif
