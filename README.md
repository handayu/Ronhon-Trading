# Ronhon-Trading
# 关于融航MOM接口的测试用例-C++二次开发

[![AppVeyor build status](https://ci.appveyor.com/api/projects/status/github/softethervpn/softethervpn?branch=master&svg=true)](https://ci.appveyor.com/project/softethervpn/softethervpn) [![Travis CI build status](https://travis-ci.org/SoftEtherVPN/SoftEtherVPN.svg?branch=master)](https://travis-ci.org/SoftEtherVPN/SoftEtherVPN) [![gitlab-ci build status](https://gitlab.com/SoftEther/SoftEtherVPN/badges/master/build.svg)](https://gitlab.com/SoftEther/SoftEtherVPN/pipelines) [![Coverity Scan Build Status](https://scan.coverity.com/projects/16304/badge.svg)](https://scan.coverity.com/projects/softethervpn-softethervpn)

- 对于第三方资管团队而言，无论用的是第三方的Third-Party程序化交易软件，还是自己搭建的交易平台。一旦涉及到和其他资金方的对接，尤其是现在的资产管理交易平台，就会涉及到接口的对接问题。融航的接口是在CTP接口的基础上，进行了
强制的API注入的形式，在调用原生CTPThosTrader之前，先调用Ronhon-API进行帐号，权限等认证，再调用原生的CTP接口的方式。

-所以对于和他们接口对接流程统一都是
- 1.首先请提供对应ctp版本的thosttraderapi.dll文件给RH。
- 2.RH通过比对该文件提供对应的thosttraderapi.dll与RohonBase.dll文件。
- 3.替换CTP的thosttraderapi.dll，并将RohonBase.dll放在同级目录下，并测试登陆。

* * *
### 预期调试效果 - 1.静态终结果
*
![image](https://github.com/handayu/Ronhon-Trading/blob/master/apitestok.png)


### 预期调试过程注意替换dll - 2.动态过程
*
![image](https://github.com/handayu/Ronhon-Trading/blob/master/apitest.gif)

### 技术应用？
- ThirdPartySoftWare Multicharts
- Trade Strategy交易策略
- Use Multicharts PowerLanguage Edit
- 事件引擎
- Windows Hook
- C++
- 交易合约映射

### 目标(该项目持续更新中)
 -打通第三方软件平台或者自有程序化交易软件和融航交易接口的对接。无缝连接交易。
  
### 参与本项目？

如有兴趣的量化－程序化交易员或者团队:
   请加 QQ：578931169。
   WeChart: hanyu196910
   
   <h3 id="weibo-weixin">微信</h3>
 *「DotNet」：专注 .NET量化交易平台开发，量化交易，程序化交易，投资组合构建。
   <br><img src="https://github.com/handayu/OandaTrading/blob/master/image/wechart.jpg" width=150 height=150>
* * *
