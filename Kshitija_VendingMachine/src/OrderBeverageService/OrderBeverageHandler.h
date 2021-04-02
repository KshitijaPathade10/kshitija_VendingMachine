#ifndef VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/OrderBeverageService.h"
#include "../../gen-cpp/WeatherService.h"
#include "../../gen-cpp/BeveragePreferenceService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine{

class OrderBeverageServiceHandler : public OrderBeverageServiceIf {
 public:
  OrderBeverageServiceHandler(
		  ClientPool<ThriftClient<WeatherServiceClient>> *, ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *);
  ~OrderBeverageServiceHandler() override=default;

  void PlaceOrder(std::string& _return, const int64_t city) override;
 private:
  ClientPool<ThriftClient<WeatherServiceClient>> *_weather_client_pool;
  private:
  ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *_beveragePreference_client_pool;
};


OrderBeverageServiceHandler::OrderBeverageServiceHandler(
		ClientPool<ThriftClient<WeatherServiceClient>> *weather_client_pool,
                ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *beveragePreference_client_pool) {
     _weather_client_pool = weather_client_pool;
     _beveragePreference_client_pool = beveragePreference_client_pool;
      
}


void OrderBeverageServiceHandler::PlaceOrder(std::string& _return, const int64_t city){
     printf("PlaceOrder\n");

#if 1   
    auto weather_client_wrapper = _weather_client_pool->Pop();
    if (!weather_client_wrapper) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
      se.message = "Failed to connect to weather-service";
      throw se;
    }
    auto weather_client = weather_client_wrapper->GetClient();
      auto beveragePreference_client_wrapper = _beveragePreference_client_pool->Pop();
      if (!beveragePreference_client_wrapper) {
             ServiceException se;
             se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
             se.message = "Failed to connect to beveragepreference service";
             throw se;
         }
       auto beveragePreference_client = beveragePreference_client_wrapper->GetClient();

   WeatherType::type weatherType;
   BeverageType::type btype;
    try {
      weatherType = weather_client->GetWeather(city);
    } catch (...) {
      _weather_client_pool->Push(weather_client_wrapper);
      LOG(error) << "Failed to send call GetWeather to weather-client";
      throw;
    }
     
   
    _weather_client_pool->Push(weather_client_wrapper);
         std::string a = "";
	std::string& preference = a ;	
   if(weatherType == WeatherType::type::WARM){
	   BeverageType::type btype = BeverageType::type::COLD;
	   std::cout<<"Beverage type is = "<<btype << std::endl;
	   
	   try{
		   beveragePreference_client->getBeverage(preference, btype);
	   } catch(...) {
		     _beveragePreference_client_pool->Push(beveragePreference_client_wrapper);
			 LOG(error) << " Failed to send call getBeverage TO Beverage-Client";
			 throw;
	   }
	   _beveragePreference_client_pool->Push(beveragePreference_client_wrapper);
	   _return = preference;
   }
		
 else {
	 BeverageType::type btype = BeverageType::type::HOT;
	  std::cout<<" Beverage Type  btype = "<< btype << std::endl;
	  try {
		  beveragePreference_client->getBeverage(preference, btype);
	  } catch(...) {
		       _beveragePreference_client_pool->Push(beveragePreference_client_wrapper);
			   LOG(error) << "Failed to send call to function getBeverage to Beverage Client ";
			   throw;
	  }
	   _beveragePreference_client_pool->Push(beveragePreference_client_wrapper);
	   _return = preference;
	   
 }
 #endif

}

}

#endif

