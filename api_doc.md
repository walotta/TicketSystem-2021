# API For TacketSystem

* 作者: `walotta`,`Foreset`

* version: `1.0`

  ---

  ### *main.cpp*

  * `by walotta`

  * `main`接收参数，进入`oj`或前端模式
  * 初始化`CMD`类对象
  * 从控制台接受输入，传递给`CMD`类对象
  * `CMD`类执行指令，返回一个由`string`表示的结果
  * 将结果返回到控制台或前端

  ---

  ### *FileStorage*

  * 由一个h和cpp组成

  * 设计为模版类，功能为实现对文件的单或多索引储存，实例化时要求给出存储的类的类型`(T)`、并给出一个`vector`给出所有的`key`的名字，其中以第一个为主值、给出储存文件名

  * 要求存在一个`key`属性为储存值存在且唯一的`KeyName`，为`MainKey`

  * 内部包含多个`BPlusTree`类对象，一个`StoragePool`类的存储类型实例，实现将数据的存入、删除、查询、修改

    | 接口 | 功能 |
    | :--: | :--: |
    |      |      |

  ---

  ### *TrainManage*

  * 由一个h和cpp组成

    |              接口               |       功能       |
    | :-----------------------------: | :--------------: |
    | add_train(i,n,m,s,p,x,t,o,d,y)  | 添加一个新的车次 |
    |        release_train(i)         |     发布车次     |
    |        query_train(i,d)         |     查询车次     |
    |         delete_train(i)         |     删除车次     |
    |   query_ticket(s,t,d,p=time)    |     查询车票     |
    |  query_transfer(s,t,d,p=time)   |     查询换乘     |
    | buy_ticket(u,i,d,n,f,t,q=false) |    为用户买票    |
    |      refund_ticket(u,n=1)       | 用户退订第n张票  |
    |             clean()             |       清除       |

  ---

  ### *Train*

  ---

  ### *UserManage*

  * 由一个h和cpp组成

  * 内部有一个`Log`、`account`类

    |            接口             |               功能               |
    | :-------------------------: | :------------------------------: |
    |         login(u,p)          |             用户登陆             |
    |          logout(u)          |             用户登出             |
    |       query_order(u)        |      查询用户的所有订单信息      |
    |  change_log(u,n=1,action)   |        修改订单信息并返回        |
    |  buy_ticket(u,i,d,f,t,n,q)  |            买/预定票             |
    |           clean()           |               清除               |
    |      ask_user_alive(u)      |       查询用户是否为登陆态       |
    |    add_user(c,u,p,n,m,g)    |            添加新用户            |
    |     query_profile(c,u)      |           查询用户信息           |
    | modify_profile(c,u,p,n,m,g) | 更新用户信息并返回修改用户的信息 |

  

