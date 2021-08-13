#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct employee
{
	int id;
	char *name;
	char *surname;
	double salary;
};

void change_salary(struct employee *e, double salary);
void print_details(struct employee *e);

struct employee *set_read_only(struct employee *e);