#include <stdio.h>

int main(int argc, char **argv)
{
	int a = 5;
	for (int i = 0; i < 5; i++) {
		a++;
	}
	return (a * 2) + (-a + 3);
}
