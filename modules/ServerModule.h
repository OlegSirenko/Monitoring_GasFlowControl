#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


using boost::asio::ip::tcp;


//---------------------- Class that represent each connection ---------------------------//
class tcp_connection
        : public std::enable_shared_from_this<tcp_connection>
{
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context);
    tcp::socket& socket();
    static std::string make_daytime_string();
    void start();
    std::string get_ip();
    int get_port();


private:
    explicit tcp_connection(boost::asio::io_context& io_context) : socket_(io_context){}
    void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);
    void start_read();
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

// Private Vars
private:
    tcp::socket socket_;
    std::string message_;
    char data_[128]{};


};





//-------------------------- Server Class --------------------------//
class tcp_server
{
public:
    explicit tcp_server(boost::asio::io_context& io_context);

private:
    void start_accept();
    void handle_accept(const tcp_connection::pointer& new_connection,
                       const boost::system::error_code& error);
    // Private Vars
private:
    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};




