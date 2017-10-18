#include <iostream>
#include<queue>
#include<cmath>
using namespace std;

double latency_reg = 0;
double latency_vip = 0;
int max_time; //Max time of simulation is 360 minute
queue <double> regular; //Queue of regular customers
queue <double> VIP; //Queue of VIP customers
double a; //mean of arrival time
double s; //mean of service time
int number_of_jobs_reg = 0; //Total number of served customers regular
int number_of_jobs_vip = 0; //Total number of served customers VIP
bool Server_busy; //Is server busy?
double next_arrival;  //Initially first event is arrival of first customer
double current_sim_time; //So this is the current simulation time that have event
double finish; //When customer finish his visit
double current_served_reg; //Currently served regular customer
double current_served_vip;
int days = 1000;
int c = 0;

double generate_exponential_time(double current, double x) {
	double f = 1;
	while (f == 1)
		f = ((double)rand() / RAND_MAX);
	return ceil(-1 * x * log(1 - f)) + current;
}

bool is_VIP() {
	int R = rand()%2;
	if (R==0)
		return true;
	else
		return false;
}


void reset() {
	//latency_reg = 0;
	//latency_vip = 0;
	max_time = 360; //Max time of simulation is 360 minute
	queue<double>regular; //Queue of regular customers
	queue<double>VIP; //Queue of VIP customers
	a = 1; //mean of arrival time
	s = 1.1; //mean of service time
	//number_of_jobs_reg = 0; //Total number of served customers regular
	//number_of_jobs_vip = 0; //Total number of served customers VIP
	Server_busy = false; //Is server busy?
	next_arrival = generate_exponential_time(0, a);  //Initially first event is arrival of first customer
	current_sim_time = next_arrival; //So this is the current simulation time that have event
	finish = 0; //When customer finish his visit
	current_served_reg = 0; ///Currently served regular customer
	current_served_vip = 0;
}

void ServeVIP() {
	finish = current_sim_time + generate_exponential_time(0, s);
	number_of_jobs_vip += 1;
	if (VIP.size() == 0)
		return;
	current_served_vip = VIP.front();
	latency_vip += finish - current_served_vip;
	VIP.pop();
}


void ServeReg() {
	finish = current_sim_time + generate_exponential_time(0, s);
	number_of_jobs_reg += 1;
	if (regular.size() == 0)
		return;
	current_served_reg = regular.front();
	latency_reg += finish - current_served_reg;
	regular.pop();
}


int main()
{
	while (c<days) {
		reset();
		while (true) {
			if ((Server_busy && (next_arrival<finish)) || (!Server_busy))
				current_sim_time = next_arrival;
			else
				current_sim_time = finish;

			// Time of simulation is at its end but we still have customers to be served
			if (current_sim_time > max_time) {
				while (VIP.size()>0) {
					ServeVIP();
				}
				Server_busy = false;

				while (regular.size() >0) {
					ServeReg();
				}
				Server_busy = false;
				break;
			}


			// If current event is new arrival
			if (current_sim_time == next_arrival) {
				if (regular.size()>0) {
					bool check = is_VIP();
					if (!check)
						regular.push(current_sim_time);
					else
						VIP.push(current_sim_time);
				}

				else
					regular.push(current_sim_time);
				next_arrival = generate_exponential_time(current_sim_time, a);
				if (!Server_busy) {
					Server_busy = true;
					if (VIP.size()>0) {
						ServeVIP();
					}
					else
						ServeReg();
				}

			}

			// IF current event is leaving
			else {
				if (VIP.size()>0)
					ServeVIP();
				else
					ServeReg();
				Server_busy = false;
			}

		}

		c++;
	}

	cout << "Av waiting for reg: " << latency_reg / number_of_jobs_reg;
	cout << "\nAv waiting for vip: " << latency_vip / number_of_jobs_vip;
	cout << "\nAv waiting for all: " << (latency_reg+latency_vip) / (number_of_jobs_reg+number_of_jobs_vip);
	cout <<"\nProfit gained through VIP: " << (number_of_jobs_vip*30)/days <<" L.E.";

	return 0;
}