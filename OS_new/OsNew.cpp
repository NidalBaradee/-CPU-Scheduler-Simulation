// CPU/Process Simulator Project for Operating System 
// Name: Nidal Baradee , id: 211001
// Dr.Hani Salah

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <cstring>


using namespace std;

struct Process {
	int id; // Process id
	int AT; // Arrival time
	int BT;    // Burst time
	int timeLeft; // For round robin
	int cs;
	int wt; // Waiting time
	int TAT_time; // Turnaround time
	int finishTime;  // Finish time
};
struct processOfSRTF {
	int pid;
	int arrival_time;
	int burst_time;
	int start_time;
	int completion_time;
	int turnaround_time;
	int waiting_time;
};


int totalWT = 0, totalTAT = 0, totalBurstTime = 0;
Process process[5];
int msize, psize, q, cs;
double avgTAT = 0, avgWT = 0;
int currentTime;
int numberOfProcesses;
double cpuUtilization = 0;

//function to read the data from file
void readFile() {

	ifstream inputFile;
	inputFile.open("processes.txt");
	if (!inputFile.is_open()) {
		cout << "cannt finputFiled the file " << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	//read file data 

	while (inputFile.good()) {
		inputFile >> numberOfProcesses;
		inputFile >> msize; // This variable likely represents the total memory size available for the system.
		inputFile >> psize; // This variable likely represents the size of each process that needs to be executed by the CPU.
		inputFile >> q;
		inputFile >> cs;

		for (int i = 0; i < numberOfProcesses; i++) {
			inputFile >> process[i].id >> process[i].AT >> process[i].BT;
		}
	}

	inputFile.close();
}

// gant chart for FCFS , SJF , RR
void GanttChart() {

	currentTime = 0;
	cout << "\n  ===================" << endl;
	cout << "//    Gantt Chart   //" << endl;
	cout << "  ===================" << endl << endl;
	cout << "================================================================" << endl;
	for (int i = 0; i < numberOfProcesses; i++) {
		cout << "(" << currentTime << ")|==P" << process[i].id << "==|";
		//to calculate current time after each process finished 
		currentTime += process[i].BT + cs;
	}
	currentTime = currentTime - cs;
	cout << "(" << currentTime << ")";
	cout << "\n================================================================" << endl;
}

void findWaitingTime(Process proc[], int n, int wt[]) {
	// waiting time for first process is 0
	wt[0] = 0;

	// calculat waiting time
	for (int i = 1; i < n; i++)
		wt[i] = proc[i - 1].BT + wt[i - 1] + proc[1].cs;
}

// calculate turn around time
void findTurnAroundTime(Process proc[], int n, int wt[], int tat[]) {
	// calculat turnaround time 
	for (int i = 0; i < n; i++)
		tat[i] = proc[i].BT + wt[i] + proc[1].cs;
}

//Function to calculate average time
void findavgTime(Process proc[], int n) {
	int wt[10], tat[10], totalWT = 0, totalTAT = 0;

	// waiting time for all processes
	findWaitingTime(proc, n, wt);

	// turn around time for all processes
	findTurnAroundTime(proc, n, wt, tat);

	//Display processes with all info
	cout << "\nProcesses  " << " Burst time  " << " Waiting time  " << " Turn around time\n";

	// total WT and total TAT

	double sum = 0;
	for (int i = 0; i < numberOfProcesses; i++)
	{
		totalWT = totalWT + wt[i];
		totalTAT = totalTAT + tat[i];
		sum = sum + proc[i].BT;
		cout << "   " << proc[i].id << "\t\t"
			<< proc[i].BT << "\t    " << wt[i]
			<< "\t\t  " << tat[i] << endl;
	}

	cout << "\nAverage waiting time = "
		<< (float)totalWT / (float)n;
	cout << "\nAverage turn around time = "
		<< (float)totalTAT / (float)n << endl;

	cout << "cpu utlization = " << (sum * 1.0 / (sum + 12)) * 100 << "%" << endl;
}


void sorty() {
	bool swapped;
	do { //Rearrange processes upwards depending on arrival time
		swapped = false;
		for (int i = 0; i < numberOfProcesses - 1; i++) {
			if (process[i].AT > process[i + 1].AT) {
				swap(process[i], process[i + 1]);
				swapped = true;
			}
		}
	} while (swapped);
}

//function to calculate & display the Shared Optimization Criteria in SJF & FCFS
void getInfo() {
	GanttChart(); //call GanttChat function to display it
	totalBurstTime = 0; totalWT = 0; totalTAT = 0; //reset totals
	process[0].finishTime = process[0].BT;
	process[0].wt = 0;
	process[0].TAT_time = process[0].finishTime - process[0].AT;

	for (int i = 1; i < numberOfProcesses; i++) { //to calculate waiting time & turnaround time & finish time for each process 
		totalBurstTime += process[i - 1].BT;
		process[i].finishTime = process[i - 1].finishTime + cs + process[i].BT;
		process[i].wt = process[i - 1].finishTime + cs - process[i].AT;
		process[i].TAT_time = process[i].finishTime - process[i].AT;
	}

	process[4].finishTime -= cs;

	for (int i = 0; i < numberOfProcesses; i++) { //to calculate the totals
		totalWT += process[i].wt;
		totalTAT += process[i].TAT_time;
	}

	totalBurstTime += process[numberOfProcesses - 1].BT;
	cpuUtilization = float(totalBurstTime * 100.0 / currentTime); //to calculate cpu utilization
	//To display Information for each process in table
	cout << "Process\t|    TAT Time   | Waiting Time\t| finish Time";
	cout << "\n";

	for (int i = 0; i < numberOfProcesses; i++) {
		cout << "P(" << process[i].id << ")\t|\t" << process[i].TAT_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].finishTime << endl;

	}

	// To display the Averages
	cout << "\nAverage Wating Time= " << (double)totalWT / (numberOfProcesses * 1.0) << endl;
	cout << "Average Turnaround Time= " << (double)totalTAT / (numberOfProcesses * 1.0) << endl;
	cout << "Cpu Utilization= " << cpuUtilization << "%" << endl;
	return;
}

