#ifndef SPINE_H_INCLUED
#define SPINE_H_INCLUED
#include "systemc.h"
#include "pkt.h"
struct spine: sc_module {
    sc_in<bool>  switch_cntrl;
    sc_in<pkt>  in0;
    sc_in<pkt>  in1;
    sc_in<pkt>  in2;


    sc_out<pkt>  out0;//总线端口
    sc_out<pkt>  out1;//leaf0端口
    sc_out<pkt>  out2;//leaf1端口
     sc_int<ADR_NUM-3> Spineid;// [2][1] [core_id][leaf_id]


    SC_CTOR(spine);
    spine(sc_module_name name,sc_int<ADR_NUM-3> Spineid):
    sc_module(name),Spineid(Spineid)
     {
      SC_THREAD(entry);
      sensitive << in0;
      sensitive << in1;
      sensitive << in2;

      sensitive << switch_cntrl.pos();
    }  

  void entry();  
 
};

#endif //Spine_H_INCLUED