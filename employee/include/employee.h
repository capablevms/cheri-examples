/*
 * Employee struct and auxiliary functions to display employee's details,
 * change the salary, and operate on the struct's permissions.
 */

#include "../../include/common.h"
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

void print_details(struct employee *e)
{
	printf("--- EMPLOYEE ---\n");
	printf("Employee Id: %d\n", e->id);
	printf("Employee Name: %s \n", e->name);
	printf("Employee Surname: %s\n", e->surname);
	printf("Employee Salary: %.2lf\n", e->salary);
	fflush(stdout);
}

void change_salary(struct employee *e, double salary)
{
	e->salary = salary;
}

/*
 * Traverse the struct to set the permissions.
 */
struct employee *set_read_only(struct employee *e)
{
	e->name = (char *)cheri_perms_and(e->name, CHERI_PERM_LOAD);
	e->surname = (char *)cheri_perms_and(e->surname, CHERI_PERM_LOAD);
	return (struct employee *)cheri_perms_and(e, CHERI_PERM_LOAD | CHERI_PERM_LOAD_CAP);
}