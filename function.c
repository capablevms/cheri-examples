#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

int gcd(int a, int b) 
{ 
    if (a == 0) 
       return b; 
    if (b == 0) 
       return a; 
    if (a == b) 
        return a; 
    if (a > b) 
        return gcd(a-b, b); 
    return gcd(a, b-a); 
} 


int main() {

	int a, b = 0;
	printf("Enter first number:");
	scanf("%d", &a);
	printf("Enter second number:");
	scanf("%d", &b);
	
	int c_gcd = gcd(a, b);
	printf("The gcd of these numbers is: %d\n", c_gcd);
	return 0;

}
