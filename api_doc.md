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
  
### *StoragePool*
  
* 为储存池类，进行底层对于书的信息存储文件的操作，实现`O(1)`的读取删除，并维护储存空间的回收
  
* 提供一个额外的一个块的零碎文件存储
  
* 设计为模版类，实例化时要求为其指明存储的文件名`(FILENAME)`与储存的块的类型`(T)`
  
* 对外接口类似于虚拟内存，提供一个数字寻址的功能，对于给定的`id`值返回其对应的储存块
  
* 内部有一个储存块指针，向文件中写入储存块，用`id*size`进行寻址
  
  * 在储存池头部有一个`head`数据点，用于储存下一个可用块的`id`，若为-1，说明应当在文件末写入
  
* 删除块时，将被删除的块的`id`写入`head`数据点，在被删除块的位置写入原`head`值
  
* 写入块时，先查询`head`数据点，若为覆写，则将`head`的值修改
  
  |             接口              |                       功能                        |
    | :---------------------------: | :-----------------------------------------------: |
  |       `add(const T &)`        | 将`T`对应的数据写入文件，并返回储存位置对应的`id` |
    |         `remove(id)`          |                 删除`id`对应的块                  |
    |        `update(id,T)`         |                  更新`id`对应块                   |
    |           `get(id)`           |               返回`id`对应的块对象                |
    |      `readExtraBlock()`       |                 读入存储的额外块                  |
    | `writeExtraBlock(extraBlock)` |                 将额外块写入文件                  |
  
  ---
  
  ### *StorageManger*
  
  * 由一个h组成

  * 设计为模版类，功能为实现对文件的单或双索引储存，实例化时要求给出存储的类的类型`(T)`、给出储存文件名、给出`bool`告知是否含有`Tag`

  * 要求存在一个`key`属性为储存值存在且唯一的`KeyName`，为`MainKey`

  * 内部包含多个`BPlusTree`类对象，一个`StoragePool`类的存储类型实例，实现将数据的存入、删除、查询、修改

    |                 接口                 |                    功能                     |
  | :----------------------------------: | :-----------------------------------------: |
    |         `Insert(MainKey,T)`          |         插入一个数据，并维护主索引          |
  |          `Remove(MainKey)`           |      根据主值删除，仅维护主值的搜索树       |
    |       `RemoveTag(MainKey,Tag)`       |    根据给出的`MainKey`删除对应数据`Tag`     |
  |           `FindByTag(Tag)`           | 根据提供的`Tag`寻找，返回一个对象的`vector` |
    |         `FindByKey(MainKey)`         |           根据`MianKey`v返回对象            |
    |         `Update(MainKey,T)`          |            更新`MianKey`对于的块            |
    | `AddTag(MainKey,vector<string>list)` |     为`MianKey`对应的对象增加`Tag`属性      |
  
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
  
  

