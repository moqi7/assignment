#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<semaphore.h>

#define BUFFER_SIZE 5
typedef int bufferitem;

sem_t sem,empty,full;
bufferitem buffer[BUFFER_SIZE];
int head,rear;

void *insert_item(void *arg){
	int i,j,k,l;
	srand(time(NULL));
	sleep(rand()%5);
	i=sem_wait(&empty);
	if(i==-1){
		printf("error\n");
		exit(1);
	}
	j=sem_wait(&sem);
	if(j==-1){
		printf("error\n");
		exit(1);
	}
	buffer[rear%BUFFER_SIZE]=rand();
	rear++;
	k=sem_post(&sem);
	if(k==-1){
		printf("error\n");
		exit(1);
	}
	l=sem_post(&full);
	if(l==-1){
		printf("error\n");
		exit(1);
	}
}

void *remove_item(void *arg){
	int i,j,k,l;
	srand(time(NULL));
	sleep(rand()%5);
	i=sem_wait(&full);
	if(i==-1){
		printf("error\n");
		exit(1);
	}
	j=sem_wait(&sem);
	if(j==-1){
		printf("error\n");
		exit(1);
	}
	printf("%d ",buffer[head%BUFFER_SIZE]);
	head++;
	k=sem_post(&sem);
	if(k==-1){
		printf("error\n");
		exit(1);
	}
	l=sem_post(&empty);
	if(l==-1){
		printf("error\n");
		exit(1);
	}
}

int main(int argc,char *argv[]){
	int t,p_num,c_num;
	sscanf(argv[1],"%d",&t);
	sscanf(argv[2],"%d",&p_num);
	sscanf(argv[3],"%d",&c_num);
	int i,j,k;
	pthread_t p[p_num],c[c_num];
	

	i=sem_init(&sem,0,4);
	if(i==-1){
		printf("error\n");
		exit(1);
	}
	j=sem_init(&empty,0,BUFFER_SIZE);
	if(j==-1){
		printf("error\n");
		exit(1);
	}
	k=sem_init(&full,0,0);
	if(k==-1){
		printf("error\n");
		exit(1);
	}
	head=0; rear=0;


	for(i=0;i<p_num;i++){
		j=pthread_create(&p[i],NULL,insert_item,NULL);
		if(j==-1){
			printf("error\n");
			exit(1);
		}
	}
	for(j=0;j<c_num;j++){
		k=pthread_create(&c[j],NULL,remove_item,NULL);
		if(k==-1){
			printf("error\n");
			exit(1);
		}
	}

	sleep(t);
	printf("\n");
	sem_destroy(&sem);
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;
}
