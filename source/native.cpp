//  This file is part of Evolutionary Algorithm
//  Copyright (C) Anya Vostinar, 2021.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/base/vector.hpp"
#include "emp/config/command_line.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"    
#include "emp/config/ArgManager.hpp" 
 
#include "World.h"   
      
// This is the main function for the NATIVE version of this project.
  
  
EMP_BUILD_CONFIG(MyConfigType,
    VALUE(SEED, int, 10, "What value should the random seed be?"), 
    VALUE(START_PROB, double, 0.5, "What cooperation probability value should the starting organism have?"),
    VALUE(maxRss, int, 2500, "What should be the highest value resource?"),
    VALUE(numRss, int, 7000, "What should be the number of resources placed on the board?"),
    VALUE(FILE_NAME, std::string, "_data.dat", "Root output file name")
)

int main(int argc, char* argv[])
{
  
  MyConfigType config;
  bool success = config.Read("MySettings.cfg");

  if(!success) {
    config.Write("MySettings.cfg");
  }
  
  auto args = emp::cl::ArgManager(argc, argv);
  if (args.ProcessConfigOptions(config, std::cout, "MySettings.cfg") == false) {
  std::cerr << "There was a problem in processing the options file." << std::endl;
  exit(1);
  } if (args.TestUnknown() == false) {
  std::cerr << "Leftover args no good." << std::endl;
  exit(1);
  } 

  /*What is this?*/
 // emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);
 

  emp::Random random(config.SEED());
  
  OrgWorld world(random, config.maxRss(), config.numRss());
 

  world.SetupOrgFile("data.csv"); 
  world.Resize(100,100);
  
  for (int i = 0; i < 1000; i++){
    emp::Ptr<Organism> new_org = new Organism(&random, 0.5);
    world.Inject(*new_org);
  }
   
 
  

  for(int i=0; i < 2300; i++) {
    //std::cout<< "Update: " << i << std::endl; 
    //   std::cout << "Population: " << world.GetNumOrgs() << std::endl;
    world.Update();
  }
}
