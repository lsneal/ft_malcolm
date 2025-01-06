#include "ft_malcolm.h"

unsigned char    *get_mac_address(char *interface) {

    int sock;
    struct ifreq ifr;
    struct sockaddr_in *addr;
    char *address;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket");
        return (NULL);
    }
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, interface);

    // SIOCGIFADDR --> for ip 
    // SIOCGIFHWADDR --> for mac
    if (ioctl(sock, SIOCGIFHWADDR, &ifr)== -1) {
        perror("Ioctl");
        goto close;
    }
	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    address = inet_ntoa(addr->sin_addr);
    (void)address;

    unsigned char *mac = malloc(7);
    memcpy(mac, (unsigned char *)ifr.ifr_hwaddr.sa_data, 6);

    close:
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
        perror("Socket:");
        return (NULL);
    }
    //printf("interface = %s\n", interface);
    ifr.ifr_addr.sa_family = AF_INET;
    printf("interface = %s\n", interface);
    strcpy(ifr.ifr_name, interface);

    // SIOCGIFADDR --> for ip 
    // SIOCGIFHWADDR --> for mac
    if (ioctl(sock, SIOCGIFADDR, &ifr)== -1) {
        //perror("Iooctl:");
        goto close;
    }

	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
    address = inet_ntoa(addr->sin_addr);

    printf("index = %d\n", ifr.ifr_ifindex);
    printf("address %s\n", address);
    *index = ifr.ifr_ifindex + 1 ;
    //char *ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    
    //printf("%s\n", address);
    
    close:
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

    //char tab[SIZE_IPV4_ADDRESS] = {0};

    char *ip_copy = strdup(arp->target.ip);
    char *token;
    int i = 0;

    token = strtok(ip_copy, ".");
    while (token != NULL && i < 4) {
        arp->target.int_ip[i] = atoi(token);
        token = strtok(NULL, ".");
        i++;
    }
    free(ip_copy);
    ip_copy = NULL;
    
    ip_copy = strdup(arp->source.ip);
    i = 0;

    token = strtok(ip_copy, ".");
    while (token != NULL && i < 4) {
        arp->source.int_ip[i] = atoi(token);
        token = strtok(NULL, ".");
        i++;
    }
}

bool check_mac_address(struct malcolm *arp, unsigned char *mac) {
    (void)mac;
    if (strlen((char *)arp->source.mac) != 17 || strlen((char *)arp->target.mac) != 17)
        return (false);
    return (true);
}