#include "socket.h"
#include <windows.h>
#include <wincrypt.h>
#include <openssl/err.h>
#include <mutex>

MessageSocket::MessageSocket(MessageWebsocketAuth auth) : auth(auth), ssl_ctx(ssl::context::tlsv12_client) {
    decompressor = {};
    inflateInit2(&decompressor, 15);
    decompressed_buffer.resize(65536);

    ssl_ctx.set_default_verify_paths();
    ssl_ctx.set_verify_mode(ssl::verify_peer);
}

std::string MessageSocket::decompress(const std::vector<uint8_t>& data) {
    std::string result;

    decompressor.next_in = const_cast<Bytef*>(data.data());
    decompressor.avail_in = static_cast<uInt>(data.size());

    do {
        decompressor.next_out = reinterpret_cast<Bytef*>(decompressed_buffer.data());
        decompressor.avail_out = static_cast<uInt>(decompressed_buffer.size());

        int ret = inflate(&decompressor, Z_SYNC_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            throw std::runtime_error("Zlib error: " + std::string(zError(ret)));
        }

        size_t have = decompressed_buffer.size() - decompressor.avail_out;
        result.append(decompressed_buffer.data(), have);
    } while (decompressor.avail_out == 0);

    return result;
}

nlohmann::json MessageSocket::read_message() {
    beast::flat_buffer buffer;
    ws.read(buffer);

    std::string raw = beast::buffers_to_string(buffer.data());
    std::vector<uint8_t> bytes(raw.begin(), raw.end());
    std::string decompressed_data = decompress(bytes);
    return nlohmann::json::parse(decompressed_data);
}

void MessageSocket::send_json(const nlohmann::json& json) {
    std::lock_guard<std::mutex> lock(mutex);
    std::string msg = json.dump();
    ws.write(net::buffer(msg));
}

void MessageSocket::heartbeat(MessageWebsocketAuth* auth) {
    while (true) {
        auto hbeat = auth->get_heartbeat();
        send_json(hbeat);
        std::this_thread::sleep_for(std::chrono::milliseconds(auth->heartbeat_interval));
    }
}

void MessageSocket::handle_connection() {
    send_json(auth.verify);
    nlohmann::json json = read_message();
    auth.heartbeat_interval = json["d"]["heartbeat_interval"];
    
    heartbeat_thread = std::thread([this]() {
        heartbeat(&this->auth);
    });

    send_json(auth.session);
    send_json(auth.subscribe);
    while (true) {
        nlohmann::json msg = read_message();

        if (msg.contains("d") && msg["d"].is_object() && msg["d"].contains("seq")) {
            auth.sequence = msg["d"]["seq"];
        }

        std::string event_type = "";
        if (msg.contains("t") && msg["t"].is_string()) {
            event_type = msg["t"].get<std::string>();
        }
        
        if (event_type == "MESSAGE_CREATE") {
            std::cout << msg["d"]["content"] << std::endl;
        } else if (event_type == "MESSAGE_UPDATE") {
            std::cout << msg["d"]["content"] << std::endl;
        } else if (event_type == "MESSAGE_DELETE") {
            //handle_message(msg["d"]);
        }
    }
}

void MessageSocket::connect() {
    try {
        tcp::resolver resolver(io_ctx);

        auto results = resolver.resolve("gateway.discord.gg", "443");
        net::connect(beast::get_lowest_layer(ws), results);

        SSL_set_tlsext_host_name(ws.next_layer().native_handle(), "gateway.discord.gg");

        ws.next_layer().handshake(ssl::stream_base::client);

        ws.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
            req.set(beast::http::field::user_agent, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:146.0) Gecko/20100101 Firefox/146.0");
        }));

        ws.handshake("gateway.discord.gg", "/?encoding=json&v=9&compress=zlib-stream");

        handle_connection();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    if (heartbeat_thread.joinable()) {
        heartbeat_thread.join();
    }

    if (ws.is_open()) {
        ws.close(websocket::close_code::normal);
    }

    if (io_ctx.stopped()) {
        io_ctx.stop();
    }

    inflateEnd(&decompressor);
    inflateInit(&decompressor);
}
