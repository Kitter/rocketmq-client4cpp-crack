Fork from [RocketMQ](https://github.com/NDPMediaCorp/RocketMQ-Client4CPP) C++ Client
===================

### 主要工作
* 发现之前项目中会存在内存泄露的问题，通过valgrind进行验证.
* 由于不需要项目ssl，因此去掉了相关代码
* 修改了PullConsumer的例子
* 修改了日志目录以及日志打印级别 

### 主要依赖

* cmake 3.2+
* g++ 4.8+ (需要支持c++11）
* 需要libthread库，zip库等


### 安装方法
<code>
  cmake .
  make && make install
</code>
