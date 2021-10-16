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
#define ADR_NUM 7
#define SPINE_NUM 16
#define HOST_NUM 64
#define LEAF_NUM 32
#define SIM_NUM 500
#include "systemc.h"
#include<vector>
using std::vector;

struct pkt {
       sc_int<8> data;
       sc_int<7> id;//发送者id [0-3][4-5][5-6] [spine][leaf][host]
       sc_int<7> dest;
       bool ishost(sc_int<ADR_NUM> hostid);//判断hostid 是不是目的host
       bool isleaf(sc_int<ADR_NUM-2> leafid);//判断leafid 是否匹配目的leaf
       bool isspine(int int_spineid);//判断spineid是否匹配spine
       int get_dest_spine();//将dest的id部分转换成int

       inline bool operator == (const pkt& rhs) const
   {
     return (rhs.data == data && rhs.id == id &&rhs.dest==dest);
   }

};
bool pkt::ishost(sc_int<ADR_NUM> hostid){
  return(hostid==dest);
  

}
bool pkt:: isleaf(sc_int<ADR_NUM-2> leafid){
  bool leafflag=true;
  for (int i = 0; i < ADR_NUM-2; i++)
  {
    leafflag=leafflag&&(leafid[i]==dest[i]);
  }
  return leafflag;
  

}
bool pkt::isspine(int int_spineid){
    bool spineflag=true;
    sc_int<ADR_NUM-3> spine_id=int_spineid;
  for (int i = 0; i < ADR_NUM-3; i++)
  {
    spineflag=spineflag&&(spine_id[i]==dest[i]);
  }
  return spineflag;

}
int pkt::get_dest_spine(){
int d=0;
for (int i = 0; i < ADR_NUM-3; i++)
{
  if (dest[i])
  {
    d=1+(d<<1);
  }
}
return d;
}

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
    sc_trace( tf, a.dest, name + ".dest" );
  for (int i = 0; i < ADR_NUM; i++)
  {
    std::string str =std:: to_string(i);
     sc_trace( tf, a.dest[i], name + ".dest"+str );
  }
  
}
#endif // PKT_H_INCLUDED
