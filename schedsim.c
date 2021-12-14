#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Find the wait time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
  
   int i,t;
   int rem_bt[n];
  //make copy of processes
   for (i=0;i < n;i++){
     rem_bt[i] = plist[i].bt;
   }
  //initialize 0
  for (i=0;i<n;i++){
    plist[i].wt = 0;
  }
  for (i=0;i<n;i++){
    plist[i].wt = (plist[i].tat - plist[i].bt);
  }
  t=0;
  for (i=0;i<n;i++){
    if (rem_bt[i]>quantum){
      t = t + quantum;
      rem_bt[i] -= quantum;
    }
    else{
      t = t + rem_bt[i];
    }
  }
} 

// Function for wait time 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
      /*
       * 1 Traverse 
     */
     int i, comp_counter, comp_time, lap_num;
     comp_counter = 0;
     comp_time = 0;
     lap_num = 0;
  
     for (i=0;i<n;i++){
       plist[i].wt = ((plist[i].wt) - 1);
       if (plist[i].wt == 0){
         comp_counter++;
         lap_num++;
         comp_time = comp_time+1;
         plist[i].wt = ((comp_time) - (plist[i].art - plist[i].bt));    
       }
     }
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // WA for first process on arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating WA 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating TA time  
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
    ProcessType *this1 = (ProcessType*) &this;
    ProcessType *that1 = (ProcessType*) &that;
  
    if (this1->pri < that1->pri){
      return 1;
    }
//used to learn how to cast 
//https://stackoverflow.com/questions/23168364/error-request-for-member-in-something-not-a-structure-or-union
    return 0;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Find WT for all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Find TA time for processes 
    findTurnAroundTime(plist, n); 
  
    //Displays 
    printf("\n*********\nSJF\n");
}

//Calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Find WT for all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Find TA time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Displays  
    printf("\n********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
    qsort(plist, n, sizeof(ProcessType),my_comparer);
    findavgTimeFCFS(plist, n);
    //Display processes along with all details 
    printf("\n********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate wait time and turn  
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
// FCFS Part
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
// SJF Part
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
// Priority Part
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
// RR Part
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 