
#LSB算法——隐藏文字信息

-------------
LSB（Least Significant Bit）最低有效位，指二进制中最低位数值。
详见WiKi：https://en.wikipedia.org/wiki/Least_significant_bit

-----------------------

## 隐写术 ##
隐写术是一种关于信息隐藏的技术，通常利用位图的RGB三种颜色值（0~255）在微小改变的情况下难以被人眼察觉的特点。每种颜色值都可以用8位二进制来表示，而最低有效位LSB技术就是通过修改最低几位的数值来将信息嵌入到图片中去。
详见WiKi：https://en.wikipedia.org/wiki/Steganography

-------------

## 相关方法 ##

LSB方面的论文有很多，在此利用以下这篇
> Chan C K, Cheng L M. Hiding data in images by simple LSB substitution[J]. Pattern recognition, 2004, 37(3): 469-474.
http://www.sciencedirect.com/science/article/pii/S003132030300284X


其具体的思路如下：

1. 设图像像素为$M_C \times N_C$， $C$表示原8-bit图像 $$C = \{x_{ij}|0 \leq i < M_C, 0 \leq j < N_C,x_{ij} \in \{0, 1, ..., 255\}\}$$ $M$为n-bit待隐藏的消息 $$M=\{m_i|0 \leq i < n, m_i \in \{0, 1\}\}$$
2. 将$M$重排为$M'$：$$M'=\{m'_i|0 \leq i < n', m'_i \in \{0, 1, ..., 2^k-1\}\}$$ 其中$n'<M_C \times N_C$ ，则$m'$ 可以表示为：$$m'_i=\Sigma^{k-1}_{j=0}m_{i \times k+j} \times 2^{k-1-j}$$
3. 从原图中按照既定规则挑选出$n'$ 个像素：$$\{x_{l1}, x_{l2}, ..., x_{ln'}\}$$ ，嵌入过程是通过用$m'$ 替换$x_{li}$ 的$k$ 位LSBs，则$x_{li}$ 会被替换成$$x'_{li}=x_{li}-(x_{li} ~mod~ 2^k)+m'_i$$
4. 提取消息的方法为：$$m'_i=x'_{li}~mod~2^k$$

------------------

## MATLAB相关函数解释 ##

1. imread( ) 用于读取需要嵌入隐藏信息的图片，并存储为 uint8 类型的三维RGB矩阵，每个数值都位于0~255间
2. strcat( ) 用于字符串连接
3. unicode2native( ) 将 unicode 编码转化为相应的数字字节，相对应的 native2unicode( ) 是将数字字节转化为对应的 unicode 编码
4. dec2bin( ) 将十进制数转化为二进制，可选参数为最少几位二进制，相应的 bin2dec( ) 是将二进制转化为十进制
5. strjoin( ) 将元胞中的字符串数组组合成一个单字符串
6. double( ) 将字符转化成相应的 ASCII 码，相应的 char( ) 将 ASCII 码转化成字符
7. imwrite( ) 将矩阵存成图片

----------------

## 注意事项 ##

1. MATLAB的 imwrite() 函数存成图片时，若选用 .jpg 格式则会出现一定程度的失真，导致无法提取出正确信息，因此最好存为 .png 或其他格式
2. 以下给出的代码以EOF作为嵌入结束的标志
3. 以下代码适用于2-LSB
4. 以下代码有选择地将信息嵌入红、绿、蓝中的一层

----------------

## 效果 ##

<img src="./flag.jpg" width = "300" />                                           <img src="./result.png" width = "300" />

左图为原图，右图为结果图。

-----------------------

## 改进：嵌入到RGB三层中 ##

**由于单个像素点的值表示成二进制为8位，为了方便进行嵌入，补上一位，凑成9位，分别嵌入到RGB三层中，且补上的一位以‘0’、‘1’、‘0’、‘1’的顺序出现，以达到‘0’、‘1’平衡，最后一个字符嵌入时对其补上的一位取反，作为结束标记（LSBforRGB文件中）**
