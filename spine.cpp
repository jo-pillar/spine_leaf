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
#define SIM_NUM 500

void spine :: entry()
{
  wait();

  // declarations
  switch_reg R0;
  switch_reg R1;
  switch_reg R2;

  switch_reg temp;

  int sim_count;
  int pkt_count;
  int drop_count;

  fifo q0_in;
  fifo q1_in;
  fifo q2_in;


  fifo q0_out;
  fifo q1_out;
  fifo q2_out;


  // FILE *result;

  // initialization
  pkt_count = 0;
  drop_count = 0;
  sim_count  = 0;

  q0_in.pntr = 0;
  q1_in.pntr = 0;
  q2_in.pntr = 0;


  q0_out.pntr = 0;
  q1_out.pntr = 0;
  q2_out.pntr = 0;


  q0_in.full  = false;
  q1_in.full  = false;
  q2_in.full  = false;


  q0_in.empty = true;
  q1_in.empty = true;
  q2_in.empty = true;


  q0_out.full = false;
  q1_out.full = false;
  q2_out.full = false;


  q0_out.empty = true;
  q1_out.empty = true;
  q2_out.empty = true;

  
  R0.free = true;
  R1.free = true;
  R2.free = true;
  bool leafflag;
  bool hostflag;


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
  while( sim_count++ < SIM_NUM )
    { 
       wait();

       /////read input packets//////////////////////////////////     
      if (in0.event()) 
       {
	 pkt_count++;
	 if (q0_in.full == true) drop_count++;
         else q0_in.pkt_in(in0.read());
       };  

       if (in1.event()) 
       {
	 pkt_count++;
	 if (q1_in.full == true) drop_count++;
         else q1_in.pkt_in(in1.read());
       };

       if (in2.event()) 
       {
	 pkt_count++;
	 if (q2_in.full == true) drop_count++;
         else q2_in.pkt_in(in2.read());
       };

       

      /////move the packets from fifo to shift register ring/////
   
      if((!q0_in.empty) && R0.free) 
	{
          R0.val  = q0_in.pkt_out();
	  R0.free = false;
	}

      if((!q1_in.empty) && R1.free) 
	{
          R1.val  = q1_in.pkt_out();
	  R1.free = false;
	}
      if((!q2_in.empty) && R2.free) 
	{
          R2.val  = q2_in.pkt_out();
	  R2.free = false;
	}
 

            /////shift the channel registers /////////////////////////
            temp = R0;
            R0 = R1;
	    R1 = R2;
	    R2 = temp;


	    /////write the register values to output fifos////////////
        //R0
        leafflag=(leafid[0]==R0.val.id[0])&&(leafid[1]==R0.val.id[1])&&(leafid[2]==R0.val.id[2]);
        hostflag=R0.val.id[3];

	    if ((!R0.free) && (!leafflag) && (!q0_out.full))// 当r0有包且是发送到第0个端口且q0栈不满。解决一个逻辑 什么时候写道R0端口
	      {
		q0_out.pkt_in(R0.val); 
		R0.free = true;//这句代码看不懂
	      }

	    if ((!R0.free) && (!leafflag) &&(!hostflag)&& (!q1_out.full))
	      {
		q1_out.pkt_in(R0.val);
		R0.free = true;
	      }
	    if ((!R0.free) && (!leafflag) &&(!hostflag) && (!q2_out.full))
	      {
		q2_out.pkt_in(R0.val);
        R0.free = true;
	      }
        	    if (!q0_out.empty) out0.write(q0_out.pkt_out()); 
	    if (!q1_out.empty) out1.write(q1_out.pkt_out());
	    if (!q2_out.empty) out2.write(q2_out.pkt_out());
//R1
        leafflag=(leafid[0]==R1.val.id[0])&&(leafid[1]==R1.val.id[1])&&(leafid[2]==R1.val.id[2])
        hostflag=R1.val.id[3];

	    if ((!R1.free) && (!leafflag) && (!q0_out.full))// 当R1有包且是发送到第0个端口且q0栈不满。解决一个逻辑 什么时候写道R1端口
	      {
		q0_out.pkt_in(R1.val); 
		R1.free = true;//这句代码看不懂
	      }

	    if ((!R1.free) && (!leafflag) &&(!hostflag)&& (!q1_out.full))
	      {
		q1_out.pkt_in(R1.val);
		R1.free = true;
	      }
	    if ((!R1.free) && (!leafflag) &&(!hostflag) && (!q2_out.full))
	      {
		q2_out.pkt_in(R1.val);
        R1.free = true;
	      }
        	    if (!q0_out.empty) out0.write(q0_out.pkt_out()); 
	    if (!q1_out.empty) out1.write(q1_out.pkt_out());
	    if (!q2_out.empty) out2.write(q2_out.pkt_out());
//R2
	     leafflag=(leafid[0]==R2.val.id[0])&&(leafid[1]==R2.val.id[1])&&(leafid[2]==R2.val.id[2])
        hostflag=R2.val.id[3];

	    if ((!R2.free) && (!leafflag) && (!q0_out.full))// 当R2有包且是发送到第0个端口且q0栈不满。解决一个逻辑 什么时候写道R2端口
	      {
		q0_out.pkt_in(R2.val); 
		R2.free = true;//这句代码看不懂
	      }

	    if ((!R2.free) && (!leafflag) &&(!hostflag)&& (!q1_out.full))
	      {
		q1_out.pkt_in(R2.val);
		R2.free = true;
	      }
	    if ((!R2.free) && (!leafflag) &&(!hostflag) && (!q2_out.full))
	      {
		q2_out.pkt_in(R2.val);
        R2.free = true;
	      }
      

	    /////write the packets out//////////////////////////////////    
	    if (!q0_out.empty) out0.write(q0_out.pkt_out()); 
	    if (!q1_out.empty) out1.write(q1_out.pkt_out());
	    if (!q2_out.empty) out2.write(q2_out.pkt_out());
    } 

  sc_stop();

  cout << endl << endl << "-------------------------------------------------------------------------------" << endl;
  cout << "End of switch operation..." << endl;
  cout << "Total number of packets received: " <<  pkt_count << endl;
  cout << "Total number of packets dropped: " <<  drop_count << endl;
  cout << "Percentage packets dropped:  " <<  drop_count*100/pkt_count << endl;
  cout << "-------------------------------------------------------------------------------" << endl;
  
}