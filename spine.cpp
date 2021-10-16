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


void spine ::entry()
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
  if (in0.event()) // received pkt from core
  {
    Pkt_Temp = in0.read();
    bool Spine_flag = false;
    
    for (int i = 0; i < ADR_NUM - 3; i++)
    {
      Spine_flag = Spine_flag && (Pkt_Temp.dest[i] == Spineid[i]);
    }
    if (!Spine_flag)
    {
      out0.write(Pkt_Temp);
    } //前4位不同说明不是目的节点 继续发往总线
    if (Spine_flag)
    {
      if (!Pkt_Temp.dest[ADR_NUM - 2])
      {
        out1.write(Pkt_Temp); //发给leaf0
      }

      else
      {
        out2.write(Pkt_Temp);
      }
    }
  }

  if (in1.event()) // received pkt from leaf1
  {
    Pkt_Temp = in0.read();
    bool Spine_flag = false;
    for (int i = 0; i < ADR_NUM - 3; i++)
    {
      Spine_flag = Spine_flag && (Pkt_Temp.dest[i] == Spineid[i]);
    }
    if (!Spine_flag)
    {
      out0.write(Pkt_Temp);
    } //前4位不同说明不是目的节点 继续发往总线
    if (Spine_flag)
    {
      if (!Pkt_Temp.dest[ADR_NUM - 2])
      {
        out1.write(Pkt_Temp); //发给leaf0
      }

      else
      {
        out2.write(Pkt_Temp);
      }
    }
  }

  if (in2.event()) // received pkt from host1
  {
    Pkt_Temp = in0.read();
    bool Spine_flag = false;
    for (int i = 0; i < ADR_NUM - 3; i++)
    {
      Spine_flag = Spine_flag && (Pkt_Temp.dest[i] == Spineid[i]);
    }
    if (!Spine_flag)
    {
      out0.write(Pkt_Temp);
    } //前五位不同说明不是目的节点 继续发往总线
    //倒数第二位id
    if (Spine_flag)
    {
      if (!Pkt_Temp.dest[ADR_NUM - 2])
      {
        out1.write(Pkt_Temp); //发给leaf0
      }

      else
      {
        out2.write(Pkt_Temp);
      }
    }
  }
}
