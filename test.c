#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFFERSIZE 5
#define LINESIZE 30

char buffer[BUFFERSIZE]; 			//아이템을 저장할 버퍼 
int item = 0; 						//생산과 소비 매체 
int line_size  = LINESIZE;			//생산과 소비를 실행할 횟수 
int buffer_size  = BUFFERSIZE;  	//버퍼 크기 지정 
int line = 0;						//현재 생산과 소비가 실행된 횟수 
int produce = 0;					//생산 횟수 
int consume = 0;					//소비 횟수 
int thread_count = 3;				//생산과 소비가 실행될 쓰레드 개수  

int produce_num[10]; //생산자쓰레드의 번호를 저장할 배열
int consume_num[10]; //소비자쓰레드의 번호를 저장할 배열

//세마포어변수
sem_t full;
sem_t empty;
sem_t mutex;

// 생산자 쓰레드가 실행할 함수 
int producer()
{
	int i = 0;
	
	item++;
    sem_wait(&empty);
    sem_wait(&mutex);
    
    //  쓰레드 종료  
    if(line >= line_size)
	{ 
        sem_post(&mutex);
        sem_post(&full);
        return -1;
    }
    
    // 버퍼 상태 변경  
    buffer[produce % buffer_size] = 'O';
    produce++;
    line++;
    
    //결과출력
    printf("[생산] "); 
    
    for(i = 0; i < buffer_size; i++)
        printf("%c", buffer[i]);
        
    printf("\n");
    
    sem_post(&mutex);
    sem_post(&full);
    
    return 0;
}

// 소비자 쓰레드가 실행할 함수  
int consumer()
{
	int i = 0;
	
	sem_wait(&full);
    sem_wait(&mutex);
	
	// 쓰레드 종료  
    if(line >= line_size)
	{
        sem_post(&mutex);
        sem_post(&empty);
        return -1;
    }
    
    // 버퍼 상태 변경  
    buffer[consume % buffer_size] = '_';
    consume++;
    line++;
    
    // 결과출력
    printf("[소비] "); 
    
    for(i = 0; i < buffer_size; i++)
        printf("%c", buffer[i]);
        
    printf("\n");
    
    sem_post(&mutex);
    sem_post(&empty);
    item--;
    
    return 0;
}

// 생산자 쓰레드
void *produce_thread(void *arg)
{ 
    while(1)
	{
        if(producer() == -1)
            pthread_exit(NULL);
    }
}
 
//소비자 쓰레드
void *consume_thread(void *arg)
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
	
	pthread_t thread_p[thread_count];		//생산자 쓰레드 id 저장 변수 
    pthread_t thread_c[thread_count];		//소비자 쓰레드 id 저장 변수 
	
	// 세마포어 초기화 
	if(sem_init(&full, 0, 0) != 0)
	{
        printf("sem_init Error\n");
        return 0;
    }
    if(sem_init(&empty, 0, buffer_size) != 0)
	{
        printf("sem_init Error\n");
        return 0;
    }
    if(sem_init(&mutex, 0, 1) != 0)
	{
        printf("sem_init Error\n");
        return 0;
    }
	
	// 버퍼 초기화  
    for(i = 0; i < 5; i++)
		buffer[i] = '_';    
	
	printf("< Item Buffer >\n");
    
    //생산자 쓰레드 생성
    for(i = 0; i < thread_count; i++)
	{
        status = pthread_create(&thread_p[i], NULL, produce_thread, NULL);
        if(status != 0)
            perror("producer thread");
    }
    //소비자 쓰레드 생성
    for(i = 0; i < thread_count; i++)
	{
        status = pthread_create(&thread_c[i], NULL, consume_thread, NULL);
        if(status != 0)
            perror("consumer thread");
    }
    
    //쓰레드가 모드 종료 될때까지 대기 
    for(i = 0; i < thread_count; i++)
	{
        pthread_join(thread_p[i], (void**)&status);
    }
     
    for(i = 0; i < thread_count; i++){
    	
        pthread_join(thread_c[i], (void**)&status);
    }

	// 세마포어 종료 
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);
     
    return 0;
}

