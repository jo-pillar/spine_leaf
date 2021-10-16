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


void leaf :: entry()
{
  wait();
  pkt Pkt_Temp;

 

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
  wait();
  // functionality
  if (in0.event())//received pkt from core
  {
    Pkt_Temp=in0.read();
    if (Pkt_Temp.dest[ADR_NUM-2]!=leafid[ADR_NUM-2])//检测目的host是否是在本leaf
    {
      //添加报告机制
    }
    //最后一位为0 发给host0 否则发给host1
    if(!Pkt_Temp.dest[ADR_NUM-1]){
      out1.write(Pkt_Temp);
    }
    else{
      out2.write(Pkt_Temp);
    }   
  }


    if (in1.event())//received pkt from host0
  {
    Pkt_Temp=in1.read();
    bool leaf_flag=Pkt_temp.isleaf()//判断收到包的目的节点是否位于同一leaf下
    if(leaf_flag){
      //说明同在一leaf下
    if(!Pkt_Temp.dest[ADR_NUM-1]){
      out1.write(Pkt_Temp);
    }
    else{
      out2.write(Pkt_Temp);
    }   
  }else{
    out0.write(Pkt_Temp);//发给core主机
  }

      if (in2.event())//received pkt from host1
  {
    Pkt_Temp=in2.read();
    bool leaf_flag=Pkt_temp.isleaf()//判断收到包的目的节点是否位于同一leaf下
    
    if(leaf_flag){
      //说明同在一leaf下
    if(!Pkt_Temp.dest[ADR_NUM-1]){
      out1.write(Pkt_Temp);
    }
    else{
      out2.write(Pkt_Temp);
    }   
  }else{
    out0.write(Pkt_Temp);//发给core主机
  }
  }


  
}

