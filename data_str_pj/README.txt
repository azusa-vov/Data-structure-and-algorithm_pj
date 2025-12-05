一.编译命令与一些结果说明

基本是按照作业要求来的，但是多次尝试后电脑本地总是显示无法将"make"识别为cmdlet，
最终用MSY32工具包在外部编译：

$ make

后，会生成loganalyzer.exe可执行文件：

$ .\loganalyzer<test/test_command.txt

这里test_command是汇总了所有用例指令的测试文件，可以在/test目录底下找到
当然，也可以采用手动编译的方法：
$ cd d:\CPP_project\.vscode\data_str_pj
g++ -Wall -std=c++11 -g -o loganalyzer main.cpp LinkedList.cpp CircularQueue.cpp BST.cpp UndoStack.cpp KMP.cpp

(需要换成本地的相关路径)
随后在交互状态下LOAD 相关文件即可进行测试，

$ ./loganalyzer
这里需要注意！！！
1.本项目的测试文件都已经备份于\test文件夹下，需要在LOAD时附上此路径：
（示例：LOAD test\test1.txt）
2.其中test3.txt和large.txt因较大，采用相应脚本编写
*3.关于内存占用和泄露：我在本地wsl使用valgrind工具进行验证，其中LOAD test3内存占用小于200MB的测试也通过
（结果截图一并附在压缩包），实际占用了34,366,370 bytes（约等于32.8MB<200MB）。关于具体指令是否泄露内存，
可以通过对/test/test_valgrind使用：

$  valgrind --leak-check=full --show-leak-kinds=all ./loganalyzer <  test/test_valgrind.txt

发现无内存泄露。



二.运行方式
进入交互模式后，等待用户输入指令。支持以下命令：
   - LOAD <文件名>                 : 读取并解析日志文件
   - FILTER <开始时间> <结束时间>  : 筛选指定时间段内的日志
   - SEARCH <关键词>               : 搜索包含关键词的日志（区分大小写）
   - STATS                         : 统计各模块 ERROR 数量并排序输出
   - DELETE <行号>                 : 删除指定行号的日志
   - UNDO                          : 撤销上一次删除操作（最多5次）
   - RECENT <数量>                 : 显示最近接收的 N 条日志
   - EXIT                          : 退出程序

三.设计简述
a.双向链表(#LogEntry.h #LinkedList.h #LinkedList.cpp)
用于存储所有的日志条目，其单一节点包含时间戳、日志级别、模块名、
消息内容四个数据域，以及 prev 和 next 指针，并能够实现项目要求
支持头插、尾插以及删除指定行号的日志。其优势在于：双向链表在已知
节点指针的情况下删除操作为 O(1)，且能够灵活地从头或从尾遍历。
本项目中链表的索引逻辑：虽然链表不支持随机访问，但 DELETE 命令
中的行号可以通过从头遍历计数找到对应节点进行操作。

b.循环队列(#CircularQueue.h #CircularQueue.cpp)
用于存储最近接收的 1000 条日志，支持 RECENT 命令。队列使用固定
大小为 1000 的数组，存储类型为“指向链表节点的指针”。其实现逻辑为：
通过维护front和rear索引。当新日志到达时，若队列未满则直接入队；
若队列已满，则移动 front 指针覆盖最旧的元素。

*需要注意的是，队列的结构在定义上和链表和BST有微妙差异，即存储指
针而非日志内容的副本，既节省了内存，又保证了通过 RECENT 查看的日
志内容与链表中的主数据保持一致（即修改链表内容能反映到队列中，但删
除链表节点也带来了同步处理队列中空悬指针的工作）。

c.二叉搜索树(#BST.h #BST.cpp)
用于统计各模块的 ERROR 级别日志数量，支持 STATS 命令。节点结构为
：Key 为模块名称（string），Value 为该模块的错误计数（int），包含
左右孩子指针。本项目的BST满足了：

>>插入和更新：在 LOAD 或添加日志时，若级别为 ERROR，则在 BST
 中查找模块名。若存在则计数+1，不存在则插入新节点。

>>删除：执行 DELETE 时，若被删日志为 ERROR 级别，则在 BST 
中对应模块计数-1。

>>遍历：STATS 命令需要按数量排序，实现时先通过中序遍历将所有节点
导出到临时数组，再对数组进行排序（先按数量降序，再按名称字典序）
后输出。

d.栈与undo(#UndoStack.h #UndoStack.cpp)
用于支持 UNDO 操作，恢复最近 5 次 DELETE 造成的系统状态变化。
结构：使用自定义栈结构，固定容量为 5。栈中元素为“系统快照”。
每次执行 DELETE 前，将当前的 链表、循环队列、BST 的状态进行深拷
贝并压入栈中。执行 UNDO 时，若栈不为空，则弹出
栈顶的快照，将其覆盖当前的内存状态，从而完全恢复到删除前的样子。

e.KMP算法(#KMP.h #KMP.cpp)
用于 SEARCH 命令中的关键词快速匹配。首先根据关键词模式串计算
next数组（部分匹配表），用于在不匹配时决定模式串向右滑动的位数，
避免回溯。对每一条日志的消息内容进行 KMP 搜索，时间复杂度为
O(N+M)，在长文本和大量日志搜索中性能优于朴素匹配算法。

四、已知问题说明
由于 UNDO 操作采用了全量深拷贝（Snapshot）机制，如果日志文
件极大（如数百万行），连续进行 DELETE 操作可能会消耗较多内
存。但在作业给定的测试规模（5MB/5万行）下运行正常。

