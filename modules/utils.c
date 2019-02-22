#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int top(sq_node_p stack_top)
{
	if(stack_top == NULL)
	{
		puts("STACK EMPTY");
		return SQ_EMPTY;
	}
	return stack_top->value;
}

int front(sq_node_p queue_front)
{
	if(queue_front == NULL)
	{
		puts("QUEUE EMPTY");
		return SQ_EMPTY;
	}
	return queue_front->value;
}

//FUNCTIONS HANDELING CHANGING THE STACK
sq_node_p push(sq_node_p stack_top, int val)
{
	sq_node_p new_node = (sq_node_p)malloc(sizeof(sq_node));
	new_node->value = val;
	new_node->next = stack_top;
	return new_node;
}

sq_node_p pop(sq_node_p stack_top)
{
	sq_node_p temp = stack_top;

	if(temp == NULL)
	{
		puts("STACK IS ALREADY EMPTY");
		return NULL;
	}

	stack_top = stack_top->next;

	int n = temp->value;
	free(temp);

	return stack_top;
}

//FUNCTIONS HANDELING CHANGING THE QUEUE
sq_node_p enque(sq_node_p front_of_queue, int val)
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

sq_node_p deque(sq_node_p front_of_queue)
{
	if(front_of_queue == NULL)
	{
		puts("QUEUE IS ALREADY EMPTY");
		return NULL;
	}

	sq_node_p temp = front_of_queue;

	front_of_queue = front_of_queue->next;

	free(temp); //dump the removed queue

	return front_of_queue;
}

void print_sq(sq_node_p sq)
{
	sq_node_p runner = sq;

	while(runner != NULL)
	{
		printf("%d  ", runner->value);
		runner = runner->next;
	}

	puts("");
}