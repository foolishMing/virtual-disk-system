# Virtual-Disk-System
- [Virtual-Disk-System](#virtual-disk-system)
  - [工作进度](#工作进度)
  - [概述](#概述)
  - [文档](#文档)
    - [需求分析](#需求分析)
    - [逻辑方案](#逻辑方案)
  - [程序](#程序)
    - [开发环境](#开发环境)
    - [第三方工具](#第三方工具)
      - [Catch2](#catch2)
        - [下载地址](#下载地址)
        - [VS项目配置](#vs项目配置)
      - [CrashRpt](#crashrpt)
      - [spdLog](#spdlog)
        - [下载地址](#下载地址-1)
        - [VS项目配置](#vs项目配置-1)
  - [参考资料](#参考资料)
## 工作进度

| 日期       | 事项                                                         | 项目版本 |
| ---------- | ------------------------------------------------------------ | -------- |
| 2021-06-25 | [doc] 创建需求分析文档                                       | v0.0     |
| 2021-06-28 | [doc] 完成需求分析文档                                       | v0.0     |
| 2021-06-29 | [doc] 逻辑方案完成了类图的框架设计;<br />[tools] 引入单元测试框架Catch2 | v0.0     |
| 2021-07-02 | [doc] 完成了类的详细设计及代码逻辑框架                       | v0.1     |
| 2021-07-05 | [tools]引入日志框架spdlog<br />[update]完成了对用户输入的解析 | v0.2     |
| 2021-07-08 | [update]初步完成了dir、cd、md、ren、cls命令                  | v0.3     |

## 概述

VirtualDisk是一款基于C++语言，运行在命令行模式下的虚拟磁盘软件。

虚拟磁盘软件是指在内存中模拟磁盘，通过接收命令，在内存中完成一些文件操作的功能。

## 文档

### 需求分析

[虚拟磁盘需求分析(markdown)](./document/需求分析/markdown/虚拟磁盘需求分析.md)

[虚拟磁盘需求分析(html)](./document/需求分析/html/虚拟磁盘需求分析.html)

[虚拟磁盘需求分析(pdf)](./document/需求分析/pdf/虚拟磁盘需求分析.pdf)

### 逻辑方案

[虚拟磁盘逻辑方案(markdown)](./document/逻辑方案/markdown/虚拟磁盘逻辑方案.md)

[虚拟磁盘逻辑方案(html)](./document/逻辑方案/html/虚拟磁盘逻辑方案.html)

[虚拟磁盘逻辑方案(pdf)](./document/逻辑方案/pdf/虚拟磁盘逻辑方案.pdf)

## 程序

### 开发环境

Visual Studio2017 

C++17

### 第三方工具

#### Catch2

##### 下载地址

https://github.com/catchorg/Catch2

##### VS项目配置

#### CrashRpt

#### spdLog

##### 下载地址

https://github.com/gabime/spdlog

##### VS项目配置

+ 添加静态库文件

![image-20210705090403131](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210705090403131.png)

+ 添加头文件包含路径

![image-20210705090504251](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210705090504251.png)



## 参考资料

[虚拟磁盘需求说明文档](https://wiki.h3d.com.cn/pages/viewpage.action?pageId=328330)

[虚拟磁盘测试用例](https://wiki.h3d.com.cn/pages/viewpage.action?pageId=328169)

[虚拟磁盘测试程序使用手册](https://wiki.h3d.com.cn/pages/viewpage.action?pageId=328117 )

[逻辑方案模板](https://wiki.h3d.com.cn/pages/viewpage.action?pageId=34111506 )

