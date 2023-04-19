结构：

    databases (数据库)
        tables (表)

## 四种操作

## DDL-数据定义语言
### 数据库操作
```cpp
show databases;  //查看所有数据库

create databases 数据库名; //创建数据库

use 数据库名; //使用某个数据库

select database(); //查看当前在哪个数据库

drop database 数据库名; //删除数据库
```

### 表操作
```cpp
show tables; //展示所有表

create table 表名 (字段 字段类型, 字段 字段类型, ...); //创建表

desc 表名; //查看这个表的信息

show create table 表名; //查看这个表的创建信息

alter table 表名 add/modify/change/drop/rename to ...; //添加字段/更新字段类型/更新字段和字段类型/删除字段/改表名

drop table 表名; //删除表
```

## DML-数据操纵语言
### 添加数据
```cpp
insert into 表名 (字段1, 字段2...) values (值1, 值2...); //给指定字段添加数据

insert into 表名 values (值1, 值2...);  //给全部字段添加数据

insert into 表名 (字段1, 字段2...) values (值1, 值2...), (值1, 值2...), (值1, 值2...);
insert into 表名 values (值1, 值2...), (值1, 值2...), (值1, 值2...); //批量添加数据
```

### 修改数据
```cpp
updata 表名 set 字段名1 = 值1, 字段名2 = 值2, ... [where 条件];
```

### 删除数据
```cpp
delete from 表名 [where 条件];
//delete不能删除某一个字段的值(可以使用update)
```

## DQL-数据查询语言
### 执行顺序
select ④

    字段列表
from ①

    表名列表
where ②

    条件列表
group by ③

    分组字段列表
having

    分组后条件列表
order by ⑤

    排序字段列表
limit ⑥

    分页参数

## DCL-数据控制语言
### 管理用户
1、查询用户

    use mysql;
    select * from user;
2、创建用户
    
    create user '用户名'@'主机名' identified '密码';
3、修改用户密码

    alter user '用户名'@'主机名' identified with mysql_native_password by '新密码';
4、删除用户

    drop user '用户名'@'主机名';

### 控制权限

|权限|说明|
|:-:|:-:|
|ALL, ALL PRIVILEGES|所有权限|
|SELECT|查询数据|
|INSERT|插入数据|
|UPDATE|修改数据|
|DELETE|删除数据|
|ALTER|修改表|
|DROP|删除数据库/表/视图|
|CREATE|创建数据库/表|

1、权限查询
    
    show grants for '用户名'@'主机名';
2、授予权限

    grant 权限列表 on 数据库.表名 to '用户名'@'主机名';
3、撤销权限

    revoke 权限列表 on 数据库.表名 from '用户名'@'主机名';


## 函数
### 字符串函数

|函数|功能|
|:-:|:-:|
|concat( $s_1 $, $s_2$,..., $s_n$)|字符串拼接，将$s_1$, $s_2$,..., $s_n$拼接成一个字符串|
|lower(str)|将字符串str全部转为小写|
|upper(str)|将字符串str全部转为大写|
|lpad(str, n, pad)|左填充，用字符串pad最str的左边进行填充，达到n个字符串长度|
|rpad(str, n, pad)|右填充，用字符串pad最str的右边进行填充，达到n个字符串长度|
|trim(str)|去掉字符串头部和尾部的空格|
|substring(str, start, len)|返回字符串str从start位置起的len个长度的字符串|

### 数值函数

|函数|功能|
|:-:|:-:|
|ceil(x)|向上取整|
|floor(x)|向下取整|
|mod(x, y)|返回x / y的模|
|rand()|返回0 - 1内的随机数|
|round(x, y)|求参数x的四舍五入的值，保留y位小数|

### 日期函数

|函数|功能|
|:-:|:-:|
|curdate()|返回当前日期|
|curtime()|返回当前时间|
|now()|返回当前日期和时间|
|year(date)|获取指定date的年份|
|month(date)|获取指定date的月份|
|day(date)|获取指定date的日期|
|date_add(date, INTERVAL expr type)|返回一个日期/时间加上一个时间间隔expr后的时间值|
|datediff(date1, date2)|返回起始时间date1和结束时间date2间的天数|

### 流程函数

|函数|功能|
|:-:|:-:|
|if(val, t, f)|如果value位true，则返回t，否则返回f|
|ifnull(value1, value2)|如果value1不为空，返回value1，否则返回value2|
|case when [val1] then [res1] ...else [default] end|如果val1为true，返回res1；...否则返回default默认值|
|case [expr] when [val1] then [res1] ... else [default] end|如果expr的值等于val1，返回res1；...，否则返回defaul默认值|



## 约束
### 概述
1、概念：约束时作用于表中字段上的规则，用于限制存储在表中的数据。
2、目的：保证数据库中数据的正确、有效性和完整性
3、分类：
|约束|描述|关键字|
|:-:|:-:|:-:|
|非空约束|限制该字段的数据不能为null|NOT NULL|
|唯一约束|保证该字段的所有数据都是唯一、不重复的|UNIQUE|
|主键约束|主键时一行数据的唯一标识，要求非空且唯一|PRIMARY KEY|
|默认约束|保存数据时，如果未指定该字段的值，则采用默认值|DEFAULT|
|检查约束|保证字段满足某一个条件|CHECK|
|外键约束|用来让两张表的数据之间建立连接，保证数据的一致性和完整性|FOREIGN KEY|

### 外键约束

添加外键

    Alter Table 表名 Add Constraint 外键名称 Foreign Key (外键字段名) References 主表(主表列名);
删除外键

    Alter Table 表名 drop foreign ket 外键名称;

删除/更新行为

|行为|说明|
|:-:|:-:|
|NO ACTION|当附表中删除/更新对应记录时，首先检查该记录是否有对应外键，如果有则不允许删除/更新|
|RESTRICT|当附表中删除/更新对应记录时，首先检查该记录是否有对应外键，如果有则不允许删除/更新|
|CASCADE|当附表中删除/更新对应记录时，首先检查该记录是否有对应外键，如果有，则也删除/更新外键在子表中的记录|
|SET NULL|当父表中删除对应记录时，首先检查该记录是否有对应外键，如果有则设置子表中该外键值为null|
|SET DEFAULT|父表有变更时，子表将外键列设置成一个默认的值|



