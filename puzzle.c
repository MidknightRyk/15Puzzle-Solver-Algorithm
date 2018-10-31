/**
 * Assignment 2 COMP20003
 * Last edited on 19th October 2018
 * 15 puzzle solver using manhattan heuristic approach
 * Written by Marishka N. Magness
 * Student ID: 805654
 * University of Melbourne
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>

#define STATE_SIZE 16
#define MOVES 4
#define ROW1 4
#define ROW2 8
#define ROW3 12
#define COL1 1
#define COL2 2
#define COL3 3
#define TRUE 1
#define FALSE 0

/*Function Declaration */
int get_position(int tile, int is_x);

/**
 * READ THIS DESCRIPTION
 *
 * node data structure: containing state, g, f
 * you can extend it with more information if needed
 */
typedef struct node{
	int state[STATE_SIZE];
	int g; //Cost of path up to s from initial state
	int f; //f(a,s) = returns state after a is performed on s
	int op; //operation performed to get this state from parent
} node_t;

/**
 * Global Variables
 */

// used to track the position of the blank in a state,
// so it doesn't have to be searched every time we check if an operator is applicable
// When we apply an operator, blank_pos is updated
int blank_pos;

// Initial node of the problem
node_t initial_node;

// Statistics about the number of generated and expendad nodes
unsigned long generated;
unsigned long expanded;


/**
 * The id of the four available actions for moving the blank (empty slot). e.x.
 * Left: moves the blank to the left, etc.
 */

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

/*
 * Helper arrays for the applicable function
 * applicability of operators: 0 = left, 1 = right, 2 = up, 3 = down
 */
int ap_opLeft[]  = { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 };
int ap_opRight[]  = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
int ap_opUp[]  = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int ap_opDown[]  = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
int *ap_ops[] = { ap_opLeft, ap_opRight, ap_opUp, ap_opDown };


/* print state */
void print_state( int* s )
{
	int i;

	for( i = 0; i < 16; i++ )
		printf( "%2d%c", s[i], ((i+1) % 4 == 0 ? '\n' : ' ') );
}

void printf_comma (long unsigned int n) {
    if (n < 0) {
        printf ("-");
        printf_comma (-n);
        return;
    }
    if (n < 1000) {
        printf ("%lu", n);
        return;
    }
    printf_comma (n/1000);
    printf (",%03lu", n%1000);
}

/* return the sum of manhattan distances from state to goal */
int manhattan( int* state )
{
	int sum = 0;
	int ti[] = {0,0};
	int tiprime[] = {0,0};

	 //tile current position
	 int i;
	 for(i = 0; i < STATE_SIZE; i++){
		 //Exclude the blank space
		 if(state[i] == 0){
			 continue;
		 }
		 //Calculate the manhattan sum
		 if(state[i] != i){
			 ti[0] = get_position(state[i], TRUE);
			 ti[1] = get_position(state[i], FALSE);
			 tiprime[0] = get_position(i, TRUE);
			 tiprime[1] = get_position(i, FALSE);
			 sum += abs(ti[0] - tiprime[0]) + abs(ti[1] - tiprime[1]);
		 }
	 }

	return( sum );
}

/* returns eiher the x or y value when given a position of 0-16 */
int get_position(int tile, int is_x){
	if(is_x == TRUE){
		return (tile - (tile < ROW1 ? 0
									: (tile < ROW2 ? ROW1
									: (tile < ROW3 ? ROW2 : ROW3))));
	} else {
		return (tile < ROW1 ? 0 : (tile < ROW2 ? COL1
												: (tile < ROW3 ? COL2 : COL3)));
	}
}

/* return 1 if op is applicable in state, otherwise return 0 */
int applicable( int op )
{
       	return( ap_ops[op][blank_pos] );
}


/* apply operator */
void apply( node_t *n, int op )
{
	int tile;

	//find tile that has to be moved given the op and blank_pos
	tile = blank_pos + (op == LEFT ? -1 : (op == RIGHT ? 1 : (op == UP ? -4 : 4)));
	/*printf("apply section info:\n op = %d, tile = %d, blank_pos = %d",
						op, tile, blank_pos);*/

	//apply op
	n->state[blank_pos] = n->state[tile];
	n->state[tile] = 0;

	//update blank pos
	blank_pos = tile;



}

/* Checks if move returns to parent */
int is_opp_move(int op_prev, int op_now){
	if ((op_prev == LEFT && op_now == RIGHT)
	|| (op_prev == RIGHT && op_now == LEFT)
	|| (op_prev == UP && op_now == DOWN)
	|| (op_prev == DOWN && op_now == UP)) {
		return TRUE;
	}
	return FALSE;
}

