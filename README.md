# snake（贪吃蛇测试平台2.0版本已经发布）
### 测试平台的简单介绍
本测试平台基于windows系统开发，运行环境也是windows。选手在进行开发的时候可以直接下载源代码并且在系统上编译。
### 测试平台的编译方法
1.下载源代码[shiyan.cpp](https://github.com/williamking5/snake/blob/master/bin/Debug/shiyan.cpp)
2.下载库函数[uselei.h](https://github.com/williamking5/snake/blob/master/bin/Debug/uselei.h)
3.在本地目录下进行编译（推荐使用codeblocks，其他IDE或编译器也可以）生成shiyan.exe。
### 测试方法
1.将你写的程序生成dll（动态链接库），命名为player_1.dll或者player_2.dll。根据蛇的先后顺序确定是player_1还是player_2
2.将动态链接库拷贝到与shiyan.exe同一目录下。然后使用命令行运行shiyan.exe。
``` 
shiyan.exe player_1.dll player_2.dll
```
注意上述命令行参数，第一个是选手一的动态链接库，第二个是选手二的动态链接库。
### 注意事项：你写的主函数名称必须是player_1或player_2
### 测试平台是公开源代码的，且正在测试当中，若发现有任何bug或有任何不明白的地方请先查看源代码。
