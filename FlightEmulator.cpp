#include "FlightEmulator.h"
#include <fstream>
#include <stdlib.h> 
#include <iostream>
#include <time.h>
#include <future>


FlightEmulator::FlightEmulator() {
	
	x.create_chaos();
	y.create_chaos();
	z.create_chaos();
	measurement_error = 0;
	open = false;
	

};

FlightEmulator::~FlightEmulator(){
	
}

void FlightEmulator::close_files()
{
	save_true.close();
	save_test.close();
	open = false;
}

void FlightEmulator::open_files()
{
	save_true.open("true.txt");
	save_test.open("test.txt");
	open = true;
};

void FlightEmulator::set_error(int measure_err_var)
{
	if (measurement_error == 0);
	measurement_error = measure_err_var;

};

void FlightEmulator::emulate(int interval)
{
	for (int i = 0; i < 100; i += interval)
	{
		save_emulation(i);
		x.make_move(i);
		y.make_move(i);
		z.make_move(i);
	}
	


};

void FlightEmulator::save_emulation(int time)
{
	if (!open)
		open_files();
	save_true << time << " ";
	save_true<<int(x.get_position()) << " ";
	save_true<<int(y.get_position()) << " ";
	save_true<<int(z.get_position()) << "\n";
	

	//teraz pacz
	//takiej chałtury dawno nie widziales
	save_test << time << " ";
	int generated =  measurement_error % (1+(rand() % 7));
	if(rand()%2==0)
	save_test << int (x.get_position() - generated )<<" ";
	else
	save_test << int (x.get_position() + generated )<<" ";
	if (rand() % 2 == 0)
	save_test << int(y.get_position() + generated)<< " ";
	else
	save_test << int(y.get_position() + generated)<< " ";
	if (rand() % 2 == 0)
	save_test << int(z.get_position() + generated)<< "\n";
	else
	save_test << int(z.get_position() + generated)<< "\n";
	

};

Movement_values::Movement_values()
{

};

Movement_values::Movement_values(double position, double Velocity, double acceleration, double measure_error)
{
	dVelocity = Velocity;
	dposition = position;
	dAcceleration = acceleration;
	dUncertain = measure_error* measure_error;
};

Movement_values::~Movement_values()
{

};

void Movement_values::set_values(double position, double Velocity, double acceleration, double measure_error)
{
	dVelocity = Velocity;
	dposition = position;
	dAcceleration = acceleration;
	dUncertain = measure_error* measure_error;
};

double Movement_values::get_position()
{
	return dposition;
}

void Movement_values::reset_values()
{
		dposition = 0;
		dVelocity = 0;
		dAcceleration = 0;
		duration_time = 0;
		dUncertain = 0, destimUncertain = 0;
}


void Emulation_values::create_chaos()
{
	srand(time(NULL));
	int a = rand() % 17487;
	dposition = a;
	dVelocity = rand() % a / 100;
	dAcceleration = a % 3;
	duration_time=0;
};

void Emulation_values::make_move(int time)
{
	dposition = dposition + (time * dVelocity) + (dAcceleration * time * time) / 2;
	dVelocity = dVelocity + dAcceleration * time;
};

void Filter_movement::Filter_Predict(int time) {
	std::cout << dposition + (time * dVelocity) + (dAcceleration * time * time) / 2;
	std::cout << dVelocity + dAcceleration * time;
};

int Filter_movement::Filter_Input(int time, int measurement) {
	duration_time += time;
	/* w chooj improwizacja*/
	//destimUncertain = (measurement-dpredPosition)*(measurement - dpredPosition)/((measurement-dposition)*(measurement-dposition));
	/*koniec improwizacji*/
	dpredPosition = dposition + (time * dVelocity) + (dAcceleration * time * time) / 2;
	dpredVel = dVelocity + dAcceleration * time;

	dposition = dpredPosition + dalpha * (measurement - dpredPosition);
	dVelocity = dpredVel + dbetha * (-1) * (dpredPosition - measurement) / time;
	dAcceleration = dAcceleration + dgamma * (-2) * (dpredPosition - measurement) / time / time;


	return dposition;

}

void Filter_movement::reset_fliter_val()
{
	reset_values();
	dpredVel = 0;
	dpredPosition = 0;

	dalpha = 0.5;
	dbetha = 0.4;
	dgamma = 0.1;
	duration_time= 0;
}
;