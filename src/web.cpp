#include<iostream>
#include<string>
#include<asio.hpp>

using asio::ip::tcp;

std::string response=
R"(<html>
<head><title>Hello World!</title></head>
<body>
<h1>Hello World!</h1>
<p>This is a simple web server implemented in C++ using Asio.</p >
</body>
</html>
)";
//包含了html响应内容

void do_write(tcp::socket& socket){
    asio::error_code ignored_error;
    //忽略写入操作可能产生的错误
    asio::write(socket,asio::buffer(response),ignored_error);
    //将response内容写入到给定的socket中
}

void handle_request(const asio::error_code& error,tcp::socket& socket){
    if(!error){
        //send the response
        do_write(socket);
    }
}

//用asio库实现简单的异步TCP服务器
//使用asio::streambuf来缓冲接受到的数据，并在接收到请求后使用handle_request函数来处理请求
asio::streambuf buffer;
void handle_accept(tcp::acceptor& acceptor,tcp::socket& socket){
    acceptor.async_accept(socket,[&](const asio::error_code& error, tcp::socket& accepted_socket){
        //async_accept用于异步接受连接，并在连接被接受或发生错误时调用lambda函数
    if (!error){
        //将读取的数据存储在buffer对象中，然后调用lambda函数
    asio::async_read_until(accepted_socket,buffer, "\r\n\r\n",[&](const asio::error_code& error, std::size_t bytes_transferred){
        // Handle the request here. For simplicity, we just send the same response for all requests.
        handle_request(error,accepted_socket);
    });
    }
    });
}

int main(int argc,char* argv[]){
    try{
        if(argc!=2){
            std::cerr<<"Usage:"<<argv[0]<<"<post>\n";
            return 1;
        }
        int port=std::atoi(argv[1]);
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context,tcp::endpoint(tcp::v4(),port));

        for (;;){
            tcp::socket socket(io_context);
            handle_accept(acceptor, socket);
            io_context.run();
        }

    }catch(std::exception& e){
        std::cerr<<"Exception:"<<e.what()<<"\n";
    }
    return 0;
}