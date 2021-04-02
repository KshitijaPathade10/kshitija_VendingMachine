#ifndef VENDING_MACHINE_MICROSERVICES_BeveragePreferenceHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_BeveragePreferenceHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/BeveragePreferenceService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine{

class BeveragePreferenceServiceHandler : public BeveragePreferenceServiceIf {
 public:
    BeveragePreferenceServiceHandler();
  ~BeveragePreferenceServiceHandler() override=default;

  void getBeverage(std::string& _return,const BeverageType::type btype) override;
};


 BeveragePreferenceServiceHandler::BeveragePreferenceServiceHandler() {
}


void BeveragePreferenceServiceHandler::getBeverage(std::string& _return,BeverageType::type btype){
     printf("BeveragePreference\n");

            std::string hot_Beverage[3] = {"cappuccino", "latte", "espresso"};
	    std::string cold_Beverage[3] = { "lemonade", "ice tea", "soda"};


	    srand(time(0));
	    
	    std::cout << "Beverage TYPE" <<btype<< std::endl;

	    if ( btype == BeverageType::type::HOT)
		     _return = hot_Beverage[rand() % 3 ];
	    else
		    _return = cold_Beverage[rand() % 3];

}
}
#endif 
