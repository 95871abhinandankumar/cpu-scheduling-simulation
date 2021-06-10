

//Code is below of code description.


/*
   Code description :

        Every time when you call any processes scheduling algorithm (FCFS, RR, etc)
        it will read the file and store in process_table (it store the details of process);

        Note : every time after use any algorithm i.e FCFS, RR, etc
               free the memory of process_table  and Every time before
               using algorithm i.e RR, FCFS etc , read the file and 
               store the deatils of process in process table
   
        FCFS_Sheduling()  Function :  
			            First it will enqueue all process 
			            and then one by it will process 
			            the process i.e in the front 
			            of  queue till it finish and 
			            update the turnaournd time and 
			            waiting time of process.
			            after that it will  dequeue that 
			            process.

			            continue.... till dequeue all 
			            process

		RR_sheduling() Function :
		               First it will enqueue the first process
		               run upto given TIME QUANTAM .After that 
		               enqueue all process that has arrival time  
		               less than equal to current time.
		               and then enqueue the current process
		               and then call dequeue function.

		               mean while update the remaing cpu burst 
		               of current process.

		               if remaing cpu_burst of current process
		               less than equal to time quantam then 
		               update the wait and turn around of 
		               current process.

		               NOTE : In this function if queue is
		                      empty and we have still process
		                       whose arrival time is greater 
		                       than current time.
		                       the enqueue that process and 
		                       update the current time.

		                       it will repeat this algorithm
		                       time till all process end.

		SRBF_scheduling Function :

		                modification of RR_scheduling() function.
		                set quantam_time to 1.

		                after every time unit update the queue.

		                Important point : I called modify_queue() function
		                                to modify the queue. i.e put the 
		                                process in front of queue whose CPU_BURST
                                        is minimum.



*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

#define MAX_PROCESS 10000

//Structure to store data of process
typedef struct processes
{
	char *name;
	int arrival;
	int cpuburst;
	int turnaround;
	int wait;
	int total_cpuburst;
}process;

process *process_table[MAX_PROCESS];


//Queue data structure
typedef struct queue
{
 	int *arr;
 	int front;
 	int rear;
 	int max_size;

}Queue;

//Function to create new queue
Queue *create_queue(int size)
{
	Queue *q = (Queue *)malloc(sizeof(Queue));
	q->arr = (int *)malloc(sizeof(int) * size);
	q->rear = -1;
	q->front = -1;
	q->max_size = size;

	return  q;
}

//Function to Enqueue
void enqueue(Queue *q, int data)
{
	if(q->front == (q->rear + 1) % q->max_size)
	{
		printf("ERROR : Queue is full\n");
		return;
	}
    
    if(q->rear == -1)
    {
    	q->rear = 0;
    	q->front = 0;
    	q->arr[q->rear] = data;
    }else
    {
    	q->rear = (q->rear + 1) % q->max_size;
    	q->arr[q->rear] = data;
    }

}

int is_empty(Queue *q)
{
	if(q->front == -1)
		return 1;
	else 
		return -1;
}


//Function to dequeue
void dequeue(Queue *q)
{
	if(q->front == -1)
	{
		printf("ERROR : Queue is empty\n");
		return;
	}

	if(q->front == q->rear)
	{
		q->front = -1;
		q->rear = -1;
		
	}else{
		q->front = (q->front + 1) % q->max_size; 
	}
}

//Function to print
//process table
void printProcessTable()
{
	int average_turnaround = 0;
	int average_wait = 0;
    
    int i =0;
    printf("\nTurnaound time : ");
    while(process_table[i] != NULL)
    {
        
        printf("%s[%d] ", process_table[i]->name, process_table[i]->turnaround);
        average_turnaround += process_table[i]->turnaround;
    	i++;
    }
    printf("\nWait time : ");
    i = 0;
    while(process_table[i] != NULL)
    {
        
        printf("%s[%d] ", process_table[i]->name, process_table[i]->wait);
         average_wait += process_table[i]->wait;
    	i++;
    }
    
    printf("\n\nAverage turnaround time : %0.2f\n", (average_turnaround*1.0)/i);
    printf("Average wait time : %0.2f\n", (average_wait * 1.0)/i);

}


//Function for First Come First 
//Served Scheduling
void FCFS_scheduling()
{
   	Queue *q = create_queue(100);
    
    for(int i = 0 ;  process_table[i] != NULL ; i++)
    {
    	enqueue(q, i);
    }

    int current_time = process_table[0]->arrival;
    while(q->front != -1)
    {
        int w_time = current_time -process_table[q->front]->arrival;
        process_table[q->front]->wait = w_time;
        process_table[q->front]->turnaround = w_time + process_table[q->front]->cpuburst;
        
        int start_time = current_time;
        current_time += process_table[q->front]->cpuburst;

        printf("[%d-%d]    %s running\n", start_time, current_time, process_table[q->front]->name);



        dequeue(q);

    }
    
    printProcessTable();

    //Free the memory
    int i = 0;
    while(process_table[i] != NULL)
    {
    	//First free the memory of name
    	free(process_table[i]->name);

    	//now free the memory of process
    	free(process_table[i]);
    	i++;
    }


}//end of FCFS 


//Function of round robbin scheduling
void RR_scheduling(int time_quantam)
{
	Queue *q = create_queue(10000);
    
    int i = 0;
    int current_time = process_table[0]->arrival;
    enqueue(q, i++);
    while(q->front != -1 || process_table[i] != NULL)
    {
        
        if(q->front == -1  && process_table[i]->arrival > current_time)
        {

              current_time = process_table[i]->arrival;
              enqueue(q, i++);
        	
        }

        int start_time = current_time;
        if(process_table[q->arr[q->front]]->cpuburst <= time_quantam){
        
           current_time += process_table[q->arr[q->front]]->cpuburst;
           process_table[q->arr[q->front]]->cpuburst = 0;
           process_table[q->arr[q->front]]->turnaround = current_time - process_table[q->arr[q->front]]->arrival;
           process_table[q->arr[q->front]]->wait = process_table[q->arr[q->front]]->turnaround - process_table[q->arr[q->front]]->total_cpuburst;
           
           printf("[%d-%d]    %s running\n", start_time, current_time, process_table[q->arr[q->front]]->name);

           while(process_table[i] != NULL && process_table[i]->arrival <= current_time)
           {
           		enqueue(q, i++);
           }
           dequeue(q);

           

        }
        else
        {
            
            current_time += time_quantam;
            process_table[q->arr[q->front]]->cpuburst -= time_quantam;

            printf("[%d-%d]    %s running\n", start_time, current_time, process_table[q->arr[q->front]]->name);
            while(process_table[i] != NULL && process_table[i]->arrival <= current_time)
           {
           		enqueue(q, i++);
           }
           	    enqueue(q, q->arr[q->front]);
           	    dequeue(q);
        }

        
        

    }
    
    printProcessTable();

    //Free the memory
    i = 0;
    while(process_table[i] != NULL)
    {
    	//First free the memory of name
    	free(process_table[i]->name);

    	//now free the memory of process
    	free(process_table[i]);
    	i++;
    }

     
}
//end of round robbin

//Function modify_queue
void modify_queue(Queue *q)
{

	 int min = INT_MAX;
	 int index;
      for(int i = q->front ; i <= q->rear ; i++)
      {
      	   if(process_table[q->arr[i]]->cpuburst < min)
      	   {
      	   	     min = process_table[q->arr[i]]->cpuburst;
      	   	     index = i; 
      	   }
      }

      int temp = q->arr[q->front];
      q->arr[q->front] = q->arr[index];
      q->arr[index] = temp;

}

//Function for SRBF scheduling
void SRBF_scheduling()
{

	int time_quantam =1; 

	Queue *q = create_queue(10000);
    
    int i = 0;
    int current_time = process_table[0]->arrival;
     int start_time = current_time;
    enqueue(q, i++);
    while(q->front != -1 || process_table[i] != NULL)
    {
        

	//when queue is empty and we have still process that has to come 
	//and his arraival time is greater than current time. 
        if(q->front == -1  && process_table[i]->arrival > current_time)
        {

              current_time = process_table[i]->arrival;
              enqueue(q, i++);

        	
        }

       
	//if the process cpu burst is equal to 1 time unit
        if(process_table[q->arr[q->front]]->cpuburst == time_quantam){
        
           current_time += process_table[q->arr[q->front]]->cpuburst;
           process_table[q->arr[q->front]]->cpuburst = 0;
           process_table[q->arr[q->front]]->turnaround = current_time - process_table[q->arr[q->front]]->arrival;
           process_table[q->arr[q->front]]->wait = process_table[q->arr[q->front]]->turnaround - process_table[q->arr[q->front]]->total_cpuburst;
           
           printf("[%d-%d]    %s running\n", start_time, current_time, process_table[q->arr[q->front]]->name);

           start_time = current_time;

           while(process_table[i] != NULL && process_table[i]->arrival <= current_time)
           {
           		enqueue(q, i++);
           }
           dequeue(q);
           modify_queue(q);

        }// run the process which has less remaining cpu bust and update the current time
	//and put the process in front of queue which has lowest cpu burst.
        else
        {
            
            current_time += time_quantam;
            process_table[q->arr[q->front]]->cpuburst -= time_quantam;

            while(process_table[i] != NULL && process_table[i]->arrival <= current_time)
           {
           		enqueue(q, i++);
           }
           	    
           	    modify_queue(q);
           

        }

        
    }
    
    printProcessTable();

    //Free the memory
    i = 0;
    while(process_table[i] != NULL)
    {
    	//First free the memory of name
    	free(process_table[i]->name);

    	//now free the memory of process
    	free(process_table[i]);
    	i++;
    }
}
//End of SRBF scheduling




int main(int argc, char *argv[])
{
    
   
    

    int time_quantam;
    while(1){


    	       printf("\n\n\nPlease read the code description inside the code, To know how i have implemented\n\n");
                //reading data from file

				     if(argc == 2)
				    {
				    	//open file
				    	FILE *file_pointer = fopen(argv[1], "r");

				    	if(file_pointer == NULL)
				    	{
				    		printf("ERROR : Unable to open file!\n");
				    		exit(0);
				    	}else{

				    		char *buff = (char *)malloc(sizeof(char ) * 30);
				    		int arrival_time;
				    		int burst_time;
				            
				            int count = 0;
				    		while(fscanf(file_pointer , "%s %d %d", buff, &arrival_time, &burst_time) != EOF)
				    		{
				    			process *p = (process *)malloc(sizeof(process));
				    			p->name = buff;
				    			p->arrival = arrival_time;
				    			p->cpuburst = burst_time;
				                p->total_cpuburst = burst_time;

				                process_table[count++] = p;

				                buff = (char *)malloc(sizeof(char ) * 30);


				    		}
				    		process_table[count] = NULL;


				    	}
				    	fclose(file_pointer);
				    	
				    }
				    else
				    {
				    	printf("ERROR : Please Enter the valid file name!\n");
				    	exit(0);
				    }


    		printf("---------------------------------------------------------------------------\n");
	        printf("                       CPU Scheduling Simulation                           \n");
			printf("---------------------------------------------------------------------------\n");


			printf("Select the scheudling algorithm [1, 2, 3 or 4] : \n");
			printf("1. First Come First Served (FCFS)\n");
			printf("2. Round Robin (RR)\n");
			printf("3. SRBF\n");
			printf("4. Exit\n");

			int t;
			scanf("%d", &t);

			switch(t)
			{
				case 1:
					printf("----------------------------------------------------------------------------------------\n");
					printf("                         First Come First Served Scheduling                             \n");
					printf("----------------------------------------------------------------------------------------\n");
					FCFS_scheduling();
					break;
				case 2:
				    
				    printf("Enter the time quantam : ");
				    scanf("%d", &time_quantam);
					printf("----------------------------------------------------------------------------------------\n");
					printf("                                Round Robin Scheduling                             \n");
					printf("----------------------------------------------------------------------------------------\n");
					RR_scheduling(time_quantam);
					break;
				case 3:
					printf("----------------------------------------------------------------------------------------\n");
					printf("                                   SRBF Scheduling                             \n");
					printf("----------------------------------------------------------------------------------------\n");
					SRBF_scheduling();
					break;
				case 4:
					exit(0);
					break;
				default :
				    printf("WARNING : Invalid Input!\n");
						


			}

			printf("\nHit (ENTER key) or (any key + ENTER) to continue ...\n");
			getchar();
			getchar();
        
	}

	return 0;

}
