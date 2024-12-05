#include "ft_malcolm.h"

void    get_mac_address(char *interface) {

    int sock;
    struct ifreq ifr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("socket error\n");
        return ;
    }
    strcpy(ifr.ifr_name, interface);

    if (ioctl(sock, SIOCGIFHWADDR, &ifr)== -1) {
        printf("Error ioctl\n");
        close(sock);
        return ;
    }

    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    printf("Mac address de %s: %02x:%02x:%02x:%02x:%02x:%02x", ifr.ifr_name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    close(sock);
}

int main() {

    get_mac_address("eth0");
    return (0);
}