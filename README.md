# ffcp
Implement of Fast Prefix Code Processing

本项目是[算法](https://www.ifi.uzh.ch/dam/jcr:ffffffff-82b7-d340-0000-000026dd8fb6/Prefix.pdf)的实现,自动生成解码表格。

## 使用方法
````
Usage: ffcp [Option]... InputFile OutputFile 
````

参数说明：

* InputFile: 输入文件，每一行是一个字符的编码，参考[文件](tests/qpack.txt)
* OuputFile: 输出文件
* Option: 目前没有任何选项

可以参考[文件](tests/ffcp_test.c)完成自己的解码函数。
