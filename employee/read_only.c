/* This program is very similar to "full_privileges.c"
 * but here we set the permissions of the capability
 * to LOAD, i.e. read-only. So the user is not allowed to
 * change the salary.
 */

#include "../include/common.h"
#include "include/employee.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct employee employee = {0, "Good", "Employee", 55000};
	double salary = 0.0;
	// set the permissions to read-only
	struct employee *ro_employee = set_read_only(&employee);
	inspect_pointer(ro_employee);
	print_details(ro_employee);
	printf("The struct is read-only so trying to change the salary will make the program crash...");
	printf("\nInsert the new salary for this employee: \n");
	fflush(stdout);
	scanf("%lf", &salary);

	change_salary(ro_employee, salary);
	print_details(ro_employee);
}

/*
 * Traverse the struct to set the permissions.
 */
struct employee *set_read_only(struct employee *e)
{
	char *ro_name = (char *)cheri_perms_and(e->name, CHERI_PERM_LOAD);
	char *ro_surname = (char *)cheri_perms_and(e->surname, CHERI_PERM_LOAD);
	struct employee *ro_employee =
		(struct employee *)cheri_perms_and(e, CHERI_PERM_LOAD || CHERI_PERM_LOAD_CAP);
	ro_employee->name = ro_name;
	ro_employee->surname = ro_surname;
	return ro_employee;
}
