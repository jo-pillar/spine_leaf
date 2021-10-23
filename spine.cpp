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
#include "spine.h"
#include "fifo.h"
#include "switch_reg.h"

spine::spine(sc_module_name name, sc_int<SPINE_IP_NUM> spine_ip, int leaf_port_num) : sc_module(name),
                                                                                      spine_ip(spine_ip),
                                                                                      leaf_port_num(leaf_port_num)
{
  for (int i = 0; i < leaf_port_num; i++)
  {
    sc_in<pkt> *in_temp = new sc_in<pkt>;
    sc_out<pkt> *out_temp = new sc_out<pkt>;
    leaf_port_in.push_back(in_temp);
    leaf_port_out.push_back(out_temp);
  }
  SC_THREAD(entry);
  sensitive << switch_cntrl.pos();
  for (int i = 0; i < leaf_port_num; i++)
  {
    sensitive << *leaf_port_in[i];
  }
}

// 析构函数
spine::~spine()
{
  leaf_port_in.~vector();
  leaf_port_out.~vector();
}

//入口函数
void spine::entry()
{
  wait();
  // int pkt_count=0//计数
  vector<fifo> leaf_qin, leaf_qout;
  for (int i = 0; i < leaf_port_num; i++)
  {
    fifo qin_temp;
    fifo qout_temp;
    leaf_qin.push_back(qin_temp);
    leaf_qout.push_back(qout_temp);
  }
  while (true)
  {
    wait();

    //读取leaf 口
    for (int i = 0; i < leaf_port_num; i++)
    {
      sc_in<pkt> &in_temp = *leaf_port_in[i];
      if (in_temp.event())
      {
        // pkt_count++;// 计数
        leaf_qin[i].pkt_in(in_temp.read());
      }
    }

    // leaf 发往leaf
    int dest_leaf = 0; //目的leaf dest_leaf=dest/(leaf_NUM/LEAF_NUM)
    for (int i = 0; i < leaf_port_num; i++)
    {

      while (!leaf_qin[i].empty)
      {
        pkt leaf_pkt = leaf_qin[i].pkt_out();
        dest_leaf = leaf_pkt.dest / (HOST_NUM / LEAF_NUM);

        //注入Spine ip
        for (int j = 0; j < SPINE_IP_NUM; j++)
        {
          leaf_pkt.data[j] = spine_ip[j];
        }

        leaf_qout[dest_leaf].pkt_in(leaf_pkt);
        // 发包
        if ((bool)switch_cntrl && switch_cntrl.event())
        {
          if (!leaf_qout[dest_leaf].empty)
          {
            sc_out<pkt> &out_temp = *leaf_port_out[i];
            out_temp.write(leaf_qout[dest_leaf].pkt_out());
          }
        }
      }
    }

  }
}