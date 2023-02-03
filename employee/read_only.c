/* This program is very similar to "full_privileges.c"
 * but here we set the permissions of the capability
 * to LOAD, i.e. read-only. So the user is not allowed to
 * change, e.g. the salary.
 */

#include "include/employee.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct employee employee = {1000, "Good", "Employee", 55000.0};
    // set the permissions to read-only
    struct employee *ro_employee = set_read_only(&employee);
    assert((cheri_perms_get(ro_employee) & CHERI_PERM_STORE) == 0);
    print_details(ro_employee);
    double new_salary = 65000.0;
    printf(
        "\nThe struct is read-only so trying to change the salary to %.2lf will make the program "
        "crash...\n",
        new_salary);
    fflush(stdout);
    change_salary(ro_employee, new_salary);
    print_details(ro_employee);
}