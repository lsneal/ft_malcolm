#include "ft_malcolm.h"

void    parse_arg(char **argv, struct malcolm *arp) {

    arp->source.ip = strdup(argv[1]);
    arp->source.mac = strdup(argv[2]);
    arp->target.ip = strdup(argv[3]);
    arp->target.mac = strdup(argv[4]);
}

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

char    *get_ip_address(char *interface) {

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

    
    //char *ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    printf("%s\n", address);
    //printf("mac: %s\n", mac);
    close(sock);

    return (address);
}

int			sizelist(struct ifaddrs **head)
{
	struct ifaddrs		*cursor;
	int			count;

	count = 0;
	cursor = *head;
	while (cursor)
	{
		cursor = cursor->ifa_next;
		count++;
	}
	return (count);
}

void    get_interface_enabled(struct malcolm *arp) {

    struct ifaddrs *ifa;
    int i = 0, size = 0;
    if (getifaddrs(&ifa) == -1) {
        printf("Error get interface\n");
        return ;
    }
    size = sizelist(&ifa) / 2;
    arp->interface = malloc(sizeof(char *) * size);
    while(i < size - 1) {
        arp->interface[i] = strdup(ifa->ifa_name);
        ifa = ifa->ifa_next;
        i++;    
    }
    arp->interface[i] = NULL;
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

    // check les ips et interface;
    for (int i = 0; arp.interface[i]; i++) {
        printf("interface = %s\n", arp.interface[i]);
        unsigned char *str = get_mac_address(arp.interface[i]);
        int j = 0;
        while (str[j]) {
            printf("%02x:", str[j]);
            j++;
        }
        printf("\n");
        free(str);
        get_ip_address(arp.interface[i]);
    }
    return (0);
}