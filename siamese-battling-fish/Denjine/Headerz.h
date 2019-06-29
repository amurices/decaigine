#pragma once

#include <Windows.h>


#include <SDL\SDL.h>
#include <GL\glew.h>
#include <SOIL\SOIL.h>
#include <GLM\glm.hpp>
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>
#include <Box2D\Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include <algorithm>


// Important constants
static const float		PI = 3.14159265358979323846f;
static const float		PI_OVER_360 = PI / 360.0f;
static const float		SQRT_2 = 0.70710678118;

// Convert radians to degrees
static inline double RadToDeg(const double deg)	{
	return deg * 180.0 / PI;
};

// reverse of that
static inline double DegToRad(const double rad)	{
	return rad * PI / 180.0;
};

// Rich blames Microsoft...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

typedef unsigned int uint;
