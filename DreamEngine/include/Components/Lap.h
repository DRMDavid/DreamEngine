#pragma once
#include "Prerequisites.h"

struct
	Lap {
	int   lap = 0;
	float currentLapTime = 0.f;
	float bestLapTime = std::numeric_limits<float>::infinity();
	float totalTime = 0.f;

	size_t checkpoint = 0;
	float  progressAlongSegment = 0.f;
};