/* Recursive IDA */
node_t* ida( node_t *node, int threshold, int *newThreshold ){
	node_t *new_node;
	new_node = malloc(sizeof(*new_node));
	assert(new_node!=NULL && node!=NULL);
	node_t *r1 = NULL;
	int action[MOVES];
	int op, i = 0, blankpos = blank_pos;


	/**
	 * FILL WITH YOUR CODE
	 *
	 * Algorithm in Figure 2 of handout
	 */

	 /*create new node for applied actions*/
	 //copy current state into new node
	 memcpy(new_node, node, sizeof(*new_node));


	 //create table of possible moves

	 for(op = 0; op < MOVES; op++){
		 	if(applicable(op) == 1 && is_opp_move(node->op, op) == 0){
				action[i] = op;
				i++;
			}
	 }

	 //Apply action for all possible actions
	 for(op = 0; op < i; op++){
		 generated++;
		 apply(new_node, action[op]);
		 new_node -> g = (node -> g)+1;
		 new_node -> f = (new_node -> g) + manhattan(new_node -> state);
		 new_node -> op = action[op];

		 //Check if threshold for continuing this path goes past threshold
		 if(new_node -> f > threshold){
			 *newThreshold = (new_node->f > *newThreshold) ? *newThreshold
			 																							 : new_node->f;

			//Reset new_node to node
			memmove(new_node->state, node->state, sizeof(new_node->state));
			blank_pos = blankpos;
			new_node->g --;
			new_node->op = node->op;
			continue;
		 }

		 //Expanded nodes that are still below the threshold
		 expanded ++;
		 //If h(n'.s) = 0, solution is found, return node
		 if (manhattan(new_node -> state) == 0){
			 return new_node;
		 } else {
			 //Else, continue recursion
			 r1 = ida(new_node, threshold, newThreshold);
		 }
		 if(r1){
			 if(new_node!=NULL){
			 	free(new_node);
			 }
			 return r1;
		 }
		 //Reset new_node to node
		 memmove(new_node->state, node->state, sizeof(new_node->state));
		 blank_pos = blankpos;
		 new_node->g --;
		 new_node->op = node->op;
	 }
	if(new_node!=NULL){
		free(new_node);
	}
	return( NULL );
}


/* main IDA control loop */
int IDA_control_loop(  ){
	node_t *r = NULL;
	int solution_length;

	int threshold;

	/* initialize statistics */
	generated = 0;
	expanded = 0;

	/* compute initial threshold B */
	initial_node.f = threshold = manhattan( initial_node.state );

	printf( "Initial Estimate = %d\nThreshold = ", threshold);


	/**
	* FILL WITH YOUR CODE
	*
	* Algorithm in Figure 1 of handout
	*/

	int newThreshold = INT_MAX, i, init_blank = blank_pos;

	while(TRUE){
		r = ida(&initial_node, threshold, &newThreshold);
		printf("%d \n", threshold);

		if(r == NULL && threshold < newThreshold){
			//Solution not found, increase threshold
			//printf("threshold = %d, newThreshold = %d\n",threshold, newThreshold);
			threshold = newThreshold;
			newThreshold = INT_MAX;
			//printf("increase threshold to %d", threshold);
		} else if (r) {
			//Solution found, stop looping
			break;
		}
	}

	if(r){
		printf("\n");
		solution_length = r->g;
		free(r);
		return solution_length;
	}else{
		return -1;
	}
}


static inline float compute_current_time()
{
	struct rusage r_usage;

	getrusage( RUSAGE_SELF, &r_usage );
	float diff_time = (float) r_usage.ru_utime.tv_sec;
	diff_time += (float) r_usage.ru_stime.tv_sec;
	diff_time += (float) r_usage.ru_utime.tv_usec / (float)1000000;
	diff_time += (float) r_usage.ru_stime.tv_usec / (float)1000000;
	return diff_time;
}

int main( int argc, char **argv )
{
	int i, solution_length;

	/* check we have a initial state as parameter */
	if( argc != 2 )
	{
		fprintf( stderr, "usage: %s \"<initial-state-file>\"\n", argv[0] );
		return( -1 );
	}


	/* read initial state */
	FILE* initFile = fopen( argv[1], "r" );
	char buffer[256];

	if( fgets(buffer, sizeof(buffer), initFile) != NULL ){
		char* tile = strtok( buffer, " " );
		for( i = 0; tile != NULL; ++i )
			{
				initial_node.state[i] = atoi( tile );
				blank_pos = (initial_node.state[i] == 0 ? i : blank_pos);
				tile = strtok( NULL, " " );
			}
	}
	else{
		fprintf( stderr, "Filename empty\"\n" );
		return( -2 );

	}
	fclose(initFile);

	if( i != 16 )
	{
		fprintf( stderr, "invalid initial state\n" );
		return( -1 );
	}

	/* initialize the initial node */
	initial_node.g=0;
	initial_node.f=0;
	initial_node.op=0;

	print_state( initial_node.state );


	/* solve */
	float t0 = compute_current_time();

	solution_length = IDA_control_loop();

	float tf = compute_current_time();

	/* report results */
	printf( "\nSolution = %d\n", solution_length);
	printf( "Generated = ");
	printf_comma(generated);
	printf("\nExpanded = ");
	printf_comma(expanded);
	printf( "\nTime (seconds) = %.2f\nExpanded/Second = ", tf-t0 );
	printf_comma((unsigned long int) expanded/(tf+0.00000001-t0));
	printf("\n\n");

	/* aggregate all executions in a file named report.dat, for marking purposes */
	FILE* report = fopen( "report.dat", "a" );

	fprintf( report, "%s", argv[1] );
	fprintf( report, "\n\tSoulution = %d, Generated = %lu, Expanded = %lu", solution_length, generated, expanded);
	fprintf( report, ", Time = %f, Expanded/Second = %f\n\n", tf-t0, (float)expanded/(tf-t0));
	fclose(report);

	return( 0 );
}
