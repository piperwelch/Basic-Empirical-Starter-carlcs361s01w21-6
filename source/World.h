#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/data/DataFile.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"

#include "Org.h"

class OrgWorld : public emp::World<Organism> {

    double resources_per_org = 100;
    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;
    
    emp::Ptr<emp::DataMonitor<double, emp::data::Histogram>> data_node_orgcoop;
    emp::Ptr<emp::DataMonitor<int>> data_node_orgcount;

    public:
    emp::vector<int> resources;
    int numRSS;
    int maxRSS;

    OrgWorld(emp::Random &random, int _maxRSS, int _numRSS) : emp::World<Organism>(random), random(random), maxRSS(_maxRSS), numRSS(_numRSS) {
        
        random_ptr.New(random);
        SetPopStruct_Grid(100, 100);
        resources.assign(10000, 0);
        addResources();
    }

    ~OrgWorld() {
        if(data_node_orgcoop) data_node_orgcoop.Delete();
        if(data_node_orgcount) data_node_orgcount.Delete();
    }

    //iterate overall population and if is neighbor is true then do the thing 
    bool isNeighbor(int id1, int id2){
      emp_assert(pop_sizes.size() == 2);

      int size_x = (int) pop_sizes[0];
      int size_y = (int) pop_sizes[1];

      int diff = id1 - id2;
      int row_diff = abs(diff / size_x);
      int col_diff = abs(diff%size_x);

      if((row_diff <= 1 || row_diff == (size_y-1)) && 
        (col_diff <= 1 || col_diff == (size_x-1)))	return true;
      else 
      return false;
    }

    emp::World<Organism>::pop_t getPop() {
      return pop;
      }


    int checkQuorumSense(int loc){
      /*how to incorporate where the rss is?*/ 
      double numCoop = 0;
      for (int i = 0; i < pop.size(); i++){
        //std::cout << "hello";
        if (i == loc){
          continue; 
        } if (IsOccupied(i) == false){
          continue;
        } if (isNeighbor(loc, i)){
          double randCoopProb = random.GetDouble();
          if (pop[loc]->coop_prob > randCoopProb and pop[i]->coop_prob > randCoopProb){
            numCoop++;
          }
        }
      }
      return numCoop; 
    }

    void quorumSense(int loc){
      double numCoop = checkQuorumSense(loc);
      
      if (numCoop/8 > .6){
        pop[loc]->points += resources[loc];
      }
    }

    bool individualRSSacq(int location){
      
      double randCoopProb = random.GetDouble();
      double rssProb = random.GetDouble();
      if (pop[location]->coop_prob < randCoopProb){
        //std::cout << pop[location]->coop_prob << std::endl;
        for (int i = 0; i < 10000; i++){
            if (isNeighbor(location, i)) 
              if (0.3 > rssProb){
                //std::cout << "SUCCESSFUL INDIVIDUAL RSS ACQ" << std::endl;
                pop[location]->points += resources[i];
                resources[i] = 0;
                return true;
          }
        }
      }
      return false;
   } 

    void addResources(){
      int count = 0;
      /*finds the number of resources needed to be added*/
      for (int i = 0; i < resources.size(); i++){
        if (resources[i] != 0){
          count++;
          }
        }
      if (count < numRSS){
        int diff = numRSS - count;
        int count2 = 0;
        while (count2 <= diff){
          //comments!
          int num = random.GetInt(resources.size());
          if (resources[num] == 0){
            int rssToAdd = random.GetInt(1, maxRSS);
            resources[num] = rssToAdd;
            count2++;
          }
        }
      }
    }
    emp::DataMonitor<int>& GetOrgCountDataNode() {
        if(!data_node_orgcount) {
        data_node_orgcount.New();
        OnUpdate([this](size_t){
            data_node_orgcount -> Reset();
            for (size_t i = 0; i< pop.size(); i++)
            if(IsOccupied(i))
                data_node_orgcount->AddDatum(1);
        });
        }
        return *data_node_orgcount;
    }
    emp::DataMonitor<double, emp::data::Histogram>& GetOrgCoopValDataNode() {
        if (!data_node_orgcoop) {
        data_node_orgcoop.New();
        OnUpdate([this](size_t){
            data_node_orgcoop->Reset();
            for (size_t i = 0; i< pop.size(); i++)
            if (IsOccupied(i))
                data_node_orgcoop->AddDatum(pop[i]->getCoopProb());
        });
        }
        return *data_node_orgcoop;
    }

    emp::DataFile & SetupOrgFile(const std::string & filename) {
    auto & file = SetupFile(filename);
    auto & node1 = GetOrgCountDataNode();
    auto & node = GetOrgCoopValDataNode();
    node.SetupBins(0.0, 1.1, 10); //Necessary because range exclusive
    file.AddVar(update, "update", "Update");
    file.AddMean(node, "mean_coopval", "Average organism cooperation value");
    file.AddTotal(node1, "count", "Total number of organisms");
    file.AddHistBin(node, 0, "Hist_0.0", "Count for histogram bin 0.0 to <0.1");
    file.AddHistBin(node, 1, "Hist_0.1", "Count for histogram bin 0.1 to <0.2");
    file.AddHistBin(node, 2, "Hist_0.2", "Count for histogram bin 0.2 to <0.3");
    file.AddHistBin(node, 3, "Hist_0.3", "Count for histogram bin 0.3 to <0.4");
    file.AddHistBin(node, 4, "Hist_0.4", "Count for histogram bin 0.4 to <0.5");
    file.AddHistBin(node, 5, "Hist_0.5", "Count for histogram bin 0.5 to <0.6");
    file.AddHistBin(node, 6, "Hist_0.6", "Count for histogram bin 0.6 to <0.7");
    file.AddHistBin(node, 7, "Hist_0.7", "Count for histogram bin 0.7 to <0.8");
    file.AddHistBin(node, 8, "Hist_0.8", "Count for histogram bin 0.8 to <0.9");
    file.AddHistBin(node, 9, "Hist_0.9", "Count for histogram bin 0.9 to 1.0");


    file.PrintHeaderKeys();

    return file;
  }

  

  void Update() {
      emp::World<Organism>::Update();

      emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
      double total_coop = 0;
      for (size_t i: schedule) {
          if (IsOccupied(i) == false){
            continue;
          }
          individualRSSacq(i);
          quorumSense(i);
          emp::Ptr<Organism> offspring = pop[i]->checkReproduction();

          if(offspring) {
              DoBirth(*offspring, i);
          }
          total_coop += pop[i]->getCoopProb();
      }
      
      addResources();
      //std::cout << "Average cooperation probability: " << total_coop/GetNumOrgs() <<std::endl;
      /* how many organisms to start with?
      how many resources and resource values?
      what mutation value?
      what is the replication threshold?*/
  }
};
#endif