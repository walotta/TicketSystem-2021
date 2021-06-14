# 后端逻辑设计

------

## 数据存储方式

### 文件

- `5`个`database`：`user_data.dat`, `log_data.dat`, `train_data.dat`, `seat_data.dat`, `order_data.dat`
- `6`个`Binary-Plus-Tree`：用户索引一个，购票记录索引一个，列车索引两个，座位索引一个，候票记录索引一个。

### 各元素成员

#### 用户

```
key: user_id
value: user_name, password, mail_address, privilege, ordered_number
```

#### 购票记录

```
key: user_id & it's order_number
value: train_id, departure_station, arrival_station, departure_time, arrival_time, cost, number, status
```

#### 列车

```
key: train_id
second_key: station_name(s)
value: station_number, seat_number, sale_begin_date, sale_end_date, If_release, train_type, station_names & times
```

#### 座位

```
key: train_id & sale_date
value: seats_of_each_satation 
```

#### 候票记录

```
key: serial_number
value: user_id, order_number, train_id, sale_date, departure_station, arrival_station
```

------

## 对外接口以及允许的操作

### 用户操作

- `add_user`：新建用户，将新的用户信息存入系统
- `login/logout`：登入/退出，将一个用户登入系统或是退出系统
- `query_profile`：查询用户信息，并给出相关信息
- `modify_profile`：更改信息，更改一个权限相对低的用户的信息，并给出相关信息
- `query_order`：查询订单，查询一个用户的所有已购/以退订单的信息

### 列车操作

- `add_train`：新建列车，将新的火车信息存入系统
- `release_train`：公开列车，将新的列车开放售票，开放售票的列车无法被删除
- `delete_train`：删除列车，删除一列火车的信息
- `query_train`：查询列车，询一列车的某一天的车票与车站信息

### 车票操作

- `query_ticket`：查询车票，查询某一天出发的某两站之间的列车信息，输出信息按照要求的花费排序
- `query_transfer`：查询换乘，查询某一天出发的某两站之间的仅换乘一次的列车信息，输出花费最小的车次信息
- `buy_ticket`：购买车票，购买某一天的某列车的某两站之间的车票，如果失败则根据用户意愿进入候补队列
- `refund_ticket`：退回车票，退回某个用户的某一个订单，并且检查是否有新的候补队列的用户订单能够成功购票