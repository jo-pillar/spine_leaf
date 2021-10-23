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
#define SPINE_NUM 4
#define HOST_NUM 64
#define LEAF_NUM 16
#define SIM_NUM 500
#define ADR_NUM 5// 地址位数等于log2(HOST_NUM)
#define SPINE_IP_NUM  2// SPINE_IP 地址位数
#define LEAF_IP_NUM 4// LEAF_IP 地址位数
#define DATA_NUM 17//数据包位数 DATA_NUM+(SPINE_IP_NUM+LEAF_IP_NUM)不能大于64位
#define DATA_RANGE 32 //等于2^(DATA_NUM-(SPINE_IP_NUM+LEAF_IP_NUM)) 
#include "systemc.h"
#include<vector>
using std::vector;

struct pkt {
       sc_int<DATA_NUM> data;
       sc_int<ADR_NUM> id;//发送者id [0-3][4-5][5-6] [spine][leaf][host]
       sc_int<ADR_NUM> dest;   //目标id [0-3][4-5][5-6] [spine][leaf][host]
    
      
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
    sc_trace( tf, a.dest, name + ".dest" );
  for (int i = 0; i < ADR_NUM; i++)
  {
    std::string str =std:: to_string(i);
     sc_trace( tf, a.dest[i], name + ".dest"+str );
  }
  
}
#endif // PKT_H_INCLUDED
