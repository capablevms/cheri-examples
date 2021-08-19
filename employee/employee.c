/*
 * Auxiliary functions to display employee's details and
 * change the salary.
 */

#include "include/employee.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void change_salary(struct employee *e, double salary)
{
	e->salary = salary;
}

void print_details(struct employee *e)
{
	printf("Employee Id: %d\n", e->id);
	printf("Employee Name: %s \n", e->name);
	printf("Employee Surname: %s\n", e->surname);
	printf("Employee Salary: %.2lf\n", e->salary);
	fflush(stdout);
}