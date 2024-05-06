#include <iostream>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/ip.h>

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
// 转换时间戳
struct tm *ltime;
char timestr[30];
ltime = localtime(&(pkthdr->ts.tv_sec));
strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);
std::cout << "Capture time: " << timestr << "." << pkthdr->ts.tv_usec / 10000 << "s\n";

// 显示数据包长度
std::cout << "Length of the packet: " << pkthdr->len << " bytes\n";

// 分析以太网帧头部
struct ether_header *eh = (struct ether_header *)packet;
std::cout << "Source MAC: " << eh->ether_shost << "\n";
std::cout << "Destination MAC: " << eh->ether_dhost << "\n";
std::cout << "Ethernet type: 0x" << std::hex << ntohs(eh->ether_type) << std::dec << "\n";

// 根据以太网类型进一步分析IP、TCP等协议
if (ntohs(eh->ether_type) == ETHERTYPE_IP) {
struct ip *iph = (struct ip *)(packet + sizeof(struct ether_header));
std::cout << "Version: " << (iph->ip_v >> 4) << "\n";
std::cout << "Header length: " << (iph->ip_v & 0x0F) << " bytes\n";
std::cout << "Total length: " << ntohs(iph->ip_len) << " bytes\n";
std::cout << "Protocol: " << (int)iph->ip_p << "\n";
std::cout << "Source IP: " << inet_ntoa(iph->ip_src) << "\n";
std::cout << "Destination IP: " << inet_ntoa(iph->ip_dst) << "\n";
// 根据IP协议类型进一步分析TCP、UDP等
}
// ... 可以继续添加对其他协议类型的分析 ...

std::cout << std::endl;
}

int main() {
char *dev, errbuf[PCAP_ERRBUF_SIZE];
dev = pcap_lookupdev(errbuf);
if (dev == nullptr) {
std::cerr << "Couldn't find default device: " << errbuf << std::endl;
return(2);
}

std::cout << "Device: " << dev << std::endl;

pcap_t *handle;
char errbuf2[PCAP_ERRBUF_SIZE];
handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf2);
if (handle == nullptr) {
std::cerr << "Couldn't open device for capture: " << errbuf2 << std::endl;
return(2);
}

if (pcap_loop(handle, 0, packet_handler, nullptr) < 0) {
std::cerr << "pcap_loop() failed: " << pcap_geterr(handle) << std::endl;
return(2);
}

pcap_close(handle);
return 0;
}