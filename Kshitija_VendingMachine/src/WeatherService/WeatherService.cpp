#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include "../utils.h"
#include "WeatherHandler.h"

using json = nlohmann::json;
using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace vending_machine;

void sigintHandler(int sig) {
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  signal(SIGINT, sigintHandler);
  init_logger();

  json config_json;
  if (load_config_file("config/service-config.json", &config_json) != 0) {
    exit(EXIT_FAILURE);
  }

  int my_port = config_json["weather-service"]["port"];

  TThreadedServer server(
      std::make_shared<WeatherServiceProcessor>(
          std::make_shared<WeatherServiceHandler>()),
      std::make_shared<TServerSocket>("0.0.0.0", my_port),
      std::make_shared<TFramedTransportFactory>(),
      std::make_shared<TBinaryProtocolFactory>()
  );
  
  std::cout << "Starting the weather server ..." << std::endl;
  server.serve();
  return 0;
}

