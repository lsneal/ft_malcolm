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

bool check_ip_argument(struct malcolm arp, char *ip) {

    int i = 0;
    char *ipt = NULL;

    while (arp.interface[i]) 
    {
        ipt = get_ip_address(arp.interface[i]);
        if (strcmp(ipt, ip) == 0) {
            return (true);
        }
        i++;
    }
    return (false);
}

bool compare_mac(char *mac, char *hexa, int k) {

    //if (hexa[k] == ':')
    //    k++;
    printf("mac = %02x %02x\n", mac[k], (unsigned char)mac[k + 1]);
    if (hexa[0] == mac[k] && mac[k] == mac[k + 1])
        return (true);

    return (false);
}

bool check_mac_address(struct malcolm arp, char *mac) {

    int i = 0, j = 0, hex = 0, k = 0;
    //char hexa[2];
    unsigned char *mact = NULL;
    char    *hexa;

    printf("mac_source = %s\n", mac);
    
    while (arp.interface[i]) {
        j = 0;
        mact = get_mac_address(arp.interface[i]);
        
        while (j < 6) {
            hex = mact[j];
            hexa = ft_itoa(hex);
            int o = 0;
            while (o < 6) {
                printf("%02x:", (unsigned char)hexa[o]);
                o++;
            }
            printf("\n");
            printf("hex = %02x %02x --> ", hexa[0], hexa[1]);
            if (compare_mac(mac, hexa, k) == true)
                k++;
            j++;
        }
        //printf("hex = %02x ", mact[0]);

        free(mact);
        printf("\n");
        i++;
    }
    return (true);
}

void    send_arp() {

    // init struct sockaddr_ll
    struct sockaddr_ll  sockaddr;
    struct ethhdr       *ethhdr;
    struct arpHdr       *arphdr;
    char    buffer[SIZE_ARP];

    memset(buffer, 0x00, SIZE_ARP);
    ethhdr = (struct ethhder *)buffer;

    sockaddr.sll_family = AF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ARP); // 0x0806 for ARP protocol
    sockaddr.sll_infindex = interface --> ifr.ifindex;
    sockaddr.sll_hatype = htons(ARPHRD_ETHER);
    sockaddr.sll_pkttype = PACKET_BROADCAST;
    sockaddr.sll_halen = SIZE_MAC_ADDRESS;
    sockaddr.sll_addr[6] = 0x00;
    sockaddr.sll_addr[7] = 0x00;

    //  set header ethernet header
    //ethhdr->h_dest = ; --> destination eth addr
    //ethhdr->h_source = ; --> source eth addr
    ethhdr->h_proto = htons(ETH_P_ARP); // 0x0806 ARP protocol 

    // set header arp 
    arphdr->ar_hrd = htons(HW_TYPE);
    arphdr->ar_pro = htons(ETH_P_IP); 
    arphdr->ar_hln = SIZE_MAC_ADDRESS;
    arphdr->ar_pln = SIZE_IPV4_ADDRESS;
    arphdr->ar_op = ARPOP_REPLY; // reply arp

    //arphdr->__ar_sha[6] --> sender mac address
    //arphdr->__ar_sip[4] --> sender IP address
    //arphdr->__ar_tha[6] --> target mac address
    //  arphdr->__ar_tip[4] --> target IP address
}

void    receive_arp(struct malcolm arp) {

    (void)arp;
    ssize_t data_size = 0;
    int sockfd;
    unsigned char buffer[SIZE_ARP];
    
    struct ethhdr *ethhdr;
    struct arpHdr *arphdr;

    // use sudo 
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP)); 
    if (sockfd < 0) {
        printf("Error socket\n");
        return ;
    }

    while (1) 
    {

        data_size = recvfrom(sockfd, buffer, SIZE_ARP, 0, NULL, NULL);
        if (data_size < 0) {
            printf("Error receive\n");
            return ;
        }
        // get header eth
        ethhdr = (struct ethhdr *)buffer;

        // check arp packet
        if (ntohs(ethhdr->h_proto) == ETH_P_ARP) {
            // get arp header
            arphdr = (struct arpHdr *)(buffer + sizeof(struct ethhdr));

            // verif si l'ARP est une demande (ARP Request) ou une reponse (ARP Reply)
            if (ntohs(arphdr->ar_op) == ARPOP_REQUEST)
            {
                printf("ARP Request reçue\n");
                print_ip("source", arphdr->__ar_sip);
                print_mac("source", arphdr->__ar_tha);
                print_ip("target", arphdr->__ar_sip);
                print_mac("target", arphdr->__ar_tha);
                // send ARP packet with spoofing if ARPOP_REQUEST
            } 
            else if (ntohs(arphdr->ar_op) == ARPOP_REPLY)
            {
                printf("ARP Reply reçue\n");
                print_ip("source", arphdr->__ar_sip);
                print_mac("source", arphdr->__ar_tha);
                print_ip("target", arphdr->__ar_sip);
                print_mac("target", arphdr->__ar_tha);
            }
        }
    }
}

// check l'ip et l'adresse mac des arg avec la meme interface
int main(int argc, char **argv) {

    //(void)argc;
    //(void)argv; 
    if (argc != 5) {
        printf("Error arg\n");
        return (1);
    }
    struct malcolm arp;

    parse_arg(argv, &arp);
    get_interface_enabled(&arp);

    bool source = check_ip_argument(arp, arp.source.ip);
    bool target = check_ip_argument(arp, arp.target.ip);

    //check_mac_address(arp, arp.source.mac);
    //check_mac_address(arp, arp.target.mac);
    if (source == false || target == false) {
        printf("IP or mac error\n");
        return (1);
    }
    receive_arp(arp);
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

