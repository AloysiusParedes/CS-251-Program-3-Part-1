#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"

typedef struct node {
    ElemType val;
    struct node *next;
} NODE;

struct list_struct {
    NODE *front;
    NODE *back;
	int count;
};

LIST *rsort(LIST *list, int radix, int largest){
	LIST *fromArr[radix]; //from bucket
	LIST *toArr[radix]; //to bucket
	int i, j, lsd, val; //lsd = least significant digit
	
	//list is empty
	if(lst_is_empty(list))
		return list;
	
	/* create lists in each array */
	for(i = 0; i < radix; i++){
		fromArr[i] = lst_create();
		toArr[i] = lst_create();
	}
	
	/* first pass values stored into buckets according to least significant digit*/
	i = 0;
	while(!lst_is_empty(list)){
		//find least significant digit
		lsd = (list->front->val) % radix;
		if(lsd == i){		
			//remove from front
			val = lst_pop_front(list);
			//push to the back of the corresponding index in array
			lst_push_back(fromArr[i], val);
			i = 0;
			continue;
		}
		i++;
	}
	
	/* next passes */	
	int divisor = radix, index, counter = 0, k = 2;
	
	while(1){
		//do fromArr to toArr
		if(counter % 2 == 0){
			j = 0;
			while(j != radix){
				if(!lst_is_empty(fromArr[j])){
					val = lst_pop_front(fromArr[j]);
					index = (val / divisor) % radix;
					lst_push_back(toArr[index], val);
					continue;
				}
				j++;
			}
		}
		//do toArr to fromArr
		else{
			j = 0;
			while(j != radix){
				if(!lst_is_empty(toArr[j])){
					val = lst_pop_front(toArr[j]);
					index = (val / divisor) % radix;
					lst_push_back(fromArr[index], val);
					continue;
				}
				j++;
			}
		}
		
		//update divisor
		divisor = divisor * radix;
		//update the number of passes
		k++;
		//update counter
		counter++;
		
		/* finished after the 10^k digit has been processed where 10^k is larger than the largest value being sorted */ 
		if(divisor > largest)
			if(lst_length(fromArr[0]) == radix || lst_length(toArr[0]) == radix)
				break;
	}
	
	printf("\nNumber of Passes: %d", --k);
	//see if it's in the from bucket
	if(counter % 2 == 0){
		/* free allocated lists */
		for(i = 0; i < radix; i++)
			lst_free(toArr[i]);
		for(i = 1; i < radix; i++)
			lst_free(fromArr[i]);
		
		//return the sorted 'list'
		return fromArr[0];
	}
	//see if it's in the to bucket
	else{
		/* free allocated lists */
		for(i = 0; i < radix; i++)
			lst_free(fromArr[i]);
		for(i = 1; i < radix; i++)
			lst_free(toArr[i]);
		
		//return the sorted 'list' 
		return toArr[0];
	}
}

int main(){
	int userInput, i = 0, radix = 0, max = 0;
	
	//userInput will hold the values of user input
	LIST *list = lst_create();
	LIST *newlist;
	
	printf("\nInput numbers for linked list: ");
	
	//loop to accept user input
	while(scanf("%d", &userInput) == 1){
		//if user input is not positive
		if(userInput < 0){
			printf("Value can't be negative\n");
			printf("Re-input: ");
			continue;
		}
		lst_push_back(list, userInput);
		if(userInput > max)
			max = userInput;
	}

	/* print the list */
	printf("\nLinked List:\n   ");
	lst_print(list);

	//set radix to the length of the list
	radix = lst_length(list);
	
	//radix sort the linked list
	newlist = rsort(list, radix, max);
	
	//print the sorted list
	printf("\nList after Radix-Sort:\n   ");
	lst_print(newlist);
	printf("\n");
		
	/*free lists made */
	if(radix != 0)
		lst_free(newlist);
	lst_free(list);
	
	return 0;
}