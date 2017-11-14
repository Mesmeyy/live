verilog期中考试复习 之 临时抱佛脚

1测试模块写法
module A(cin,enable,cc)
reg enable,cc;
wire cin;

initial begin ;;;end
always #delay begin ;;; end
Testmd m(.in1(ina),.in2(inb),.out(cc));//测试模块的调用

initial begim ;;; end
endmodule



2平常模块的调用
调用函数的参数不用.



3always和initial里面只能reg型 
   always@(posedge clk)//当这个时钟处于上升边沿的时候

4assign有同时的含义

5<= 保留时序电路的含义,=是逻辑的含义

6数组声明:
    input [2:0] a;//两个元素你懂得
    数字赋值:Key =  4'b1001;//一律二进制
7声明时候 :模块名字最好就是文件名字


