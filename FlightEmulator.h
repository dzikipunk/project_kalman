#pragma once
#include <fstream>
#include <stdlib.h> 
#include <iostream>
#include <time.h>
#include <future>
#include <string>


class Movement_values{
public:
	double dposition = 0;
	double dVelocity = 0;
	double dAcceleration = 0;
	int duration_time = 0;
	double dUncertain = 0, destimUncertain = 0;
	
public:
	Movement_values();
	Movement_values(double,double,double, double);
	~Movement_values();
	void set_values(double, double, double, double);
	double get_position();
	void reset_values();
	

};

class Filter_movement : public Movement_values {
public:
	double dpredVel = 0;
	double dpredPosition = 0;

	double dalpha = 0.5;
	double dbetha = 0.4;
	double dgamma = 0.1;
	int duration_time=0;
	void Filter_Predict(int);
	int Filter_Input(int,int);
	void reset_fliter_val();


};
class Emulation_values :public Movement_values {
public:
	void create_chaos();
	void make_move(int);
};

class FlightEmulator
{
public:
	std::ofstream save_true;
	std::ofstream save_test;
	Emulation_values x, y, z;
	int measurement_error;
	bool open;
	

	void set_error(int);
	void emulate(int);
	void save_emulation(int);
	FlightEmulator();
	~FlightEmulator();
	void close_files();
	void open_files();
};


