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
#include "switch.h"
#include "fifo.h"
#include "switch_reg.h"
#include <vector>


using std::vector;
void mcast_pkt_switch :: entry()
{
  wait();

  // declarations
  vector<switch_reg> reg_vec;//实例化寄存器向量
  vector<fifo> qin_vec,qout_vec;//实例化输出和输入队列
  for (int i = 0; i < SPINE_NUM; i++)
  {
    switch_reg R_temp;
    fifo qin_temp;
    fifo qout_temp;
    reg_vec.push_back(R_temp);
    qin_vec.push_back(qin_temp);
    qout_vec.push_back(qout_temp);

  }//生成16个

   for (int i = 0; i < SPINE_NUM; i++)
  {
    reg_vec[i].free=true;
    qin_vec[i].empty=true;
    qin_vec[i].full=false;
    qin_vec[i].pntr=0;
    qout_vec[i].empty=true;
    qout_vec[i].full=false;
    qout_vec[i].pntr=0;
  }//指定初始值
  int sim_count=0;
  int pkt_count=0;
  int drop_count=0;
  // result = fopen("result","w");

  cout << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  cout << endl << "             4x4 Multicast Helix Packet Switch Simulation" << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  cout << "  This is the simulation of a 4x4 non-blocking multicast helix packet switch.  " << endl; 
  cout << "  The switch uses a self-routing ring of shift registers to transfer cells     " << endl;
  cout << "  from one port to another in a pipelined fashion, resolving output contention " << endl;
  cout << "  and handling multicast switch efficiently." << endl << endl;
 
  wait();
  // functionality
  while( sim_count++ < SIM_NUM )
    { 
       wait();
    
       


       /////read input packets//////////////////////////////////     
      
   for (int i = 0; i < SPINE_NUM; i++)
       {
         sc_in<pkt> &in_temp=*in_vec[i];

         if (in_temp.event())//第i个spine发了包
         {
         pkt_count++;

         qin_vec[i].pkt_in(in_temp.read());//写入qin_vec
       };  
         }
         
       }
      /////move the packets from fifo to shift register ring/////
         if((bool)switch_cntrl && switch_cntrl.event())
	{ 
    
 int dest_spine;
   for (int i = 0; i < SPINE_NUM; i++)
   {
     
    if((!qin_vec[i].empty)&&reg_vec[i].free)
    {
     pkt temp=qin_vec[i].pkt_out();//读取包
    dest_spine=temp.dest.range(SPINE_IP_NUM-1,0);//获取目的spineid
    qout_vec[dest_spine].pkt_in(temp);//写入对应spineid的qout
     sc_out<pkt> &out_temp=*out_vec[dest_spine];
    out_temp.write(qout_vec[dest_spine].pkt_out());//将包写入到对应输出端口

      }
    }
   }
   
   
  sc_stop();

  cout << endl << endl << "-------------------------------------------------------------------------------" << endl;
  cout << "End of switch operation..." << endl;
  cout << "Total number of packets received: " <<  pkt_count << endl;
  cout << "Total number of packets dropped: " <<  drop_count << endl;
  cout << "Percentage packets dropped:  " <<  drop_count*100/pkt_count << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  
}
