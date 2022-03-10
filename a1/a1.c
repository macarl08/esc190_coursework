#include "a1.h"

#define MAX_PRECISION_DOUBLE 16

/**
	Add your functions to this file.
	Make sure to review a1.h.
	Do NOT include a main() function in this file
	when you submit.
*/

Restaurant* initialize_restaurant(char* name) {

	// Initialize restaurant
	Restaurant *new_restaurant;
	new_restaurant = (Restaurant *) malloc(sizeof(Restaurant));

	// Restaurant name
	new_restaurant->name = (char *) malloc(sizeof(char) * MAX_ITEM_NAME_LENGTH);
	strcpy(new_restaurant->name, name);

	// Restaurant menu
	new_restaurant->menu = load_menu(MENU_FNAME);

	// Restaurant num orders
	new_restaurant->num_completed_orders = 0;
	new_restaurant->num_pending_orders = 0;

	// Restaurant pending_order queue
	Queue *new_queue;
	new_queue = (Queue *) malloc(sizeof(Queue));
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_restaurant->pending_orders = new_queue;

}

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


void print_receipt(Order* order, Menu* menu){
	for (int i = 0; i < order->num_items; i++){
		double item_cost = get_item_cost(order->item_codes[i], menu);
		fprintf(
			stdout, 
			"%d x (%s)\n @$%.2f ea \t %.2f\n", 
			order->item_quantities[i],
			order->item_codes[i], 
			item_cost,
			item_cost * order->item_quantities[i]
		);
	}
	double order_subtotal = get_order_subtotal(order, menu);
	double order_total = get_order_total(order, menu);
	
	fprintf(stdout, "Subtotal: \t %.2f\n", order_subtotal);
	fprintf(stdout, "               -------\n");
	fprintf(stdout, "Tax %d%%: \t$%.2f\n", TAX_RATE, order_total);
	fprintf(stdout, "              ========\n");
}
