#ifndef LEAF_H_INCLUED
#define LEAF_H_INCLUED
#include "systemc.h"
#include "pkt.h"

struct leaf: sc_module {
  SC_HAS_PROCESS(leaf);
  int spine_port_num;
  int host_port_num;
   sc_int<LEAF_IP_NUM> leaf_ip;// [2][1] [core_id][leaf_ip]
  vector<sc_in<pkt>*> spine_port_in;
    vector<sc_in<pkt>*> host_port_in;

    sc_in<bool>  switch_cntrl;
    vector<sc_out<pkt>*> spine_port_out;
    vector<sc_out<pkt>*> host_port_out;
leaf(sc_module_name name,sc_int<LEAF_IP_NUM> leaf_ip,int spine_port_num,int host_port_num);
  ~leaf();
  void entry();  
 
};
   

#endif //LEAF_H_INCLUED