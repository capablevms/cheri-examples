/* A normal program without any capabilities set.
 * Print the details of a generic employee and
 * and then allows the user to change the salary.
 */

#include "include/employee.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct employee employee = {0, "Good", "Employee", 55000};
    double salary = 0.0;
    print_details(&employee);
    printf("\nInsert the new salary for this employee: \n");
    scanf("%lf", &salary);
    change_salary(&employee, salary);
    print_details(&employee);
}
