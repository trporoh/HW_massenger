#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>

struct massege {
	char* name;
	char* massege;
};

struct list {
	struct massege* massege;
	struct list* next;
};

struct send_to {
	char** whom;
	char* string;
};

mqd_t server_buffer;

int check_function(struct list* args, char** names, int number_of_names);
void* send_to_client(void* args);


int main(int argc, char** argv) {

	struct list* tail = NULL;
	struct list* head = tail;

	char** clients = (char**)malloc(512);
	int number_of_clients = 0;
	int processing;

	server_buffer = mq_open("/servers_buf", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, NULL);
	while (1) {

		processing = check_function(tail, clients, number_of_clients);

		if (processing == 1) {
			number_of_clients += 1;
		}
	}

}

int check_function(struct list* args, char** names, int number_of_names) {

	char* string = (char*)malloc(512);
	int *status;

	struct mq_attr attr;
	struct send_to send_to;

	mq_receive(server_buffer, string, attr.mq_msgsize, NULL);

	if (!strcmp(string, "")) {
		sleep(1);
		return -1;
	}

	if (string[0] == '/') {

		printf("\033[1;33;40mFound massege with name of buffer.\033[0m\n");

		names[number_of_names] = malloc(strlen(string));
		strcpy(names[number_of_names], string);

		return 1;
	}

	printf("\033[1;32;40mFound massege to send.\033[0m\n");

	pthread_t send;
	send_to.string = (char*)malloc(strlen(string));
	strcpy(send_to.string, string);
	send_to.whom = names;

	pthread_create(&send, NULL, send_to_client, (void*)&send_to);

	struct massege* str = malloc(sizeof(struct massege));
	str->name = strtok(string, "\n");
	str->massege = strtok(string, "\0");
	if (NULL == args) {
		args = malloc(sizeof(struct list));
		args->massege = str;
		args->next = NULL;
	}
	else {
		args = args->next;
		args = malloc(sizeof(struct list));
		args->massege = str;
		args->next = NULL;
	}
	
	pthread_join(send, (void**) &status);

	return 0;
}


void* send_to_client(void* args) {

	int i = 0;
	char* queue;
	struct send_to* ptr = (struct send_to*)args;
	mqd_t open;

	while (ptr->whom[i]) {

		queue = ptr->whom[i];
		open = mq_open(queue, O_WRONLY);
		if (-1 == open) {
			perror("Smth went wrong in pthread");
			exit(EXIT_FAILURE);
		}
		mq_send(open, ptr->string, strlen(ptr->string), 1);

		mq_close(open);
		i++;

	}

	//exit(EXIT_SUCCESS);
}