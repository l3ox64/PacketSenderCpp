#include <iostream>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int main() {
    //general
    string protocol;

    // source
    string source_ip;
    string source_mac;
    int source_port;

    // destination
    string dest_ip;
    string dest_mac;
    int dest_port;

    cout << "Protocol (TCP/UDP): ";
    cin >> protocol;

    cout << "source IP (only IPV4): ";
    cin >> source_ip;

    cout << "source MAC: ";
    cin >> source_mac;

    cout << "source PORT: ";
    cin >> source_port;

    cout << "dest IP (only IPV4): ";
    cin >> dest_ip;

    cout << "dest MAC: (if left blank will do ARP): ";
    cin >> dest_mac;

    cout << "dest PORT: ";
    cin >> dest_port;

    const char* payload = "sudoers test";
    /*cout << "Enter payload data: ";
    cin.ignore();
    string payload;
    getline(cin, payload);
    */
   
    // build socket
    int sock;
    if (protocol == "UDP") {
        sock = socket(AF_INET, SOCK_DGRAM, 0); // UDP
    } else {
        sock = socket(AF_INET, SOCK_STREAM, 0); // TCP
        struct sockaddr_in temp_addr;
        memset(&temp_addr, 0, sizeof(temp_addr));
        temp_addr.sin_family = AF_INET;
        temp_addr.sin_port = htons(dest_port);
        inet_pton(AF_INET, dest_ip.c_str(), &temp_addr.sin_addr);
        connect(sock, (struct sockaddr*)&temp_addr, sizeof(temp_addr));
    }

    // packet ips
    struct sockaddr_in source_addr, dest_addr;
    memset(&source_addr, 0, sizeof(source_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));

    // source
    source_addr.sin_family = AF_INET;
    source_addr.sin_port = htons(source_port); 
    inet_pton(AF_INET, source_ip.c_str(), &source_addr.sin_addr);

    // destination
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(dest_port);
    inet_pton(AF_INET, dest_ip.c_str(), &dest_addr.sin_addr);

    // send packet
    if (protocol == "UDP") {
        sendto(sock, payload, strlen(payload), 0,
               (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    } else {
        send(sock, payload, strlen(payload), 0);
    }

    cout << "packet sent, check with wireshark. " << endl;



    close(sock);

    return 0;
}