#include "ServerModule.h"
#include "include/ImGuiNotify.hpp"

//------------------------- Server Methods ---------------------------------------------------------------------------//

tcp_server::tcp_server(boost::asio::io_context &io_context)
    : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 12000))
{
    start_accept();
}


void tcp_server::start_accept() {
    const tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept,
                                            this, new_connection,
                                            boost::asio::placeholders::error));

}

void tcp_server::handle_accept(const tcp_connection::pointer &new_connection, const boost::system::error_code &error) {
    if (!error)
    {
        connection_count_++;
        connections_.push_back(new_connection);
        std::cout << "New connection from: " << new_connection->get_ip() <<":"<<new_connection->get_port()<< std::endl;
        ImGui::InsertNotification({ImGuiToastType::Info, 10000, "New connection from: %s:%d", new_connection->get_ip().c_str(), new_connection->get_port()});
        std::cout << "Total connections count: " << connection_count_ << std::endl;
        new_connection->start();

        new_connection->set_close_callback([this, new_connection] {
            connection_count_--;
            ImGui::InsertNotification({ImGuiToastType::Warning, 10000, "Connection closed %s:%d", new_connection->get_ip().c_str(), new_connection->get_port()});
            connections_.erase(std::remove(connections_.begin(), connections_.end(), new_connection), connections_.end());
            std::cout << "Connection closed. Total active connections: " << connection_count_ << std::endl;
        });
    }
    start_accept();
}

int tcp_server::get_connections_count() const {
    return connection_count_;
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

void tcp_connection::handle_write(const boost::system::error_code &error, size_t) const {
    if (!error){
       //start_read();  // wait for new data from the client
    }
    else{
        // Handle the error
        std::cerr << "Error during write: " << error.message() << std::endl;
        if (close_callback_)
        {
            close_callback_();
        }
    }
}

void tcp_connection::start_read() {
    // Asynchronously read data from the socket
    socket_.async_read_some(boost::asio::buffer(data_),
                            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_read(const boost::system::error_code &error, const size_t bytes_transferred) {
    if (!error) {
        // Handle the data...
        data_[bytes_transferred] = '\0';
        // Send the latest data after receiving data from the client
        send_data(latest_data_to_send_);
        // Continue reading from the client
        start_read();
    }
    else {
        // If there's an error, it could mean the connection was closed
        std::cout << "Connection closed or error in reading: " << error.message() << std::endl;
        if (close_callback_) {
            close_callback_();
        }
    }
}

std::string tcp_connection::get_ip() const {
    return socket_.remote_endpoint().address().to_string();
}

int tcp_connection::get_port() const {
    return socket_.remote_endpoint().port();
}

void tcp_connection::set_close_callback(const tcp_connection::CloseCallback &callback) {
    close_callback_ = callback;
}

std::string tcp_connection::get_latest_data() {
    std::string actual_data(data_);
    return actual_data;
}

void tcp_connection::send_data(const std::string& data) {
    boost::asio::async_write(socket_, boost::asio::buffer(data),
                                         boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
}


void tcp_connection::update_sending_data(const std::string &latest_data) {
    latest_data_to_send_ = latest_data;
}

//--------------------------------------------------------------------------------------------------------------------//

