#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MENU_DELIM ","
#define ITEM_CODE_LENGTH 3
#define MAX_ITEM_NAME_LENGTH 100
#define MAX_ITEM_QUANTITY_DIGITS 20
#define MAX_PRECISION_DOUBLE 16

#define MENU_DELIM ","
#define MENU_FNAME "menu.txt"

#define TAX_RATE 13
typedef struct Menu {
	int num_items;
	char** item_codes; 
	char** item_names; 
	double* item_cost_per_unit;
} Menu;

typedef struct Order {
	int num_items;
	char** item_codes;
	int* item_quantities;
} Order;


Menu* load_menu(char* fname) {

	// Initialize menu
	Menu *new_menu;
	new_menu = (Menu *) malloc(sizeof(Menu));

	// Initialize menu attributes
	int count_items = 1;
	
	// Get number of items
	FILE *input_file;
	input_file = fopen(fname, "r");
	char c;
	for (c=getc(input_file); c!=EOF; c=getc(input_file)){
		if (c=='\n') count_items++;
	}
	fclose(input_file);

	// Continue to initialize menu attributes
	char **item_codes_array;
	item_codes_array = (char **) malloc(sizeof(char*) * count_items);
	char **item_names_array;
	item_names_array = (char **) malloc(sizeof(char*) * count_items);
	double *item_cost_array = (double *) malloc(sizeof(double) * count_items);

	// Read file line by line
	char *line;
	line = (char *) malloc(ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE);

	int idx = 0;
	input_file = fopen(fname, "r");
	while (fgets(line, ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE, input_file) != NULL) {
		

		char *item_code = strtok(line, MENU_DELIM);
		char *item_name = strtok(NULL, MENU_DELIM);
		char *item_cost = strtok(NULL, MENU_DELIM);

		item_codes_array[idx] = (char *) malloc(sizeof(char) * ITEM_CODE_LENGTH);
		strcpy(item_codes_array[idx], item_code);

		item_names_array[idx] = (char *) malloc(sizeof(char) * MAX_ITEM_NAME_LENGTH);
		strcpy(item_names_array[idx], item_name);

		//char *item_cost_clean = item_cost+1; // removes first character
  		//item_cost_clean[strlen(item_cost)-1] = '\0'; // removes last character
		item_cost_array[idx] = strtod(item_cost+1, NULL);
		idx++;
	}

	fclose(input_file);
	new_menu->num_items = count_items;
	new_menu->item_codes = item_codes_array;
	new_menu->item_names = item_names_array;
	new_menu->item_cost_per_unit = item_cost_array;

	return new_menu;

}


Order* build_order(char* items, char* quantities) {
	Order *new_order;
	new_order = (Order *) malloc(sizeof(Order));

	int num_unique_item = strlen(items) / (ITEM_CODE_LENGTH - 1);
   

	char **item_codes_array_for_order;
	item_codes_array_for_order = (char **) malloc(sizeof(char*) * num_unique_item);
	int *item_q_array = (int *) malloc(sizeof(int) * num_unique_item);

	int code_idx = 0;

    char *cpy_quantities;
    cpy_quantities = (char *) calloc(sizeof(char), strlen(quantities)+1);
    strcpy(cpy_quantities,quantities);


	for (int loop=0; loop<num_unique_item; loop++) {

		// Assign item code
		item_codes_array_for_order[loop] = (char *) calloc(sizeof(char) , (ITEM_CODE_LENGTH));
		strncpy(item_codes_array_for_order[loop], items, ITEM_CODE_LENGTH-1);
		items = items + ITEM_CODE_LENGTH-1;
        //printf("%s\n",item_codes_array_for_order[loop]);

        // Assign item quantities
		char *item_quantity;
		if (loop==0) {
			item_quantity = strtok(cpy_quantities, MENU_DELIM);
		}
		else {
			item_quantity = strtok(NULL, MENU_DELIM);
		}
        
		item_q_array[loop] = atoi(item_quantity);
        //printf("%d\n",item_q_array[loop]);


	}
    free(cpy_quantities);
	new_order->num_items = num_unique_item;
	new_order->item_codes = item_codes_array_for_order;
	new_order->item_quantities = item_q_array;
	return new_order;
	
}



void print_menu(Menu* menu){
	fprintf(stdout, "--- Menu ---\n");
	for (int i = 0; i < menu->num_items; i++){
		fprintf(stdout, "(%s) %s: %.2f\n", 
			menu->item_codes[i], 
			menu->item_names[i], 
			menu->item_cost_per_unit[i]	
		);
	}
}

void print_order(Order* order){
	for (int i = 0; i < order->num_items; i++){
		fprintf(
			stdout, 
			"%d x (%s)\n", 
			order->item_quantities[i], 
			order->item_codes[i]
		);
	}
}


int main() {
	//Menu *new;
	//new = load_menu("menu.txt"); 
	//print_menu(new);
    Order * order = build_order ("A1B1C1D4", "20,11,17,1");
    print_order(order);
}