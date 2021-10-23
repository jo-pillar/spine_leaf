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

  sender.cpp - This is the implementation file for the synchronous process
               "sender".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <stdlib.h>
#include <time.h>
#include "host.h"
host::host(sc_module_name name) :
sc_module(name)
   {
      SC_METHOD(receiver); 
      dont_initialize();
      sensitive << pkt_in;
      SC_CTHREAD(sender, CLK.pos()); 

    }  
    
void host:: receiver()
{


 pkt temp_val;
  // Ignore the first packet arriving on start-on

  
	temp_val = pkt_in.read();
   // 获取Spine ip
  int spine=0;//若改使用SC_INT类型请务必检查是否输出值是否为-1;
 spine=temp_val.data.range(SPINE_IP_NUM-1,0);
   
   // 获取第一次Leaf ip
  int leaf=0;//若改使用SC_INT类型请务必检查是否输出值是否为-1;
    leaf =temp_val.data.range(SPINE_IP_NUM + LEAF_IP_NUM-1,SPINE_IP_NUM);

  // 获取第二次Leaf ip
    int leaf2=0;//若改使用SC_INT类型请务必检查是否输出值是否为-1;
    leaf2 =temp_val.data.range((SPINE_IP_NUM + LEAF_IP_NUM*2)-1,SPINE_IP_NUM + LEAF_IP_NUM);
  if ((int)temp_val.id!=(int)id.read())
  {
    cout<<"目的iD与接收ID不匹配";
  }


  
	cout << "                                  .........................." << endl;
	cout << "                                  New Packet Received" << endl;
	cout << "                                  Receiver ID: " << (int)id.read()  << endl;
	cout << "                                  Packet Value: " << (int)temp_val.data << endl;
	cout << "                                  Sender ID: " << (int)temp_val.id  << endl;//取消加一
  cout << "                                  Path：Leaf:"<<leaf<<"Spine: "<<spine<<"Leaf2: "<<leaf2<<endl;
	cout << "                                  .........................." << endl;
   

}
void host:: sender()
{
   pkt pkt_data;


   srand((unsigned)time(NULL));
   wait(8);
   while(true)
     {
       /////generate an 8-bit random value for data//////////
       pkt_data.data = rand()%DATA_RANGE;

       ////stamp the sender's id
       pkt_data.id = id.read();

      
       /////生成64的hostid//////////////
       pkt_data.dest= rand()%(2<<ADR_NUM);
       
     

       pkt_out.write(pkt_data);//将pkt写入到输出端口

        cout << ".........................." << endl;
	cout << "New Packet Sent" << endl;
	cout << "Destination Addresses: "<<(int)pkt_data.dest<<endl;

	cout << "Packet Value: " << (int)pkt_data.data << endl;
	cout << "Sender ID: " << (int)id.read()  << endl;
	cout << ".........................." << endl;      
       
       wait(); 

       /////wait for 1 to 3 clock periods/////////////////////
       wait(1+(rand()%3));
     }

}
