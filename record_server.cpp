#include "record_server.h"


using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

//ThreadPool
static MYWORK::ThreadPool threadPool(POOL_SIZE);

void* func(void *arg) {
    struct MYWORK::parameter *p_arg = (MYWORK::parameter *) arg;
    std::string rtmp_url = p_arg->rtmp_url;
    Json::Value clientData = p_arg->clientData;

    MYWORK::ParseConf json(rtmp_url, clientData);
    std::string type = json.get_type();
    MYWORK::RtmpServer server(json, PRE_CHATROOM_USERID);
    server.record();
    delete p_arg;
}

// Define a callback to handle incoming messages
void on_message(MYWORK::server* s, websocketpp::connection_hdl hdl, MYWORK::message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    // parse json
    Json::Reader reader;
    Json::Value clientData;
    if (!reader.parse(msg->get_payload(), clientData)) {
        fprintf(stderr, "%s: Main ParseFromArray failed.", __FILE__);
        return;
    }

    std::string type;
    try {
        type = clientData["type"].asString();
    } catch (std::exception& e) {
        fprintf(stderr, "Json parse error %s \n", e.what());
    }
    if (0 == type.compare("ping")) {
        try {
            s->send(hdl, "record server is working\n", msg->get_opcode()); 
        } catch (const websocketpp::lib::error_code& e) {
            std::cout << "Echo failed because: " << e
            << "(" << e.message() << ")" << std::endl;
        } 
        return;
    } else if (0 == type.compare("record") ) {
        struct MYWORK::parameter *arg = new MYWORK::parameter(RTMP_URL, clientData);
        threadPool.addTask(boost::bind(&func, arg));
        return; 
    }
    return;
}
 
int main() {
    // Create a server endpoint
    MYWORK::server record_server;

    try {
        // Set logging settings
        record_server.set_access_channels(websocketpp::log::alevel::all);
        record_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        record_server.init_asio();
        
        //Enable reuse addr
        record_server.set_reuse_addr(true);
        
        // Register our message handler
        record_server.set_message_handler(bind(&(on_message), &record_server, ::_1, ::_2));

        // Listen on port 9002
        record_server.listen(9002);

        // Start the server accept loop
        record_server.start_accept();

        // Start the ASIO io_service run loop
        record_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
