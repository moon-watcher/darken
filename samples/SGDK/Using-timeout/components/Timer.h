#pragma once

typedef struct ComponentTimer
{
	unsigned int counter;
	unsigned int max;

	void (*trigger)();
	void *param1;
	void *param2;
	void *param3;
} ComponentTimer;