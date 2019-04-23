#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* You will need a structure to store the information to be passed to each thread (see "Passing Parameters to Each Thread" in the textbook's project description)*/
typedef struct 
{
   int start_i;
   int end_i;
   int start_j;  
   int end_j;
   int arrSpot;
} parameters;
/* the thread that checks columns */
void *column_worker(void *param); 	// the function parameter is for the structure you defined
void *row_worker(void *param);
void *subGrid_worker(void *param);
/* You will also need to declare threads that checks rows and 3x3 subgrids */
int checkArray();
/* You are suggested to declare an array of integer values that is visible to each thread. The value in the array (0 or 1) indicates whether the worker thread's number is valid (see "Returning Results to the Parent Thread" in the textbook's project description) */

//index 0 and 1 are form column and row respectivly and the rest are for 3x3s
int arr[11];
int grid[9][9];
int main(int argc, char *argv[])
{	
	int i, j;
        printf("argc: %d/n",argc);
	if(argc < 2){
	   printf("Please pass in the grid param!\n");
	   exit(0);
        }
	printf("\n");
	int c;
	FILE *fp;
	fp = fopen(argv[1], "r");
	i = 0;
	j = 0;
	if(fp){
	   while((c = getc(fp)) != EOF){
	      if(c != ' ' && c != '\n')	{
	         grid[i][j] = c - '0';
		 j++;
	      }
	      if(c == '\n'){
		 i++;
		 j = 0;
	      }
	     //putchar(c);
           }
	   fclose(fp);
	}
	printf("\n");
	
	for(i = 0; i < 9; i++){
	  for(j = 0; j < 9; j++)
	     printf("%d ",grid[i][j]);
	  printf("\n");
	}


	/*You need to assign values to the structure variable. Then you can create multiple worker threads by passing the information using the structure variable*/
	parameters *data = (parameters*) malloc(sizeof(parameters));
	data->start_i = 0;
	data->end_i = 3;
	data->start_j = 0;
	data->end_j = 3;
	data->arrSpot = 2;

	pthread_t threads[11];

        pthread_create(&threads[0], NULL, column_worker, NULL);
        pthread_join(threads[0],NULL);	
	pthread_create(&threads[1], NULL, row_worker, NULL);
	pthread_join(threads[1],NULL);	
	
	int totalJ = 3;
	for(i = 2; i < 11; i++)
	{
	   pthread_create(&threads[i], NULL, subGrid_worker, data);
	   pthread_join(threads[i],NULL);

	   if(totalJ == 9){
	      totalJ = 3;
              data->start_i += 3;
	      data->end_i += 3;
	      data->start_j = 0;
	      data->end_j = 3;
	   }else{	
	      data->start_j += 3;
	      data->end_j += 3;
	   }
	   data->arrSpot += 1;	
	}

	/*You need to call pthread_join() for each childer thread so that the parent will wait*/
	
	/* Finally, after all children returns, you can check the status array that is visible to everyone and see if it is valid. You then print out the final checking result*/
	
	printf("\n %d \n", arr[1]);
	int isGood = checkArray();
        if(isGood == 1)
	   printf("The puzzle is good!\n");
	else
	   printf("The puzzle is bad!\n");
	return 0;
	
}

int checkArray(){
   int i;
   for(i = 0; i < 11; i++)
      if(arr[i] == 0)
         return 0;
   return 1;
}
/*thread code for child checking all columns*/
void *column_worker(void *params)
{
   int seen[] = {0,0,0,0,0,0,0,0,0};
   int i, j, k;
   for(i = 0; i < 9; i++){
      for(j = 0; j < 9; j++)
	 if(seen[grid[i][j]-1] == 0)
            seen[grid[i][j]-1] = 1;
	 else{
	    arr[1] = 0;
	    return;
 	 }
     for(k = 0; k < 9; k++)
         seen[k] = 0;
   }
   arr[0] = 1;
   pthread_exit(0);
}

void *row_worker(void *params)
{
   int seen[] = {0,0,0,0,0,0,0,0,0};
   int i, j, k;
   for(j = 0; j < 9; j++){
      for(i = 0; i < 9; i++)
	 if(seen[grid[i][j]-1] == 0)
            seen[grid[i][j]-1] = 1;
	 else{
	    arr[1] = 0;
	    return;
 	 } 
     for(k = 0; k < 9; k++)
        seen[k] = 0;
   }
       
   arr[1] = 1;
   pthread_exit(0);	
}

void *subGrid_worker(void *params)
{
   parameters *data = params;
   int seen[] = {0,0,0,0,0,0,0,0,0};
   int i,j,k;
   for(i = data->start_i; i < data->end_i; i++){
      for(j = data->start_j; j < data->end_j; j++)
         if(seen[grid[i][j]-1] == 0)
            seen[grid[i][j]-1] = 1;
	 else{
	    arr[data->arrSpot] = 0;
	    return;
         }
      for(k = 0; k < 9; k++)
	seen[k] = 0;
   }
   arr[data->arrSpot] = 1;
   //printf("start_i: %d\n", data->start_i);
   pthread_exit(0);
}

/* also need to define threads for checking rows and 3x3 subgrids */
