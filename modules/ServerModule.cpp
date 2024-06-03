#include "ServerModule.h"

//------------------------- Server Methods ---------------------------------------------------------------------------//

tcp_server::tcp_server(boost::asio::io_context &io_context)
    : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 12000))
{
    start_accept();
}


void tcp_server::start_accept() {
    tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept,
                                            this, new_connection,
                                            boost::asio::placeholders::error));

}

void tcp_server::handle_accept(const tcp_connection::pointer &new_connection, const boost::system::error_code &error) {
    if (!error)
    {
        std::cout << "New connection from: " << new_connection->get_ip() << std::endl;
        new_connection->start();
    }
    start_accept();
}
//--------------------------------------------------------------------------------------------------------------------//




//------------------------- Each Connection Methods ------------------------------------------------------------------//
tcp_connection::pointer tcp_connection::create(boost::asio::io_context &io_context) {
    return pointer(new tcp_connection(io_context));
}

tcp::socket &tcp_connection::socket() {
    return socket_;
}

std::string tcp_connection::make_daytime_string() {
        using namespace std; // For time_t, time and ctime;
        time_t now = time(0);
        return ctime(&now);
}

void tcp_connection::start() {

    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
                             boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));

    start_read();
}

void tcp_connection::handle_write(const boost::system::error_code &, size_t) {

}

void tcp_connection::start_read() {
    // Asynchronously read data from the socket
    socket_.async_read_some(boost::asio::buffer(data_),
                            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
    if (!error) {
        // Handle the data...
        data_[bytes_transferred] = '\0';

        std::string received_data(data_);
        std::cout << "Received data: " << received_data << "From IP: "<< get_ip() <<":"<< get_port() <<"  "<< std::endl;

        // Continue reading from the socket
        start_read();
    }
}

std::string tcp_connection::get_ip() {
    return socket_.remote_endpoint().address().to_string();
}

int tcp_connection::get_port() {
    return socket_.remote_endpoint().port();
}
//--------------------------------------------------------------------------------------------------------------------//

