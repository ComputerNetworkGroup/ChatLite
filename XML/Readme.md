# XMLLog 文档

# Requirement

- TinyXML-2v

# Structure

## 1. logBuf
存储Login日志类中进行通讯的信息
| 变量      | 作用                      |
|-----------------|---------------------------|
| ip[32]        | 指向XLLogin的指针         |
| username[32] | 指向xlDataTransform的指针 |
| type[16]        | 指向XLLogin的指针         |
| time[16] | 指向xlDataTransform的指针 |

# CLass

## 1. XmlLog 

### 1.1 Members

| 类成员变量      | 作用                      |
|-----------------|---------------------------|
| *xlLogin        | 指向XLLogin的指针         |
| xlDataTransform | 指向xlDataTransform的指针 |
| timePresent     | 时间戳                    |
| timeBuf[32]     | 输出时间缓存字符串        |
| mapType         | Type到字符串的映射字典    |

### 1.2 Functions

| 类成员函数          | 作用                |
|---------------------|---------------------|
| _writeLogin         | 写Login日志         |
| _writeDataTransform | 写数据传输日志      |
| writeNorm           | 写正常日志(顶层API) |
| writeError          | 写异常日志(顶层API) |
| saveLog             | 保存日志            |
| initMap             | 初始化映射字典      |

## 2. XmlLogIn

### 2.1 Members

| 类成员变量      | 作用                      |
|-----------------|---------------------------|
| *doc        | 登陆日志指针         |
| *xXLLogin        | &lt;login&gt;的XML标签 |

### 2.2 Functions

| 类成员函数          | 作用                |
|---------------------|---------------------|
| setLogin         | 设置&lt;login&gt;的XML标签 |
| setItem | 添加一个日志表项      |


## 3. XmlDataTransform

### 3.1 Members

| 类成员变量      | 作用                      |
|-----------------|---------------------------|
| *doc        | 数据传送日志指针         |
| *xXLLogin        | &lt;dataTransform&gt;的XML标签|

### 3.2 Functions

| 类成员函数          | 作用                |
|---------------------|---------------------|
| setDataTransform         | 设置&lt;dataTransform&gt;的XML标签         |
| setItem | 添加一个日志表项      |