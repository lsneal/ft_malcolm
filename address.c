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