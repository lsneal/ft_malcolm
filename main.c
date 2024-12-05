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
    
    //printf("L'adresse IP de l'interface %s est : %s\n", ifr.ifr_name, inet_ntoa(ifr.ifr_addr));

    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    printf("Mac address de %s: %02x:%02x:%02x:%02x:%02x:%02x\n", ifr.ifr_name, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    close(sock);
}

void    parse_arg(char **argv, struct malcolm *arp) {

    arp->source.ip = strdup(argv[1]);
    arp->source.mac = strdup(argv[2]);
    arp->target.ip = strdup(argv[3]);
    arp->target.mac = strdup(argv[4]);
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
        printf("error\n");
        return ;
    }
    size = sizelist(&ifa) / 2;
    arp->interfaces = malloc(sizeof(char *) * size);
    while(i < size - 1) {
        arp->interfaces[i] = strdup(ifa->ifa_name);
        ifa = ifa->ifa_next;
        i++;    
    }
    arp->interfaces[i] = NULL;
}

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

    for (int i = 0; arp.interfaces[i]; i++) {
        get_mac_address(arp.interfaces[i]);
    }
    return (0);
}