// Function for FCFS
void FCFS() {
	cout << "First Come First is \n";
	sorty();
	getInfo();  // call getInfo function to display Gantt Chart & other information  
}

//Function o
void displayGanttChart(vector<pair<int, int>>& gantt) {
	// Print the top border of the Gantt chart
	cout << "+";
	for (auto& p : gantt) {
		cout << string(p.second * 4, '-') << "+"; // Adjust cell width to fit more information
	}
	cout << "\n|";

	// Print the process IDs with the start and end times in the Gantt chart
	int currentTime = 0;
	for (auto& p : gantt) {
		cout << "p" << p.first << "(" << currentTime << "-" << currentTime + p.second << ") |";
		currentTime += p.second;
	}

	cout << "\n+";
	for (auto& p : gantt) {
		cout << string(p.second * 4, '-') << "+"; // Match the width from the top border
	}
	cout << endl; // Removed the printing of the time line
}

// Function of STRF
void SRTF() {
	ifstream infile("processesOfSTRF.txt");
	if (!infile) {
		cerr << "Error opening file." << endl;
		return;
	}

	int n;
	infile >> n;
	struct processOfSRTF p[100];
	float avg_turnaround_time;
	float avg_waiting_time;
	float cpu_utilisation;
	int total_turnaround_time = 0;
	int total_waiting_time = 0;
	int total_idle_time = 0;
	int burst_remaining[100];
	int is_completed[100];
	memset(is_completed, 0, sizeof(is_completed));

	cout << setprecision(2) << fixed;

	for (int i = 0; i < n; i++) {
		infile >> p[i].arrival_time >> p[i].burst_time;
		p[i].pid = i + 1;
		burst_remaining[i] = p[i].burst_time;
	}
	infile.close();

	vector<pair<int, int>> gantt; // Stores pair of PID and duration for Gantt chart
	int current_time = 0;
	int completed = 0;
	int prev = 0;

	while (completed != n) {
		int idx = -1;
		int mn = 10000000;
		for (int i = 0; i < n; i++) {
			if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
				if (burst_remaining[i] < mn) {
					mn = burst_remaining[i];
					idx = i;
				}
				if (burst_remaining[i] == mn) {
					if (p[i].arrival_time < p[idx].arrival_time) {
						mn = burst_remaining[i];
						idx = i;
					}
				}
			}
		}

		if (idx != -1) {
			if (burst_remaining[idx] == p[idx].burst_time) { // process starts now
				p[idx].start_time = current_time;
				total_idle_time += p[idx].start_time - prev;
			}
			burst_remaining[idx] -= 1;
			if (gantt.empty() || gantt.back().first != p[idx].pid) {
				gantt.push_back({ p[idx].pid, 1 });
			}
			else {
				gantt.back().second++;
			}
			current_time++;
			prev = current_time;

			if (burst_remaining[idx] == 0) {
				p[idx].completion_time = current_time;
				p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
				p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

				total_turnaround_time += p[idx].turnaround_time;
				total_waiting_time += p[idx].waiting_time;

				is_completed[idx] = 1;
				completed++;
			}
		}
		else {
			current_time++;
		}
	}

	int min_arrival_time = 10000000;
	int max_completion_time = -1;
	for (int i = 0; i < n; i++) {
		min_arrival_time = min(min_arrival_time, p[i].arrival_time);
		max_completion_time = max(max_completion_time, p[i].completion_time);
	}

	avg_turnaround_time = (float)total_turnaround_time / n;
	avg_waiting_time = (float)total_waiting_time / n;
	cpu_utilisation = ((max_completion_time - total_idle_time) / (float)max_completion_time) * 100;

	cout << endl << endl;

	cout << "#P\t" << "ArrivalTime\t" << "BurstTime\t" << "StartTime\t" << "FinishTime\t" << "TAT\t" << "WT\t" << "\n" << endl;

	for (int i = 0; i < n; i++) {
		cout << p[i].pid << "\t" << p[i].arrival_time << "\t\t" << p[i].burst_time << "\t\t" << p[i].start_time << "\t\t" << p[i].completion_time << "\t\t" << p[i].turnaround_time << "\t" << p[i].waiting_time << "\t" << "\n" << endl;
	}
	cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
	cout << "Average Waiting Time = " << avg_waiting_time << endl;
	cout << "CPU Utilization = " << cpu_utilisation << "%" << endl;
	
	displayGanttChart(gantt);
}


