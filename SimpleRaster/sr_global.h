#ifndef __SR_GLOBAL_H__
#define __SR_GLOBAL_H__

//-----------------------------------------------------------------------------
// 全局定义
//-----------------------------------------------------------------------------

// 圆周率
#define PI    3.1415926536f
#define DB_PI 6.2831853072f

// 全局定义函数

// 夹逼
float srClampf(float val, float max, float min);

// 选择最值
float srMaxf(float a, float b);
float srMinf(float a, float b);

//-----------------------------------------------------------------------------

// 随机数
void initRandSeed();

// 随机浮点数
float randomf(float min, float max);

// 随机整数
int randomi(int min, int max);

// 随机布尔值
bool randomb();

//-----------------------------------------------------------------------------

#endif // __SR_GLOBAL_H__
