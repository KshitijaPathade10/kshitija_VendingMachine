#ifndef VENDING_MACHINE_MICROSERVICES_WEATHERHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_WEATHERHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/WeatherService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine{

class WeatherServiceHandler : public WeatherServiceIf {
 public:
  WeatherServiceHandler();
  ~WeatherServiceHandler() override=default;

  WeatherType::type GetWeather(const int64_t city) override;
};

WeatherServiceHandler::WeatherServiceHandler() {

}

WeatherType::type WeatherServiceHandler::GetWeather(const int64_t city) {

    return (city % 2 == 0 )? WeatherType::type::COLD : WeatherType::type::WARM;
}

}


#endif 

