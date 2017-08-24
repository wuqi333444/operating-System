#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>

void producer(int num, char item){
    int i = 0;
    int fdin;
    char item_with_num[33];
    char num_str[31];
    item_with_num[0] = item;
    item_with_num[1] = '\0';
    
    if ((fdin = open("/dev/scullbuffer0",O_WRONLY))== -1){//open device for writing
		perror("Could not open the device\n");
		exit(1);
	}
    sleep(2);	// sleep to allow other procs to finish open() call	
    // produce num items
    for (i = 1; i <= num; i++){
        // comstruct item
        sprintf(num_str, "%031d", i);
        strcat(item_with_num, num_str);
	item_with_num[32] = '\0';
        // write item 
        if(write(fdin,item_with_num,strlen(item_with_num)) == 0)
        {
		printf("%s\n", "Producer: Call to write returned 0, exiting");
        	break;
        }

	printf("Producer %c wrote item: %s\n", item, item_with_num);
        
        item_with_num[1] = '\0';
    }
     
    close(fdin);
    printf("%s\n", "after close() in producer");
}

void consumer(char id, int num){ 
    int i = 0;
    int fdout;
    char item_with_num[33];
    if ((fdout = open("/dev/scullbuffer0",O_RDONLY))== -1){//open device for writing
		perror("Could not open the device\n");
		exit(1);
	}
    // sleep to allow other procs to finish open() call
    sleep(2);	
    // consume num items
    for (i = 1; i <= num; i++){
	// read an item from the buffer	
        if(read(fdout,item_with_num,32) == 0)
        {
	    printf("%s\n", "Consumer: Call to read returned 0, exiting");
            break;
        }

	item_with_num[32] = '\0';
        
        printf("Consumer %c consumed: %s\n", id, item_with_num);
    }
    
    close(fdout);
    printf("%s\n", "after close() in consumer");
}

/*start of the main function*/
int main( int argc,  char* argv[] ){
	/*
	argv contains 1.producer number 2. items producer produce 3. consumer number 4. items consumer consumes
	*/
	int prod_num, prod_item_num, con_num, con_item_num;
	prod_num = atoi(argv[1]);
	prod_item_num = atoi(argv[2]);
	con_num = atoi(argv[3]);
	con_item_num = atoi(argv[4]);
	
//	printf("%s %d\n", "con_num =", con_num);
	pid_t child_id;
	

	int i = 0;
	pid_t cons[2];
	pid_t prod[2];
        // fork producer procs
	for(i = 0; i < prod_num; i++)
	{
		prod[i] = fork();
		if(prod[i] == 0)
	    	{
	        	producer(prod_item_num,(char)(65+i));
		    	return 0;
		}
	}
        // fork consumer procs
	for(i = 0; i < con_num; i++)
	{
		cons[i] = fork();
		if(cons[i] == 0)
	    	{
	        	consumer((char)(65+i),con_item_num);
	        	return 0;	
	    	}
	}
	
	
	printf("%s\n", "main: Waiting on child processes");
        // wait on all producer procs
	for(i = 0; i < prod_num; i++)
	{
		waitpid(prod[i], NULL, 0);
	}
        // wait on all consumer procs
	for(i = 0; i < con_num; i++)
	{
		waitpid(cons[i], NULL, 0);
	}

	printf("%s\n", "main: finished waiting");
	
	return 0;
}










