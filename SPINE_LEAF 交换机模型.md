## 1 主要功能
通过修改宏参数能够控制搭建任意数量的leaf spine host 组成的网络模型，并输出传输路径
## 2 宏参数介绍
 SPINE_NUM 4
 HOST_NUM 64
 LEAF_NUM 16// 以上数量要能够整除
 SIM_NUM 500// 仿真器数量
 ADR_NUM 5// 地址位数 等于log2(HOST_NUM)
 SPINE_IP_NUM  2// SPINE_IP 地址位数
 LEAF_IP_NUM 4// LEAF_IP 地址位数
 DATA_NUM 17//数据包数据部分位数 注意DATA_NUM+(SPINE_IP_NUM+2*LEAF_IP_NUM)不能大于64位，由SC_INT 类型所限制
 DATA_RANGE 32 //等于2^(DATA_NUM-(SPINE_IP_NUM+LEAF_IP_NUM)) 
## 2 项目模块介绍
主要有数据包PKT模块，主机HOST模块，叶交换机leaf模块，脊交换机SPINE模块，下面进行详细介绍
### 2.1PKT模块
由发送IP，接收IP，以及数据部分组成，为记录路径信息，当从一个脊交换机获叶交换机发出时，交换机会按照约定格式将自身IP注入到数据部分。当接收端收到包之后，通过解码数据包即可获取路径信息及原始数据部分。
数据组成
```
(0--SPINE_IP_NUM-1)(SPINE_IP_NUM--SPINE_IP_NUM+LEAF_IP_NUM-1)(SPINE_IP_NUM+LEAF_IP_NUM--SPINE_IP_NUM+2*LEAF_IP_NUM-1)(SPINE_IP_NUM+2*LEAF_IP_NUM--end)
(spine_ip)(第一次leafip)(第二次leafip)(原始数据部分)
```

### 2.2 主机模块
主机要负责数据的接收和发送，并同时有自己的IP。
成员介绍：
```
pkt_in  输入端
pkt_out 输出端
id;  本机ID  在组网时分配
CLK 时钟
方法
receiver（）：  
接收数据包并打印数据信息和路径信息  
sender():  
随机生成id和数据部分，并打印发送信息  
```
### 3.3 叶交换机模块
叶交换机负责SPINE host 之间的转发

成员介绍：
```  
spine_port_num; 脊交换机侧端口数量  
host_port_num;叶交换机端口数量  
leaf_ip; 本机ip  
spine_port_in;脊交换机输入端口  
spine_port_out;脊交换机输出端口  
host_port_in;主机输入端口  
host_port_out;主机输出端口  
switch_cntrl;控制信号  
```
具体路由算法如下  
HOST————>SPINE:  
读取host 端口数据 并将其循环发送给（0--SPINE_NUM)编号对应的Spine 同时第一次注入leafIP  
SPINE————>host:  
读取spine端口数据包的目的IP信息 目的主机所在端口编号dest_host=dest % (HOST_NUM / LEAF_NUM);同时第二次注入leafIP   
### 3.4 脊交换机模块
成员介绍：  
leaf_port_num; leaf端口数量  
spine_ip; 本机IP  
leaf_port_in; leaf输入端口  
leaf_port_out; leaf 输出端口  
switch_cntrl;  

具体路由算法：  
读取leaf端口数据包目的ip信息 目的主机所在leaf端口编号 dest_leaf=dest/(leaf_NUM/LEAF_NUM)  
## 4 辅助模块介绍
### 4.1 时钟
switch clock 模块完成clock到switch_cntrl之间的转换
### 4.2 fifo 队列
成员信息:  
deque <pkt> regs;//deque容器包装的PKT 实现了无限深的先入先出队列   
bool full;  
bool empty;  
## 5 main函数
完成了拓扑搭建   
SC_SIGNAL 信号类型负责连接端口，其中  
host_signal_in,host_signal_out;//host--leaf 信号 in out 相对host 而言  
spine_signal_in,spine_signal_out;//spine--leaf 信号 in out 相对spine 而言  








