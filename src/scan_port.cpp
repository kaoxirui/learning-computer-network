#include<iostream>
#include<vector>
#include<thread>
#include<boost/asio.hpp>

using boost::asio::ip::tcp;

void scan_port(tcp::resolver::query query,tcp::socket& socket,std::mutex& mtx
                ,std::vector<int>&open_ports){
    try{
        boost::asio::execution_context& execution_context = socket.get_executor().context();
        tcp::resolver resolver(execution_context);
        tcp::resolver::iterator endpoint_iterator=resolver.resolve(query);
        boost::asio::connect(socket,endpoint_iterator);
        std::lock_guard<std::mutex>lock(mtx);
        open_ports.push_back(query.service_name().data()[0]-'0');
    }catch(const boost::system::system_error& e){
        //post is closed;
    }
}

void scan_ip(const std::string& ip,int start_port,int end_port){
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    std::mutex mtx;
    std::vector<int>open_ports;
    std::vector<std::thread>threads;

    for(int port=start_port;port<=end_port;port++){
        std::string port_str=std::to_string(port);
        tcp::resolver::query query(ip,port_str);
        threads.emplace_back([&](){
            scan_port(query,socket,mtx,open_ports);
        });
    }
    for(auto&t:threads){
        t.join();
    }

    for(int port:open_ports){
        std::cout<<"Port "<<port<<"is open"<<std::endl;
    }
}

int main() {
std::string ip;
int start_port, end_port;

std::cout << "Enter IP address: ";
std::cin >> ip;
std::cout << "Enter start port: ";
std::cin >> start_port;
std::cout << "Enter end port: ";
std::cin >> end_port;

scan_ip(ip, start_port, end_port);

return 0;
}