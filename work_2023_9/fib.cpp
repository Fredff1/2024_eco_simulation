#include <stdio.h>

void hanoi(int n, char from, char aux, char to) {
    if (n == 1) {
        printf("Move disk from %c to %c\n", from, to);
        return;
    }
    hanoi(n - 1, from, to, aux);
    printf("Move disk from %c to %c\n", from, to);
    hanoi(n - 1, aux, from, to);
}

int main() {
    int n;
    printf("Enter the number of disks: ");
    scanf("%d", &n);
    hanoi(n, 'A', 'B', 'C');
    return 0;
}
