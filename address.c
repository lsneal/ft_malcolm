#include "ft_malcolm.h"

unsigned char    *get_mac_address(char *interface) {

    int sock;
    struct ifreq ifr;
    struct sockaddr_in *addr;
    char *address;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("socket error\n");
        return (NULL);
    }
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, interface);

    // SIOCGIFADDR --> for ip 
    // SIOCGIFHWADDR --> for mac
    if (ioctl(sock, SIOCGIFHWADDR, &ifr)== -1) {
        printf("Error ioctl\n");
        close(sock);
        return (NULL);
    }
	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    address = inet_ntoa(addr->sin_addr);
    (void)address;

    unsigned char *mac = malloc(7);
    memcpy(mac, (unsigned char *)ifr.ifr_hwaddr.sa_data, 6);
    close (sock);
    return (mac);
}

char    *get_ip_address(char *interface, int *index) {

    int sock;
    struct ifreq ifr;
    struct sockaddr_in *addr;
    char *address;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("socket error\n");
        return (NULL);
    }
    //printf("interface = %s\n", interface);
    strcpy(ifr.ifr_name, interface);

    // SIOCGIFADDR --> for ip 
    // SIOCGIFHWADDR --> for mac
    if (ioctl(sock, SIOCGIFADDR, &ifr)== -1) {
        printf("Error ioctl\n");
        close(sock);
        return (NULL);
    }

	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    address = inet_ntoa(addr->sin_addr);

    *index = ifr.ifr_ifindex;
    //char *ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    
    //printf("%s\n", address);
    
    close(sock);

    return (address);
}

void    macto_int(struct malcolm *arp) {

    char tab[SIZE_MAC_ADDRESS] = {0};

    for (int i = 0, j = 0, t = 0; i < (int)strlen((char *)arp->source.mac); i++) {
        if (arp->source.mac[i] && arp->source.mac[i] != ':') {
            j = 0;
            while (j < SIZE_MAC_ADDRESS && arp->source.mac[i] && arp->source.mac[i] != ':') {
                tab[j] = arp->source.mac[i];
                i++;
                j++;
            }
            if (t < SIZE_MAC_ADDRESS) {
                arp->source.int_mac[t] = hex2int(tab);
                t++;
            }
        }
    }
    memset(tab, 0x00, SIZE_MAC_ADDRESS);
    for (int i = 0, j = 0, t = 0; i < (int)strlen((char *)arp->target.mac); i++) {
        if (arp->target.mac[i] && arp->target.mac[i] != ':') {
            j = 0;
            while (j < SIZE_MAC_ADDRESS && arp->target.mac[i] && arp->target.mac[i] != ':') {
                tab[j] = arp->target.mac[i];
                i++;
                j++;
            }
            if (t < SIZE_MAC_ADDRESS) {
                arp->target.int_mac[t] = hex2int(tab);
                t++;
            }
        }
    }
}

void    ipto_int(struct malcolm *arp) {

    char tab[SIZE_IPV4_ADDRESS] = {0};

    for (int i = 0, j = 0, t = 0; i < (int)strlen(arp->source.ip); i++) {
        if (arp->source.ip[i] && arp->source.ip[i] != '.') {
            j = 0;
            while (j < SIZE_IPV4_ADDRESS && arp->source.ip[i] && arp->source.ip[i] != '.') {
                tab[j] = arp->source.ip[i];
                i++;
                j++;
            }
            if (t < SIZE_IPV4_ADDRESS) {
                arp->source.int_ip[t] = atoi((const char *)tab);
                t++;
            }
        }
    }
    memset(tab, 0x00, SIZE_IPV4_ADDRESS);
    for (int i = 0, j = 0, t = 0; i < (int)strlen(arp->target.ip); i++) {
        if (arp->target.ip[i] && arp->target.ip[i] != '.') {
            j = 0;
            while (j < SIZE_IPV4_ADDRESS && arp->target.ip[i] && arp->target.ip[i] != '.') {
                tab[j] = arp->target.ip[i];
                i++;
                j++;
            }
            if (t < SIZE_IPV4_ADDRESS) {
                arp->target.int_ip[t] = atoi((const char *)tab);
                t++;
            }
        }
    }
}
