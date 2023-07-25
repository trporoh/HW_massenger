#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>

char* client_name;
char* massege_to_send;

mqd_t server;
mqd_t queue;

pthread_t check;

void getname(char** name); 
void* wait_for_massege(void* argv);

int send_massege();

int main(int args, char** arhv) {

	server = mq_open("/servers_buf", O_WRONLY);

	massege_to_send = (char*)malloc(512);

	getname(&client_name);
	strcpy(massege_to_send, client_name);

	pthread_create(&check, NULL, wait_for_massege, NULL);

	while (send_massege()) {
	}
}

void getname(char** name) {

	char* buffer ;
	*name = (char*)malloc(64);
	
	printf("Enter your name:\n");

	scanf("%s",*name);
	
	buffer = (char*)malloc(strlen(*name) + 1);
	buffer[0] = '/';
	strcat(buffer, *name);

	queue = mq_open(buffer, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, NULL);

	mq_send(server, buffer, strlen(buffer), 1);

	system("clear");
}

int send_massege() {

	char* string = malloc(512);
//	scanf("%s", string);
	gets(string,sizeof(string));

	strcat(massege_to_send, "\n");
	strcat(massege_to_send, string);

	mq_send(server, string, strlen(string), 1);

	return 1;

}

void* wait_for_massege(void* argv) {

	struct mq_attr* attr;
	char* received_string = malloc(512);

	while (1) {

		mq_getattr(queue, attr);

		mq_receive(queue, received_string, attr->mq_msgsize, NULL);

		if (strcmp(received_string, "")) {

			printf("%s", received_string);

		}
	}
	//free(attr);
	//free(received_string);
}