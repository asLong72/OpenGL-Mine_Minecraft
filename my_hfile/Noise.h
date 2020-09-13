#pragma once
#ifndef __NOISE_H__
#define __NOISE_H__

double Noise(int , int );
double SmoothedNoise(int , int );
double Cosine_Interpolate(double , double , double );
double InterpolatedNoise(float , float );

double PerlinNoise(float , float );

#endif // !__NOISE_H__