// Function for Round Robin Scheduling with q time quantum 
void RR() {
	sorty();
	cout << "Round Robin\n";
	cout << " \n";
	int totalProcessTime, remain_processes, flag = 0;
	remain_processes = numberOfProcesses; // to decide how many processes remain (not finish)
	totalProcessTime = 0;
	int i;
	cout << "\n  ===================" << endl; // To display Gantt Chart 
	cout << "//    Gantt Chart   //" << endl;
	cout << "  ===================" << endl << endl;

	for (i = 0; i < numberOfProcesses; i++) { // To set each process's left time
		process[i].timeLeft = process[i].BT;
	}

	cout << "====================================================================================================" << endl;
	for (currentTime = 0, i = 0; remain_processes != 0;) {
		if (process[i].timeLeft > 0) { // Check if process is not finished
			cout << "(" << currentTime << ")|==P" << process[i].id << "==|";

			if (process[i].timeLeft <= q) { // If burst time is smaller than or equal to quantum 
				currentTime += process[i].timeLeft + cs;
				process[i].timeLeft = 0;
				flag = 1;
			}
			else { // If burst time is greater than quantum
				process[i].timeLeft -= q;
				currentTime += q + cs;
			}

			if (process[i].timeLeft == 0 && flag == 1) { // If we finish process 
				remain_processes--; // decrement number of processes
				process[i].finishTime = currentTime - cs;
				process[i].TAT_time = currentTime - process[i].AT; // to calculate turn around time
				process[i].wt = process[i].TAT_time - process[i].BT; // waiting time
				flag = 0;
			}
		}

		if (i == numberOfProcesses - 1) {
			i = 0; //if finish all processes
		}
		else if (process[i + 1].AT <= currentTime) {
			i++;
		}
		else {
			i = 0;
		}
	}

	cout << "(" << currentTime << ")" << endl; // Print the last current time
	cout << "====================================================================================================" << endl;
	totalWT = 0;
	totalTAT = 0;

	for (int i = 0; i < numberOfProcesses; i++) {
		//to calculate Total WT 
		totalWT += process[i].wt;
		//to calculate total TAT
		totalTAT += process[i].TAT_time;
	}

	cout << "Process\t|TurnAround Time| Waiting Time  |  Finish Time\n";
	totalBurstTime = 0;

	for (int i = 0; i < numberOfProcesses; i++) {
		//to set process turn around time
		cout << "P(" << process[i].id << ")\t|\t" << process[i].TAT_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].finishTime << endl;
		totalBurstTime += process[i].BT;
	}

	cout << endl;
	//to show Average times
	cout << "Avg Waiting time = " << (double)totalWT * 1.0 / numberOfProcesses << endl;
	cout << "Avg Turnaround time = " << (double)totalTAT / numberOfProcesses << endl;
	cpuUtilization = float(totalBurstTime * 100.0 / (currentTime));
	cout << "Cpu Utilization = " << cpuUtilization << "%" << endl;
}

int main() {

	readFile();
	cout << "Your file information:" << endl << endl;
	cout << "Process\t| Arrival Time  | Burst Time \n";

	for (int i = 0; i < numberOfProcesses; i++) {
		cout << "P(" << process[i].id << ")\t|\t" << process[i].AT << "\t|\t" << process[i].BT << "\n";
	}

	int choice;
	do {
		cout << "=======================================\n"
			<< "1) First-Come First-Served (FCFS)\n"
			<< "2)  Shortest Remaining Time First (SRTF)\n"
			<< "3) Round-Robin (RR)\n"
			<< "4) Exit Program\n"
			<< "Enter your choice: ";
		cin >> choice;

		switch (choice) {

		case 1:
			FCFS();
			system("pause");
			break;

		case 2:
			SRTF();
			system("pause");
			break;

		case 3:
			RR();
			system("pause");
			break;

		case 4:
			cout << "Bye Bye" << endl;;
			system("pause");
			break;

		default:
			cout << "Invalid Choice" << endl;
			system("pause");
			break;
		}

	} while (choice != 4);

	return 0;
}