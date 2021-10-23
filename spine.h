#ifndef SPINE_H_INCLUED
#define SPINE_H_INCLUED
#include "systemc.h"
#include "pkt.h"
struct spine: sc_module {
  SC_HAS_PROCESS(spine);
  int leaf_port_num;
  sc_int<SPINE_IP_NUM>spine_ip;
  vector<sc_in<pkt>*>  leaf_port_in;
  vector<sc_out<pkt>*> leaf_port_out;
  spine(sc_module_name name,sc_int<SPINE_IP_NUM> spine_ip,int leaf_port_num);
  sc_in<bool>  switch_cntrl;
  ~spine();
  void entry();  
 
};

#endif //Spine_H_INCLUED