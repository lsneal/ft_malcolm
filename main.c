#include "ft_malcolm.h"


void    get_interface_enabled(struct malcolm *arp) {

    struct ifaddrs *ifa, *ifatemp;
    int i = 0, size = 0;
    if (getifaddrs(&ifa) == -1) {
        printf("Error get interface\n");
        return ;
    }
    ifatemp = ifa;
    size = sizelist(&ifa) / 2;
    arp->interface = malloc(sizeof(char *) * size);
    while(i < size - 1) {
        arp->interface[i] = strdup(ifa->ifa_name);
        ifa = ifa->ifa_next;
        i++;    
    }
    ifa = ifatemp;
    arp->interface[i] = NULL;
    freeifaddrs(ifa);
}

/*void check_ip_and_mac(struct malcolm arp) {

    for (int i = 0; arp.interface[i]; i++) {

    }
}*/



int main(int argc, char **argv) {

    (void)argc;
    (void)argv; 
    /*if (argc != 5) {
        printf("error\n");
        return (1);
    }*/
    struct malcolm arp;

    //parse_arg(argv, &arp);

    //arp.source.ip = strdup(argv[1]);
    get_interface_enabled(&arp);

    for (int i = 0; arp.interface[i]; i++) {
        printf("interface = %s\n", arp.interface[i]);
        unsigned char *str = get_mac_address(arp.interface[i]);
        int j = 0;
        while (j < 6) {
            printf("%02x:", str[j]);
            j++;
        }
        printf("\n");
        free(str);
        get_ip_address(arp.interface[i]);
    }
    free_interface(&arp);
    return (0);
}
    
