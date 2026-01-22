#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "config.h"
#include <zlib.h>
#include <thread>
#include <iostream>
#include <functional>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

struct ZlibDecomp {
	ZlibDecomp(websocket::stream<beast::ssl_stream<tcp::socket>>* ws);

    websocket::stream<beast::ssl_stream<tcp::socket>>* ws;

    z_stream decompressor;
    std::vector<char> decompressed_buffer;

    std::string decompress(const std::vector<uint8_t>& data);

	nlohmann::json read_message();

    void init();
    void close();
};

struct Socket {
    std::atomic<bool> running = true;
	net::io_context io_ctx;
	ssl::context ssl_ctx{ ssl::context::tlsv12_client };
	websocket::stream<beast::ssl_stream<tcp::socket>> ws{ io_ctx, ssl_ctx };

    std::mutex mutex;

    dc::Config config;

    void send_json(const nlohmann::json& json);

    void connect();
    void stop();

    virtual void handle_connection() = 0;

	Socket(dc::Config config);
};

struct MessageSocket : public Socket {
    MessageWebsocketAuth auth;
	ZlibDecomp zlib_decomp;

    std::thread heartbeat_thread;

    void heartbeat(MessageWebsocketAuth* auth);
	std::function<void(const std::string&)> on_message;

    MessageSocket(dc::Config cfg, std::function<void(const std::string&)> callback);

    void handle_connection() override;
};