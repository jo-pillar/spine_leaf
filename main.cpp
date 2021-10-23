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
#include "host.h"
#include "leaf.h"
#include "spine.h"
#include <vector>
#include<string>
using std::string;
using std::vector;

int sc_main(int, char *[])
{

const int host_port_num=HOST_NUM/LEAF_NUM;
const int spine_port_num=SPINE_NUM;
const int leaf_port_num= LEAF_NUM;
//初始化时钟 参数调整
sc_clock host_clock("host_clock", 120, SC_NS, 0.5, 60, SC_NS);
sc_clock leaf_clock("leaf_clock", 100, SC_NS, 0.5, 0.0, SC_NS);
sc_clock spine_clock("spine_clock", 100, SC_NS, 0.5, 0.0, SC_NS);
switch_clk  leaf_switch_clk("LEAF_SWITCH_CLK");
switch_clk  spine_switch_clk("SPINE_SWITCH_CLK");
sc_signal<bool> leaf_switch_cntrl;
sc_signal<bool> spine_switch_cntrl;
spine_switch_clk.CLK(spine_clock);
spine_switch_clk.switch_cntrl(spine_switch_cntrl);
leaf_switch_clk.CLK(leaf_clock);
leaf_switch_clk.switch_cntrl(leaf_switch_cntrl);

//实例化 信号向量变量
 vector<sc_signal<pkt> *> host_signal_in,host_signal_out;//host--leaf 信号 in out 相对host 而言

vector<sc_signal<pkt> *>spine_signal_in,spine_signal_out;//spine--leaf 信号 in out 相对spine 而言

// 
for (int i = 0; i < HOST_NUM ; i++)
{
   sc_signal<pkt> *intemp = new sc_signal<pkt>;
    host_signal_in.push_back(intemp);
    //实例化PKTout
    sc_signal<pkt> *outtemp = new sc_signal<pkt>;
    host_signal_out.push_back(outtemp);
}


for (int i = 0; i < LEAF_NUM*SPINE_NUM; i++)
{
 sc_signal<pkt> *intemp = new sc_signal<pkt>;
    spine_signal_in.push_back(intemp);
    //实例化PKTout
    sc_signal<pkt> *outtemp = new sc_signal<pkt>;
    spine_signal_out.push_back(outtemp);
}

// id地址
vector<sc_signal<sc_int<ADR_NUM>> *> id_signal;
for (int i = 0; i < HOST_NUM ; i++)
{sc_signal<sc_int<ADR_NUM>> *id_temp = new sc_signal<sc_int<ADR_NUM>>;
    id_signal.push_back(id_temp);
    sc_signal<sc_int<ADR_NUM>> &id_ttemp = *id_temp;
    //指定id
    id_ttemp.write(i);
}


//实例化HOST
  vector<host *> host_vec;
  for (int i = 0; i < HOST_NUM; i++)
  {
    string host_name = "HOST";
    string i_str=to_string(i);
    host_name=host_name+i_str;
    host *temp=new host(host_name.c_str());
    temp->id(*id_signal[i]);
    host_vec.push_back(temp);

  }

  //实例化leaf
 vector<leaf *> leaf_vec;
  for (int i = 0; i < LEAF_NUM; i++)
  {
    sc_int<LEAF_IP_NUM> leaf_ip=i;
        string leaf_name = "LEAF";
    string i_str=to_string(i);
    leaf_name=leaf_name+i_str;
    leaf *temp=new leaf(leaf_name.c_str(),leaf_ip,spine_port_num,leaf_port_num);
    temp->switch_cntrl(leaf_switch_cntrl);
    leaf_vec.push_back(temp);
  }
  // 实例化spine
  vector<spine *>spine_vec;
  for (int i = 0; i < SPINE_NUM; i++)
  {
    sc_int<SPINE_IP_NUM> spine_ip=i;
    string spine_name = "spine";
    string i_str=to_string(i);
    spine_name=spine_name+i_str;
    spine *temp=new spine(spine_name.c_str(),spine_ip,leaf_port_num);
    temp->switch_cntrl(spine_switch_cntrl);
    spine_vec.push_back(temp);
  }
   //初始化HOST并绑定leaf端口
 

  for (int i = 0; i < HOST_NUM; i++)
  {
    int leaf_id=0;// host 对应的leaf
    int leaf_port_id=0;// host 对应的leaf 的host 口编号
    host &host_temp = *host_vec[i];//第i个host
    host_temp.pkt_out(*host_signal_out[i]);
    host_temp.pkt_in(*host_signal_in[i]);

    host_temp.CLK(host_clock);//指定时钟
    host_temp.id(*id_signal[i]);
    leaf_port_id=i%host_port_num;
    leaf_id=i/host_port_num;
    leaf &leaf_temp =*(leaf_vec[leaf_id]);
    sc_in<pkt> port_in_temp=*leaf_temp.host_port_in[leaf_port_id];
    port_in_temp(*host_signal_out[i]);
    sc_out<pkt> port_out_temp=*leaf_temp.host_port_out[leaf_port_id];
    port_out_temp(*host_signal_in[i]);  
  }
  // 初始化spine 并绑定端口
  for (int i = 0; i < SPINE_NUM; i++)
  {
    spine &spine_temp=*spine_vec[i];

    for (int j = 0; j < leaf_port_num; j++)
    {
       sc_in<pkt> port_in_temp=*spine_temp.leaf_port_in[j];
       sc_out<pkt> port_out_temp=*spine_temp.leaf_port_out[j];
       int signal_id=i*leaf_port_num+j;
       port_in_temp(*spine_signal_in[signal_id]);
       port_out_temp(*spine_signal_out[signal_id]);
       leaf &leaf_temp =*(leaf_vec[j]);
       sc_in<pkt> spine_port_in_temp=*leaf_temp.spine_port_in[i];
        sc_out<pkt> spine_port_out_temp=*leaf_temp.spine_port_out[i];
        spine_port_in_temp(*spine_signal_out[signal_id]);
        spine_port_out_temp(*spine_signal_out[signal_id]);

    } 
  }
  
  // 初始化LEAF 并绑定端口 host_port_num个host，spine_port_num个spine
  sc_start();
  return 0;
}
