#ifndef LEAF_H_INCLUED
#define LEAF_H_INCLUED
#include "systemc.h"
#include "pkt.h"
struct leaf: sc_module {
    sc_in<bool>  switch_cntrl;
    sc_in<pkt>  in0;
    sc_in<pkt>  in1;
    sc_in<pkt>  in2;


    sc_out<pkt>  out0;//core端口
    sc_out<pkt>  out1;//host0端口
    sc_out<pkt>  out2;//host1端口
    sc_in<int<3>> ieaf_id// [2][1] [core_id][leaf_id]


    SC_CTOR(mcast_pkt_switch) 
     {
      SC_THREAD(entry);
      sensitive << in0;
      sensitive << in1;
      sensitive << in2;

      sensitive << switch_cntrl.pos();
    }  

  void entry();  
 
};

#endif //LEAF_H_INCLUED