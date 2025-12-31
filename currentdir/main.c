#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *f = fopen("name.txt", "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    char buf[7] = {0};   // 6 chars + null terminator
    fread(buf, 1, 6, f);
    fclose(f);

    printf("your name is ");
    if (strncmp(buf, "justin", 6) == 0) {
        printf("justin\n");
    }
    else if (strncmp(buf, "adrien", 6) == 0) {
        printf("adrien\n");
    }
    else {
        printf("unknown\n");
    }

    return 0;
}
