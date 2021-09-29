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

  pkt.h - This file defines a SystemC structure "pkt".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#ifndef PKT_H_INCLUDED
#define PKT_H_INCLUDED

#include "systemc.h"

struct pkt {
       sc_int<8> data;
       sc_int<4> id;//发送者id [2][1][1] [core_id][leaf_id][host_id]
       bool dest[4];

       inline bool operator == (const pkt& rhs) const
   {
     return (rhs.data == data && rhs.id == id &&rhs.dest==dest);
   }

};

inline
ostream&
operator << ( ostream& os, const pkt& /* a */ )
{
    os << "streaming of struct pkt not implemented";
    return os;
}

inline
void
#if defined(SC_API_VERSION_STRING)
    sc_trace( sc_trace_file* tf, const pkt& a, const std::string& name )
#else
    sc_trace( sc_trace_file* tf, const pkt& a, const sc_string& name )
#endif
{
  sc_trace( tf, a.data, name + ".data" );
  sc_trace( tf, a.id, name + ".id" );
    sc_trace( tf, a.dest, name + ".dest0" );
  for (int i = 0; i < 4; i++)
  {
    std::string str =std:: to_string(i);
     sc_trace( tf, a.dest[i], name + ".dest"+str );
  }
  
}
#endif // PKT_H_INCLUDED
