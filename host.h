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

  host.h - This file defines a SystemC structure "host".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#ifndef HOST_H_INCLUDED
#define HOST_H_INCLUDED

#include "systemc.h"
#include "pkt.h"
struct host: sc_module {
  sc_in<pkt> pkt_in;  
  sc_in<sc_int<4> > id;  //本机ID等于sourceid
  int first;

  sc_out<pkt> pkt_out; 
  sc_in_clk CLK;

  SC_CTOR(host) {
      SC_METHOD(receiver); 
      dont_initialize();
      sensitive << pkt_in;
      first = 1;
      SC_CTHREAD(sender, CLK.pos()); 

    }  
 void receiver();
 void sender();
};






#endif // HOST_H_INCLUDED
