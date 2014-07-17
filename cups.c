#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct CupState{
	int volume_x;
	int volume_y;
	struct CupState * parent_state;
}CupState;

// Code for the linked list and queue
struct Node{
	CupState value;
	struct Node * next;
};

typedef struct{
	struct Node * head;
	struct Node * tail;
	int size;
} Queue;

Queue queue_initialize(){
	CupState zero_cup_state = {0,0};
	Queue new_queue = {NULL , NULL , 0};
	return new_queue;
}

int queue_size(Queue * current_queue){
	return current_queue->size;
}

CupState queue_front(Queue * current_queue){
	if(!current_queue->head){
		CupState zero_cup_state = {0,0};
		return zero_cup_state;
	}
	else 
		return (current_queue->head)->value;
}

void queue_insert(Queue * current_queue , const CupState new_value){
	struct Node * ptr_new_node = malloc(sizeof (struct Node));

	ptr_new_node->value = new_value;
	ptr_new_node->next = NULL;

	if(current_queue->tail){
		struct Node * current_last_node = current_queue->tail;
		current_last_node->next = ptr_new_node;
	}

	current_queue->tail = ptr_new_node;

	if(!current_queue->head)
		current_queue->head = ptr_new_node;

	current_queue->size++;
}

void queue_pop(Queue * current_queue){
	struct Node * current_head_node = current_queue->head;
	current_queue->head = current_head_node->next;
	free(current_head_node);

	if(!current_queue->head)
		current_queue->tail = NULL;

	current_queue->size--;
}
// End of linked list code and queue

int MAX_VOLUME_X;
int MAX_VOLUME_Y;
int TARGET_VOLUME;

int minimum(const int a, const int b){
	if(a < b) return a;
	else return b;
}

CupState pour_from_y_to_x(const CupState * current_state){ //take note that this returns a pointer!
	CupState new_state = {0,0,current_state};
	new_state.volume_x = current_state->volume_x;
	new_state.volume_y = current_state->volume_y;

	new_state.volume_x = minimum(new_state.volume_x + new_state.volume_y , MAX_VOLUME_X);
	new_state.volume_y -= (new_state.volume_x - current_state->volume_x); 
	return new_state;
}

CupState pour_from_x_to_y(const CupState * current_state){ //take note that this returns a pointer!
	CupState new_state = {0,0,current_state};
	new_state.volume_x = current_state->volume_x;
	new_state.volume_y = current_state->volume_y;

	new_state.volume_y = minimum(new_state.volume_y + new_state.volume_y , MAX_VOLUME_Y);
	new_state.volume_x -= (new_state.volume_y - current_state->volume_y); 
	return new_state;	
}

CupState empty_x(const CupState * current_state){
	CupState new_state = {0,0,current_state};
	new_state.volume_x = 0;
	new_state.volume_y = current_state->volume_y;
	return new_state;
}

CupState empty_y(const CupState * current_state){
	CupState new_state ={0,0,current_state};
	new_state.volume_y = 0;
	new_state.volume_x = current_state->volume_x;
	return new_state;
}

CupState fill_x(const CupState * current_state){
	CupState new_state = {0,0,current_state};
	new_state.volume_x = MAX_VOLUME_X;
	new_state.volume_y = current_state->volume_y;
	return new_state;
}

CupState fill_y(const CupState * current_state){
	CupState new_state = {0,0,current_state};
	new_state.volume_y = MAX_VOLUME_Y;
	new_state.volume_x = current_state->volume_x;
	return new_state;
}

bool isTargetVolume(const CupState * current_state){
	return current_state->volume_x == TARGET_VOLUME || current_state->volume_y == TARGET_VOLUME;
}

int main(int args , char * argv[])
{
	if(args == 3){
		MAX_VOLUME_X = atoi(argv[1]);
		MAX_VOLUME_Y = atoi(argv[2]);
		TARGET_VOLUME = atoi(argv[3]);
	}
	else {
		MAX_VOLUME_X = 9;
		MAX_VOLUME_Y = 4;
		TARGET_VOLUME = 2;
	}

	// Variable declarations
	CupState initial_state = {0 ,0 , NULL};
	CupState current_state;
	CupState * ptr_current_state;
	bool found_solution = false;

	// Some bfs initializations
	Queue bfs_queue = queue_initialize();
	bool visited_states[200][200];
	memset(visited_states , false , sizeof visited_states);
	queue_insert(&bfs_queue , initial_state);
	
	while(queue_size(&bfs_queue) > 0){

		current_state = queue_front(&bfs_queue);
		queue_pop(&bfs_queue);
		ptr_current_state = malloc(sizeof(CupState));
		ptr_current_state->volume_x = current_state.volume_x;
		ptr_current_state->volume_y = current_state.volume_y;
		ptr_current_state->parent_state = current_state.parent_state;

		if(visited_states[current_state.volume_x][current_state.volume_y])
			continue;

		visited_states[current_state.volume_x][current_state.volume_y] = true;
		if(isTargetVolume(ptr_current_state)){ //some sort of code to be called on success
			do {
				printf("(%d , %d)\n" , ptr_current_state->volume_x , ptr_current_state->volume_y);
				ptr_current_state = ptr_current_state->parent_state;
			}while(ptr_current_state);
			found_solution = true;
			break;
		}

		CupState next_state;
		int i;
		for(i = 0 ; i < 6 ; i++){
			switch(i){
				case 0 : next_state = pour_from_x_to_y(ptr_current_state); break;
				case 1 : next_state = pour_from_y_to_x(ptr_current_state); break;
				case 2 : next_state = empty_y(ptr_current_state); break;
				case 3 : next_state = empty_x(ptr_current_state); break;
				case 4 : next_state = fill_y(ptr_current_state); break;
				case 5 : next_state = fill_x(ptr_current_state); break;
			}
			if(!visited_states[next_state.volume_x][next_state.volume_y]){
				queue_insert(&bfs_queue , next_state);
			}
		}
	}
	if(!found_solution)
		puts("No possible soluton!");

	return 0;
}