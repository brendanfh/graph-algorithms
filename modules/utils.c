#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int sq_top(sq_node_p stack_top)
{
	if(stack_top == NULL)
	{
		puts("STACK EMPTY");
		return SQ_EMPTY;
	}
	return stack_top->value;
}

int sq_front(sq_node_p queue_front)
{
	if(queue_front == NULL)
	{
		puts("QUEUE EMPTY");
		return SQ_EMPTY;
	}
	return queue_front->value;
}

//FUNCTIONS HANDELING CHANGING THE STACK
sq_node_p sq_push(sq_node_p stack_top, int val)
{
	sq_node_p new_node = (sq_node_p)malloc(sizeof(sq_node));
	new_node->value = val;
	new_node->next = stack_top;
	return new_node;
}

sq_node_p sq_pop(sq_node_p stack_top)
{
	sq_node_p temp = stack_top;

	if(temp == NULL)
	{
		puts("STACK IS ALREADY EMPTY");
		return NULL;
	}

	stack_top = stack_top->next;

	free(temp); //dump the removed top of stack

	return stack_top;
}

//FUNCTIONS HANDELING CHANGING THE QUEUE
sq_node_p sq_enque(sq_node_p front_of_queue, int val)
{
	sq_node_p new_node = (sq_node_p)malloc(sizeof(sq_node));
	new_node->value = val;
	new_node->next = NULL; //this will be at the back of the queue

	if(front_of_queue == NULL)
		front_of_queue = new_node;
	else
	{
		sq_node_p runner = front_of_queue;

		while(runner->next != NULL) //get to the end of the queue
		{
			runner = runner->next;
		}

		runner->next = new_node; //add new node to the end of the queue
	}

	return front_of_queue;
}

sq_node_p sq_deque(sq_node_p front_of_queue)
{
	sq_node_p temp = front_of_queue;

	if(temp == NULL)
	{
		puts("QUEUE IS ALREADY EMPTY");
		return NULL;
	}

	front_of_queue = front_of_queue->next;

	free(temp); //dump the removed front of queue

	return front_of_queue;
}


int sq_contains(sq_node_p sq, int val)
{
	while(sq != NULL)
	{
		if(sq->value == val)
			return 1;

		sq = sq->next;
	}

	return 0;
}

void sq_print(sq_node_p sq)
{
	sq_node_p runner = sq;

	while(runner != NULL)
	{
		printf("%d  ", runner->value);
		runner = runner->next;
	}

	puts("");
}

void sq_delete(sq_node_p sq)
{
	while(sq != NULL)
		pop(sq);
}

dj_node_p insert_dj(dj_node_p front_of_list, int id, int distance)
{
	dj_node_p new_node = (dj_node_p)malloc(sizeof(dj_node));
	new_node->next = NULL;
	new_node->node_id = id;
	new_node->distance = distance;

	if (front_of_list == NULL)
	{
		front_of_list = new_node;
		return front_of_list;
	}
	else
	{
		dj_node_p walker = front_of_list;
		while (walker->next != NULL)
		{
			walker = walker->next;
		}
		walker->next = new_node;
		return front_of_list;
	}
}

dj_node_p remove_dj(dj_node_p front_of_list, int id)
{
	dj_node_p walker = front_of_list;
	dj_node_p follower = walker;
	while (walker != NULL && walker->node_id != id)
	{
		follower = walker;
		walker = walker->next;
	}
	if (walker == NULL)
	{
		printf("bruh %d\n", id);
		return front_of_list;
	}
	else if (walker == front_of_list)
	{
		front_of_list = front_of_list->next;
		free(walker);
		return front_of_list;
	}
	else
	{
		follower->next = walker->next;
		free(walker);
		return front_of_list;
	}
}

int is_in_list(dj_node_p front_of_list, int id)
{
	dj_node_p walker = front_of_list;
	while (walker != NULL && walker->node_id != id)
	{
		walker = walker->next;
	}
	if (walker == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int get_distance(dj_node_p front_of_list, int id)
{
	dj_node_p walker = front_of_list;
	while (walker != NULL && walker->node_id != id)
	{
		walker = walker->next;
	}
	if (walker == NULL)
	{
		return -1;
	}
	else
	{
		return walker->distance;
	}
}

void delete_dj(dj_node_p front_of_list)
{
	dj_node_p follower = front_of_list;
	while (front_of_list != NULL)
	{
		follower = front_of_list;
		front_of_list = front_of_list->next;
		free(follower);
	}
}
