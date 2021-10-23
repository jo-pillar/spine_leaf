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

  switch.cpp - This is the implementation file for the asynchronous process
               "switch".

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
#include "leaf.h"
#include "fifo.h"
#include "switch_reg.h"
leaf::leaf(sc_module_name name, sc_int<LEAF_IP_NUM> leaf_ip, int spine_port_num, int host_port_num) :
 sc_module(name),
 leaf_ip(leaf_ip),
 spine_port_num(spine_port_num),
 host_port_num(host_port_num)
{
  for (int i = 0; i < spine_port_num; i++)
  {
    sc_in<pkt> *in_temp = new sc_in<pkt>;
    sc_out<pkt> *out_temp = new sc_out<pkt>;
    spine_port_in.push_back(in_temp);
    spine_port_out.push_back(out_temp);
  }
  for (int i = 0; i < host_port_num; i++)
  {
    sc_in<pkt> *in_temp = new sc_in<pkt>;
    sc_out<pkt> *out_temp = new sc_out<pkt>;
    host_port_in.push_back(in_temp);
    host_port_out.push_back(out_temp);
  }
  SC_THREAD(entry);
  sensitive << switch_cntrl.pos();
  for (int i = 0; i < spine_port_num; i++)
  {
    sensitive << *spine_port_in[i];
  }
  for (int i = 0; i < host_port_num; i++)
  {
    sensitive << *host_port_in[i];
  }
}
leaf::~leaf()
{
  spine_port_in.~vector();
  spine_port_out.~vector();
  host_port_in.~vector();
  host_port_out.~vector();
}
void leaf ::entry()
{
  wait();
  

  // result = fopen("result","w");

  /* cout << endl;
   cout << "-------------------------------------------------------------------------------" << endl;
   cout << endl << "             LEAF Switch Simulation" << endl;
   cout << "-------------------------------------------------------------------------------" << endl;
   cout << "  This is the simulation of a 4x4 non-blocking multicast helix packet switch.  " << endl;
   cout << "  The switch uses a self-routing ring of shift registers to transfer cells     " << endl;
   cout << "  from one port to another in a pipelined fashion, resolving output contention " << endl;
   cout << "  and handling multicast switch efficiently." << endl << endl;
  */

  // functionality
  int pkt_count = 0;
  vector<fifo> host_qin, host_qout;   // host 出入缓存
  vector<fifo> spine_qin, spine_qout; // spine 出入缓存

  for (int i = 0; i < host_port_num; i++)
  {
    fifo qin_temp;
    fifo qout_temp;
    host_qin.push_back(qin_temp);
    host_qout.push_back(qout_temp);
  }
  for (int i = 0; i < spine_port_num; i++)
  {
    fifo qin_temp;
    fifo qout_temp;
    spine_qin.push_back(qin_temp);
    spine_qout.push_back(qout_temp);
  }
  while(pkt_count<SIM_NUM)
  {
  
    wait();

    //读取host 口
    for (int i = 0; i < host_port_num; i++)
    {
      sc_in<pkt> &in_temp = *host_port_in[i];
      if (in_temp.event())
      {
        pkt_count++;
        host_qin[i].pkt_in(in_temp.read());
      }
    }

    //读取spine 口
    for (int i = 0; i < spine_port_num; i++)
    {
      sc_in<pkt> &in_temp = *spine_port_in[i];
      if (in_temp.event())
      {
        pkt_count++;
        spine_qin[i].pkt_in(in_temp.read());
      }
    }

    // host 发往spine
    int dest_spine = 0; //循环发送给spine
    for (int i = 0; i < host_port_num; i++)
    {
      while (!host_qin[i].empty)
      {
        pkt host_pkt = host_qin[i].pkt_out();

        dest_spine = dest_spine % spine_port_num;
        dest_spine++;

         //第一次注入Leaf ip；
  for (int j = SPINE_IP_NUM; j < (SPINE_IP_NUM + LEAF_IP_NUM); j++)
  {
  host_pkt.data[j] = leaf_ip[j - SPINE_IP_NUM];
  }// 结束

        spine_qout[dest_spine].pkt_in(host_pkt);
        if ((bool)switch_cntrl && switch_cntrl.event())
        {
          if (!host_qout[dest_spine].empty)
          {
            sc_out<pkt> &out_temp = *spine_port_out[i];
            out_temp.write(spine_qout[dest_spine].pkt_out());
          }
        }
      }
    }

    

    // spine发往host
    int dest_host = 0; //目的host  dest_host=dest%4;
    for (int i = 0; i < spine_port_num; i++)
    {
      while (!spine_qin[i].empty)
      {
        pkt spine_pkt = spine_qin[i].pkt_out();
        dest_host = spine_pkt.dest % (HOST_NUM / LEAF_NUM);

        //第二次注入Leaf ip；
  for (int j= SPINE_IP_NUM + LEAF_IP_NUM; j < (SPINE_IP_NUM + LEAF_IP_NUM*2); j++)
  {
   spine_pkt.data[j] = leaf_ip[j -  (SPINE_IP_NUM + LEAF_IP_NUM)];
  }//结束

        host_qout[dest_host].pkt_in(spine_pkt);
        if ((bool)switch_cntrl && switch_cntrl.event())
        {
          if (!host_qout[dest_host].empty)
          {
            sc_out<pkt> &out_temp = *host_port_out[i];
            out_temp.write(host_qout[dest_host].pkt_out());
          }
        }
      }
    }
  }
}
