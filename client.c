#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <curses.h>

#define SPACE "                                                                                                                        " //NEED MORE SPACES!!!

WINDOW* received_masseges;
WINDOW* text;

char* client_name;
char* massege_to_send;

mqd_t server;
mqd_t queue;

pthread_t check;

void getname(char** name); 
void* wait_for_massege(void* argv);
int send_massege();
void init();

int main(int args, char** arhv) {

	server = mq_open("/servers_buf", O_WRONLY);

	massege_to_send = (char*)malloc(512);

	getname(&client_name);
	strcpy(massege_to_send, client_name);

	system("clear");

	init();

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

	wmove(text, 1, 1);
	wgetstr(text, string);
	mvwprintw(text, 1, 1, SPACE);

	//strcat(massege_to_send, "\n");
	//strcat(massege_to_send, string);

	mq_send(server, string, strlen(string), 1);

	return 1;

}

void* wait_for_massege(void* argv) {

	struct mq_attr* attr;
	char* received_string = malloc(512);
	int ptr = 1;

	while (1) {

		mq_getattr(queue, attr);

		mq_receive(queue, received_string, attr->mq_msgsize, NULL);

		if (strcmp(received_string, "")) {

			mvwprintw(received_masseges, ptr, 1,received_string);
			//wmove(text, 1, 1);
			wrefresh(received_masseges);
			ptr += 4;

		}
	}
	//free(attr);
	//free(received_string);
}

void init() {

	initscr();
	curs_set(TRUE);
	refresh();

	received_masseges = newwin(30, 100, 0, 1);
	text = newwin(5, 100, 30, 1);

	box(received_masseges, '|', '-');
	box(text, '|', '-');

	wrefresh(received_masseges);
	wrefresh(text);

}