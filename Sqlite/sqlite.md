# Sqlile

## 介绍 

### 特性

1. 零配置
2. 可移植
3. 紧凑
4. 简单
5. 灵活
6. 自由的授权
7. 可靠
8. 易用

### 数据类型

一般数据采用固定的静态数据类型，而SQLite采用的是鼎泰数据类型，会根据存入值自定判断
- SQLite具有以下5中基本数据类型
    
    1. integer： 带符号的整型（最多64位）
    2. real： 8字节表示的浮点型
    3. text： 字符类型，支持多种编码（如UTF-8，UTF-16），无大小限制
    4. blob：任意类型的数据，无大小限制。BLOB（binary large object）二进制对象，使用二进制保存数据
    5. null 表示空

## 使用

>创建打开sqlite数据库

```shell
sqlite *.db 
# 当*.db不存在时，则创建并打开数据库文件
# 当*.db存在时，则打开数据库文件 
```
>退出数据库

```shell
.quit 或 .exit
```

>sql的语句格式
```
所有的SQL语句都是以分号结尾的，SQL语句不区分大小写。两个减号“--”代表注释
关系型数据库的核心操作
    1. 创建，修改，删除表
    2. 添加，修改，删除行
    3. 查表
```

>创建表: create语句
```sql
create table <tablename>(<字段> <类型>,<字段> <类型>...);
#example： 
create table tab(id integer,name text);

# 查看当前数据库的表
.table
# 查看当前数据库的表的创建语句
.schema
```
![查看表](./Sqlite.assets/sqlitelooktab.png)

> 设置主键

在用sqlite设计表时，每个表都可以通过primary key手动设置主键，每个表只能有一个主键，设置为主键的列数据不能重复。
```sql
create table <tablename>(<字段> <类型> primary key,<字段> <类型>...);
#example： 
create table tab(id integer primary key,name text);
```

> 修改表：alter 语句

在已有的表中添加或删除字段以及修改表名，无法修改字段属性
```sql
#修改表名
alter table <tablename> rename to <newname>;
#example
alter table stu2 rename to stu;
```
```sql
#添加字段
alter table <tablename> add <字段> <属性>;
#example 该例添加了字段 grade 属性为 integer
alter table stu add grade integer;
```

>删除表 drop语句

用于删除表（表的结构，属性，以及表的索引也会被删除）
```sql
#删除表
drop table <表名>;
#example 删除表
drop table nstu;
```

> 插入新行 insert into语句（全部赋值）

给一行中所有列赋值
```sql
# 插入新行 当列值为字符串时，要加上单引号 ''
insert into <表名> values (<列值1>,<列值2>,<列值3>,<列值4>,...)
# example 
insert into tbl values(100,'朱芷杰' ,'ahhs');
```



