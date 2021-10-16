/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  main.cpp - This file instantiates all processes and ties them together
             with signals.

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "pkt.h"
#include "switch_clk.h"
#include "switch.h"
#include "host.h"
#include "leaf.h"
#include "spine.h"
#include <vector>
using std::vector;

int sc_main(int, char *[])
{

  sc_signal<bool> switch_cntrl;//控制信号

//初始化时钟
  sc_clock clock1("CLOCK1", 75, SC_NS, 0.5, 0.0, SC_NS);
  sc_clock clock2("CLOCK2", 30, SC_NS, 0.5, 10.0, SC_NS);

 // 实例化向量变量
  vector<sc_signal<pkt> *> pktin_vec, pktout_vec,spipktin_vec,spipktout_vec,switchpktin_vec,switchpktout_vec;
  vector<sc_signal<sc_int<7>> *> id_vec;
  for (int i = 0; i < HOST_NUM; i++)
  {
    //实例化PKT IN
    sc_signal<pkt> *intemp = new sc_signal<pkt>;
    pktin_vec.push_back(intemp);
    //实例化PKTout
    sc_signal<pkt> *outtemp = new sc_signal<pkt>;
    pktout_vec.push_back(outtemp);
    //初始化 id
    sc_signal<sc_int<7>> *id_temp = new sc_signal<sc_int<7>>;
    id_vec.push_back(id_temp);
    sc_signal<sc_int<7>> &id_ttemp = *id_temp;
    //指定id
    id_ttemp.write(i);
  }
  for (int i = 0; i < LEAF_NUM; i++)
  {
     //实例化SPINE_PKT IN
    sc_signal<pkt> *intemp = new sc_signal<pkt>;
    spipktin_vec.push_back(intemp);
    //实例化SPINE_PKTout
    sc_signal<pkt> *outtemp = new sc_signal<pkt>;
    spipktout_vec.push_back(outtemp);
  }
  for (int i = 0; i < SPINE_NUM; i++)
  {
      //实例化Switch_PKT IN
    sc_signal<pkt> *intemp = new sc_signal<pkt>;
    switchpktin_vec.push_back(intemp);
    //实例化switch_PKTout
    sc_signal<pkt> *outtemp = new sc_signal<pkt>;
    switchpktout_vec.push_back(outtemp);
  }
  
  

  //实例化HOST
  vector<host *> host_vec;
  for (int i = 0; i < HOST_NUM; i++)
  {
    char Host_name = 'HOST' + char(i);
    const char const_host_name = Host_name;
    host temp(&const_host_name);
    host_vec.push_back(&temp);
  }


  //实例化 LEAF
  vector<leaf *> leaf_vec;
  for (int i = 0; i < LEAF_NUM; i++)
  {
    char Leaf_Name = 'LEAF' + char(i);
    const char const_leaf_name = Leaf_Name;
    leaf temp(&const_leaf_name,i);
    leaf_vec.push_back(&temp);
  }


  //实例化 Spine
  vector<spine *> spine_vec;
  for (int i = 0; i < SPINE_NUM; i++)
  {
    char Spine_Name = 'SPINE' + char(i);
    const char Const_Spine_Name = Spine_Name;
    spine temp(&Const_Spine_Name,i);
    spine_vec.push_back(&temp);
  }


  //初始化HOST并绑定leaf端口
  for (int i = 0; i < HOST_NUM; i++)
  {
    host &host_temp = *host_vec[i];
    host_temp.pkt_out(*pktin_vec[i]);
    host_temp.pkt_in(*pktout_vec[i]);
    host_temp.CLK(clock1);//指定时钟
    host_temp.id(*id_vec[i]);
    leaf &leaf_temp = *leaf_vec[i / 2];
    if (i % 2)//host1对应in2
    {
      leaf_temp.in2(*pktin_vec[i]);
      leaf_temp.out2(*pktout_vec[i]);
    }
    else//host0对应in1
    {
      leaf_temp.in1(*pktin_vec[i]);
      leaf_temp.out1(*pktout_vec[i]);
    }
  }
  switch_clk switch_clk1("SWITCH_CLK");
  // hooking up signals to ports by name
  switch_clk1.switch_cntrl(switch_cntrl);
  switch_clk1.CLK(clock2);
  //初始化LEAF绑定spine端口
  for (int i = 0; i < LEAF_NUM; i++)
  {
     leaf &leaf_temp =*leaf_vec[i];
     leaf_temp.in0(*spipktin_vec[i]);
     leaf_temp.out0(*spipktout_vec[i]);
     leaf_temp.switch_cntrl(switch_cntrl);
        spine &spine_temp=*spine_vec[i/2];
     if(i%2){//同上
       spine_temp.out2(*spipktout_vec[i]);
       spine_temp.in2(*spipktin_vec[i]);
     }else{
        spine_temp.out1(*spipktout_vec[i]);
       spine_temp.in1(*spipktin_vec[i]);
     }
  }
    mcast_pkt_switch switch1("SWITCH");
  for (int i = 0; i < SPINE_NUM; i++)
  {
    spine &spine_temp=*spine_vec[i];
    spine_temp.in0(*switchpktout_vec[i]);
    spine_temp.out0(*switchpktin_vec[i]);
    spine_temp.switch_cntrl(switch_cntrl);
   sc_in<pkt>&in_temp= *(switch1.in_vec[i]);
   in_temp(*switchpktin_vec[i]);
    sc_out<pkt>&out_temp= *(switch1.out_vec[i]);
   out_temp(*switchpktout_vec[i]);
  }
  

  
  //初始化SPINE

 


  sc_start();
  return 0;
}
