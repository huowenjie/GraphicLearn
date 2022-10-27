#ifndef __SRT_GLOBAL_H__
#define __SRT_GLOBAL_H__

//-----------------------------------------------------------------------------
// 全局定义
//-----------------------------------------------------------------------------

// 圆周率
#define PI    3.1415926536f
#define DB_PI 6.2831853072f

// 全局定义函数

// 夹逼
float srtClampf(float val, float max, float min);

// 选择最值
float srtMaxf(float a, float b);
float srtMinf(float a, float b);

//-----------------------------------------------------------------------------

#endif // __SRT_GLOBAL_H__
