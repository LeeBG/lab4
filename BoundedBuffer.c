#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFFERSIZE 5
#define LINESIZE 30

char buffer[BUFFERSIZE]; 		//will store item buffer 
int item = 0; 				//item 
int line_size  = LINESIZE;		//produce and consume will proceed NO 
int buffer_size  = BUFFERSIZE;  	//Size of Buffer 
int line = 0;				//now produce and consume proceeded NO
int produce = 0;			//Produce NO 
int consume = 0;			//consume NO
int thread_count = 3;			//proceed  thread NO  

//semaphore
sem_t full_sm;
sem_t empty_sm;
sem_t mutex_sm;

// Producer 
int producer()
{
	int i = 0;

	item++;
	sem_wait(&empty_sm);
	sem_wait(&mutex_sm);

	//  exit thread 
	if(line >= line_size)
	{ 
		sem_post(&mutex_sm);
		sem_post(&full_sm);
		return -1;
	}

	// Buffer state change  
	buffer[produce % buffer_size] = 'O';
	produce++;
	line++;

	//result
	printf("[Produce] "); 

	for(i = 0; i < buffer_size; i++)
		printf("%c", buffer[i]);

	printf("\n");

	sem_post(&mutex_sm);
	sem_post(&full_sm);

	return 0;
}

int consumer()			//customer
{
	int i = 0;

	sem_wait(&full_sm);
	sem_wait(&mutex_sm);

	// exit thread  
	if(line >= line_size)
	{
		sem_post(&mutex_sm);
		sem_post(&empty_sm);
		return -1;
	}

	// change buffer 
	buffer[consume % buffer_size] = '_';
	consume++;
	line++;

	// result
	printf("[Custom] "); 

	for(i = 0; i < buffer_size; i++)
		printf("%c", buffer[i]);

	printf("\n");

	sem_post(&mutex_sm);
	sem_post(&empty_sm);
	item--;

	return 0;
}


void *produce_thread(void *arg)		//producer thread
{ 
	while(1)
	{
		if(producer() == -1)
			pthread_exit(NULL);
	}
}


void *consume_thread(void *arg)		//customer thread
{
	while(1)
	{
		if(consumer() == -1)
			pthread_exit(NULL);
	}
}

int main()
{
	int i;
	int status;

	pthread_t thread_p[thread_count];		 
	pthread_t thread_c[thread_count];		 

	// init semaphore 
	if(sem_init(&full_sm, 0, 0) != 0)
	{
		printf("sem_init Error\n");
		return 0;
	}
	if(sem_init(&empty_sm, 0, buffer_size) != 0)
	{
		printf("sem_init Error\n");
		return 0;
	}
	if(sem_init(&mutex_sm, 0, 1) != 0)
	{
		printf("sem_init Error\n");
		return 0;
	}

	// init buffer  
	for(i = 0; i < 5; i++)
		buffer[i] = '_';    

	printf("< Item Buffer >\n");

	
	for(i = 0; i < thread_count; i++)			//producer
	{
		status = pthread_create(&thread_p[i], NULL, produce_thread, NULL);
		if(status != 0)
			perror("producer thread");
	}
	
	for(i = 0; i < thread_count; i++)			//customer
	{
		status = pthread_create(&thread_c[i], NULL, consume_thread, NULL);
		if(status != 0)
			perror("consumer thread");
	}

	//wait end thread 
	for(i = 0; i < thread_count; i++)
	{
		pthread_join(thread_p[i], (void**)&status);
	}

	for(i = 0; i < thread_count; i++){

		pthread_join(thread_c[i], (void**)&status);
	}

	// semaphore close 
	sem_destroy(&full_sm);
	sem_destroy(&empty_sm);
	sem_destroy(&mutex_sm);

	return 0;
}

