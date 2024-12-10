#include "ft_malcolm.h"

void    print_ip(char *machine, unsigned char *ip) {

    printf("IP %s: ", machine);
    for (int i = 0; i < 4; i++) {
        if (i == 3)
            printf("%d", ip[i]);
        else
            printf("%d.", ip[i]);
    }
    printf("\n");
}

void    print_mac(char *machine, unsigned char *mac) {

    printf("MAC %s: ", machine);
    for (int i = 0; i < 6; i++) {
        if (i == 5)
            printf("%02x", mac[i]);
        else
            printf("%02x:", mac[i]);
    }
    printf("\n");  
}