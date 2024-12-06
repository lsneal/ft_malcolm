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

bool check_ip_and_mac_argument(struct malcolm arp, char *ip, char *mac) {

    int i = 0;
    char *ipt = NULL;
    unsigned char *mact = NULL;
    while (arp.interface[i]) 
    {
        ipt = get_ip_address(arp.interface[i]);
        if (strcmp(ipt, ip) == 0) { // check ip source
            free(ipt);
            mact = get_mac_address(arp.interface[i]);
            if (strcmp(mact, mac) == 0) { // check mac source
                return (true);
            }
        }
        i++;
    }
    return (false);
}



int main(int argc, char **argv) {

    //(void)argc;
    //(void)argv; 
    if (argc != 5) {
        printf("error\n");
        return (1);
    }
    struct malcolm arp;

    parse_arg(argv, &arp);
    get_interface_enabled(&arp);

    bool source = check_ip_and_mac_argument(arp, arp.source.ip, arp.source.mac);
    bool target = check_ip_and_mac_argument(arp, arp.target.ip, arp.target.mac);

    if (source == false || target == false) {
        printf("IP or mac error\n");
        return (1);
    }

    /*for (int i = 0; arp.interface[i]; i++) {
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
    }*/
    free_interface(&arp);
    return (0);
}

