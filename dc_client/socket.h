#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "auth.h"
#include <zlib.h>
#include <thread>
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

struct MessageSocket {
    MessageWebsocketAuth auth;

    net::io_context io_ctx;
    ssl::context ssl_ctx{ssl::context::tlsv12_client};
    websocket::stream<beast::ssl_stream<tcp::socket>> ws{io_ctx, ssl_ctx};

    z_stream decompressor;
    std::vector<char> decompressed_buffer;

    std::thread heartbeat_thread;

    std::mutex mutex;

    MessageSocket(MessageWebsocketAuth auth);

    std::string decompress(const std::vector<uint8_t>& data);

    void send_json(const nlohmann::json& json);

    nlohmann::json read_message();

    void heartbeat(MessageWebsocketAuth* auth);

    void handle_connection();

    void connect();
};