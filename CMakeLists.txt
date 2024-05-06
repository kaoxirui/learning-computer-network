cmake_minimum_required(VERSION 3.10)
project(cs_net)

add_executable(TCPserver ./src/TCPserver.cpp)
add_executable(TCPclient ./src/TCPclient.cpp)
add_executable(packet ./src/packet.cpp)
add_executable(web ./src/web.cpp)
#add_executable(scan_port ./src/scan_port.cpp)

target_link_libraries(
    packet -lpcap
)