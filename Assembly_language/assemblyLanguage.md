# 8086汇编语言 《王爽》

## 寄存器

> ### 通用寄存器

AX BX CX DX

8086CPU的上一代CPU中的寄存器都是8位的，为了保证兼容，使原来基于上代CPU编写的程序稍加修改就可以运行在8086之上，8086CPU的AX、BX、CX、DX这4个寄存器都可分为两个可独立使用的8位寄存器来用：
- AX可分为 AH，AL 
- BX可分为 BH，BL 
- CX可分为 CH，CL 
- DX可分为 DH，DL 

以AX为例，8086CPU的16位寄存器分为两个8位寄存器
![16位寄存器](./Assembly_language.assets/16位寄存器.png)

> ### 段寄存器

- CS 代码段寄存器
- DS 数据段寄存器
- SS 栈寄存器
- ES 扩展栈寄存器

立即数无法直接送入段寄存器

CS 和 IP是8086cpu中两个最关键的寄存器。他指示了cpu当前要读取指令的地方。CS为**代码段寄存器**，IP为**指令指针寄存器**

8086CPU 中，有两个奇存器，段寄存器 sS 和寄存器 SP，栈顶的段地址存放在SS 中，偏移地址存放在SP 中。任意时刻，SS:SP 指向栈顶元素。push 指令和 pop 指
令执行时，CPU 从SS 和SP 中得到栈顶的地址


> #### 修改CS IP的指令
在 CPU 中，程序员能够用指令读写的部件只有寄存器，程序员可以通过改变寄存器中的内容实现对 CPU 的控制。CPU 从何处执行指令是由 CS、IP 中的内容决定的，程序
员可以通过改变CS、IP中的内容来控制 CPU 执行目标指令。

可以修改CS，PC的最简单的指令：jmp指令处取指令

若想同时修改CS，IP的内容，可用形如“jmp 段地址:偏移地址”的指令完成。例如
jmp 2AE3:3 执行后：CS=2AE3H IP=3  CPU将从2AE33H开始执行

若想修改IP的内容，可以形如`jmp 某一合法寄存器`的指令完成。例如
mov ax,3H
jmp ax  执行后：IP=3H  CPU将从3H开始执行


> #### 字在寄存器中的存储

- 字节 Byte, 一个字节由8个bit组成,可以存在8位寄存器中
- 字 word, 一个字由两个字节组成，这两个字节分别为这个字的高位字节，与地位字节

一个字既可以看成一个1byte数据类型，也可以看成2个8bit数据类型

S BP  PSW 

> #### 8086cpu给出地址的方法

8086CPU有20位地址总线，可以传送20位地址，达到1MB 寻址能力。8086CPU又是16位结构，在内部一次性处理、传输、暂时存储的地址为 16位。从8086CPU的内部结构来看，如果将地址从内部简单地发出，那么它只能送出16位的地址，表现出的寻址能力只有 64KB。故8086CPU采用一种在内部用两个16位地址合成的方法来形成一个20 位的物理地址。
![物理地址寻址](./Assembly_language.assets/265BF63E630C355E1859A83BE9C3D2C8.png)
![物理地址寻址](./Assembly_language.assets/5512D8C84C381D59205115FC6FF64699.png)



> #### DS 和 [address]

CPU 要读写一个内存单元的时候，必须先给出这个内存单元的地址，在8086PC 中，内存地址由段地址和偏移地址组成。8086CPU 中有一个 DS 寄存器，通常用来存放要访问数据的段地址。比如我们要读取 10000H 单元的内容，可以用如下的程序段进行。
```asm
mov bx,1000H
mov ds,bx
mov al,[0]
```
上面的3条指令将 10000H(1000:0)中的数据读到al中。

其中寄存器ds中的值为内存的段地址，`[]`代表偏移地址

前面我们使用mov 指令，可完成两种传送：①将数据直接送入寄存器：②将一个寄存器中的内容送入另一个寄存器。

也可以使用mov 指令将一个内存单元中的内容送入一个寄存器中。从哪一个内存单元送到哪一个寄存器中呢？在指令中必须指明。寄存器用寄存器名来指明，内存单元则需用内存单元的地址来指明。显然，此时 mov 指令的格式应该是：mov 寄存器名，内存单元地址。
`[]`表示一个内存单元，`[…]` 中的 。表示内存单元的偏移地址。

> #### 字的传送

字在内存中存储时，要用两个地址连续的内存单元来存放，字的低位宇节存放在低地址单元中，高位字节存放在高地址单元中。

在内存和奇存器之间传送字型数据时，高地址单元和高8位奇存器、低地址单元和低 8位奇存器相对应。

> #### 数据段

对于8086PC 机，在编程时，可以根据需要，将一组内存单元定义为一个段。我们可以将一组长度为 N(≤64KB)、地址连续、起始地址为 16 的倍数的内存单元当作专门存储数据的内存空间，从而定义了一个数据段。比如用 123BOH~123B9H 这段内存空间来存放数据，我们就可以认为，123B0F~123B9H 这段内存是一个数据段，它的段地址为 123BH，长度为10个字节。

如何访问数据段中的数据呢？将一段内存当作数据段，是我们在编程时的一种安排，可以在具体操作的时候，用 ds 存放数据段的段地址，再根据需要，用相关指令访问数据段中的具体单元。

> ### cpu提供的栈机制

现今的 CPU 中都有栈的设计，8086CPU 也不例外。8036CPU 提供相关的指令来以栈的方式访问内存空间。这意味着，在基于 8086CPU 编程的时候，可以将一段内存当作栈
来使用。
8086CPU 提供入栈和出栈指令，最基本的两个是 PUSH（入栈）和 POP（出栈）。比如，push ax 表示将寄存器ax 中的数据送入栈中，pop ax 表示从栈顶取出数据送入 ax。
8086CPU 的入栈和出栈操作都是以字为单位进行的

8086CPU 中，有两个奇存器，段寄存器 sS 和寄存器 SP，栈顶的段地址存放在SS 中，偏移地址存放在SP 中。**任意时刻，SS:SP 指向栈顶元素**。push 指令和 pop 指
令执行时，CPU 从SS 和SP 中得到栈顶的地址

### DI DI寄存器

SI 和 DI寄存器是8086寄存器中和bx功能相似的寄存器，但SI、DI寄存器无法分成两个8位寄存器来使用





## debug程序

不区分大小写

R命令  查看，改变cpu寄存器的内容
```
r  查看所有寄存器ax的值
r ax 修改寄存器ax中的值
```

D命令  查看内存中的内容
```
d 1000:0  查看10000H处的内存，但显示8行（每一行16个字节）
d 1000:2 9  查看10002H~10009H的内容

指定段寄存器   E U A命令同理
d ds:0 查看ds段寄存器指向的地址加上偏移量处的内存
```

E命令  改写内存中的内容
```
e 1000:0 1 2 3 4 5 6...  改写10000H及以后的数据
e 1000:0 + enter 将会逐个地址修改数据，按空格跳过，再次按下enter执行修改
```

U命令 查看内存中机器码含义, 用法和D命令类似
```
u 1000:0  查看10000H处的内存，但显示8行（每一行16个字节）
u 1000:2 9  查看10002H~10009H的内容
```

T命令  执行机器码
```
t 执行cs ip寄存器中指向的指令
```
![t指令](./Assembly_language.assets/t指令.png)

A命令 以汇编的形式在内存中写入指令
![a指令](./Assembly_language.assets/a指令.png)


## 汇编指令

> mov  mov指令被称为传送指令

mov指令可以有一下几种形式
```asm
mov 寄存器,数据        mov ax,0115H
mov 寄存器,寄存器       mov ax,bx
mov 寄存器,内存单元     mov ax,[0]
mov 内存单元,寄存器     mov [0],ax
mov 段寄存器,寄存器     mov ds,ax
```



> add 加指令  双操作数
> sub 减指令

> jmp 跳转指令 用于修改cs ip寄存器的值

> inc 加1 
```asm
inc bx 的含义是将bx中的内容加1
mov bx,1
inc bx
执行后bx = 2
```

> push pop指令
![push pop指令](./Assembly_language.assets/DF97CB08B788F1395D36066FF86C2C07.png)
![push pop指令](./Assembly_language.assets/E35DFF53DE488C798B346DA5ECDA3114.png)

- push 指令的执行步骤：①SP-SP-2：②向 SS:SP 指向的字单元中送入数据。
- pop 指令的执行步骤：①从 SS:SP 指向的字单元中读取数据：②SP-SP+2。
- 任意时刻，ss:SP 指向栈顶元素。
- 8086CPU 只记录栈顶，栈空间的大小我们要自己管理。
- 用栈来暂存以后需要恢复的寄存器的内容时，寄存器出栈的顺序要和入栈的顺序相反。
- push、pop 实质上是一种内存传送指令，注意它们的灵活应用。

> and和or指令  按位与 按位或 双操作数

```asm
mov al,00100011b
and al,01100011B
; 按位与操作后 al中的值为 000100011b
```




## 第一个程序

### 汇编语言从写出到执行的过程

1. 编写汇编源程序

2. 对源程序进行编译连接

编译成的可执行文件包含2部分
- 程序（从源程序中的汇编指令翻译过来的机器码）和数据（源程序中定义的数据）
- 相关的描述信息（比如：程序有多大，要占多少内存空间）

这一步工作的结果：产生了一个可在操作系统中运行的可执行文件

3. 执行可执行文件

操作系统依照可执行文件中的描述信息，将可执行文件中的机器码和数据加载如内存，并进行相关的初始化（比如设置CS:IP指向第一条要执行的指令），然后由cpu执行指令

### 源程序

![源程序](./Assembly_language.assets/AC3202F61C323249693CC49C98974E06.png)

#### 伪指令

在汇编语言源程序中，包含两种指令，一种是汇编指令，一种是伪指令。汇编指令是有对应的机器码的指令，可以被编译为机器指令，最终为 CPU 所执行。而伪指令没有对应的机器指令，最终不被 CPU 所执行。那么谁来执行伪指令呢？伪指令是由编译器来执行的指令，编译器根据伪指令来进行相关的编译工作。


> 段

上述源程序中，segment，ends是一对成对使用的伪指令。segment，ends的功能是定义一个段。segment说明一个段开始，ends说明一个段结束。一个段必须有一个名称来标识，使用格式为：

段名 segment

段名 ends

一个汇编程序是由多个段组成的，这些段被用来存放代码、数据或当作栈空间来使用。我们在前面的课程中所讲解的段的概念，在汇编源程序中得到了应用与体现，一个源程序中所有将被计算机所处理的信息：指令、数据、栈，被划分到了不同的段中。(这里的段和c语言中内存空间分布中的段是一样的)

一个有意义的汇编程序中至少要有一个段，这个段用来存放代码。

我们可以看到，程序4.1中，在codesg segment 和 codesg ends 之间写的汇编指令是这个段中存放的内容，这是一个代码段(其中还有我们不认识的指令，后面会进行讲解）。

> end

end 是一个汇编程序的结束标记，编译器在编译汇编程序的过程中，如果碰到了伪指令end，就结束对源程序的编译。所以，在我们写程序的时候，如果程序写完了，要在结尾处加上伪指令end。否则，编译器在编译程序时，无法知道程序在何处结束。
注意，不要搞混了 end 和ends， ends 是和 segment 成对使用的，标记一个段的结束，ends 的含义可理解为“end segment”。我们这里讲的end 的作用是标记整个程序的结束。

> assume

这条伪指令的含义为“假设”。它假设某一段寄存器和程序中的某一个用segment...ends 定义的段相关联。通过 assume 说明这种关联，在需要的情况下，编译程序可以将段寄存器和某一个具体的段相联系。assume 并不是一条非要深入理解不可的伪指令，以后我们编程时，记着用 assume 将有特定用途的段和相关的段寄存器关联起来即可

#### 源程序中的“程序”

用汇编语言写的源程序，包括伪指令和汇编指令，我们编程的最终目的是让计算机完成一定的任务。源程序中的汇编指令组成了最终由计算机执行的程序，而源程序中的伪指令是由编译器来处理的，它们并不实现我们编程的最终目的。这里所说的程序就是指源程序中最终由计算机执行、处理的指令或数据。

注意，以后可以将源程序文件中的所有内容称为源程序，将源程序中最终由计算机执行、处理的指令或数据，称为程序。程序最先以汇编指令的形式存在源程序中，经编译、连接后转变为机器码，存储在可执行文件中。这个过程如图4.2所示。
![程序](Assembly_language.assets/DB6EB34CE2AB7B397565D3EB8DE4078E.png)

#### 标号

汇编源程序中，除了汇编指令和份指令外，还有一些标号，比如“codesg”。一个标号指代了一个地址。比如codesg 在segment 的前面，作为一个段的名称，这个段的名称最
终将被编译、连接程序处理为一个段的段地址。


#### 程序的结构

编程运算2^3的结构
![汇编程序结构](./Assembly_language.assets/AC3202F61C323249693CC49C98974E06.png)

#### 程序的返回

一个程序结束后，将 CPU 的控制权交还给使它得以运行的程序，我们称这个过程为：程序返回。那么，如何返回呢？应该在程序的末尾添加返回的程序段。

我们回过头来，看一下程序 4.1 中的两条指令：
```asm
mov ax, 4c00H
int 21H
```
这两条指令所实现的功能就是程序返回给操作系统。

![和结束相关的概念](./Assembly_language.assets/B7A024802FA6BA49A7FC03A85DCC1E59.png)

### 编译连接

编译使用masm程序。在命令行中输入 
```shell
masm <文件名>;
```

连接使用link程序。在命令行中输入 
```shell
link <文件名>;
```

### 跟踪

跟踪使用debug程序。在命令行中输入 
```shell
debug <文件名全称>;
```

## 第5章\[BX\]和loop指令

### 5.1 \[BX\]

`mov ax,[bx]`

功能：bx 中存放的数据作为一个偏移地址 EA，段地址SA 默认在ds 中，将SA:EA处的数据送入ax中。即：(ax)=((d5)*16+(bx)。

`mov [bx],ax`

功能：bx 中存放的数据作为一个偏移地址 EA，段地址SA 默认在ds中，将ax 中的数据送入内存 SA:EA 处。即：(ds)*16+(bx))=(ax)。

### 5.2 Loop指令

loop 指令的格式是：loop 标号，CPU 执行 loop 指令的时候，要进行两步操作：
1. (cx)=(cx)-1x 
2. 判断 cx 中的值，不为零则转至标号处执行程序，如果为零则向下执行。

要执行n次，cx应设为n


### [BX]与loop的联合应用

```asm
; 计算ffff:0~ffff:b单元中的数据和，结果存储在dx中
assume cs:code

code segment
    mov ds,0ffffH
    mov cx,12
    mov bx,0
    mov dx,0
s: mov al,[bx]
    mov ah,0
    add dx,ax
    
    loop s

    mov ax,4c00H
    int 21
code ends
end

; 分析
; 首先考虑dx是否会发生溢出，因为8位数据最大为255，12个255相加，结果远小于16位存储空间的65535.
; 对于8位的字节型数据，无法直接与16位的dx相加，所以要对8位数据进行处理
; 这里使用了ax这样的一个中间寄存器，将数据段中的8位数据拓展位16位
; al存放低8位的数据，ah存放0000
; 最后让dx与ax进行累加
```

## 第6章 包含多个段的程序

```asm
assume cs:code,ds:data,ss:stack

data segment
    dw 0123H,0456H,0789H,0abcH,0defH,0fedH,0cbaH,0987H
data ends
 
stack segment
    dw 0,0,0,0,0,0,0,0,0,0,0,0,0
stack ends

code segment
start:

    mov ax,stack
    mov ss,ax  
    mov sp,20H ; 设置栈顶 stack:20
    
    mov ax,data
    mov ds,ax  ; 设置数据段
    mov bx,0   ; ds:bx 指向data段的第一个单元

    mov cx,8

s:  push [bx]
    add bx,2
    loop s

    mov bx,0
    mov cx,8

s0: pop [bx]
    add bx,2
    loop s0

    mov ax,4c00H
    int 21h

code ends
end start


```
下面对以上程序作出说明
1. 定义多个段的方法

    这点，我们从程序中可明显地看出，定义一个段的方法和前面所讲的定义代码段的方法没有区别，只是对于不同的段，要有不同的段名。
2. 对段地址的引用

    现在，程序中有多个段了，如何访问段中的数据呢？当然要通过地址，而地址是分为两部分的，即段地址和偏移地址。如何指明要访问的数据的段地址呢？在程序中，段名就相当于一个标号，它代表了段地址。所以指令`mov ax,data`的含义就是将名称为`data`的段的段地址送入`ax`。一个段中的数据的段地址可由段名代表，偏移地址就要看它在段中的位置了。程序中`data`段中的数据 `0abch` 的地址就是：`data:6`。要将它送入`bx`中，就要用如下的代码：
    ```asm
    mov ax, data 
    mov ds, ax
     mov bx,ds:[6]
    ```
    我们不能用下面的指令：
    ```asm
    mov ds, data 
    mov bx,ds:[6]
    ```
    其中指令`mov ds,data` 是错误的，因为 `8086CPU`. 不允许将一个数值直接送入段寄存器中。程序中对段名的引用，如指令`mov ds,data` 中的`data`，将被编译器处理为一个表示段地址的数值。
3. `代码段`、`数据段`、`栈段`完全是我们的安排

    现在，我们以一个具体的程序来再次讨论一下所谓的`代码段`、`数据段`、`栈段`。在汇编源程序中，可以定义许多的段，比如在程序中，定义了了个段，`code`、`data`和`stack`。我们可以分别安排它们存放代码、数据和栈。

## 第7章 更灵活的定位内存地址的方法

>### 以字符形式给出的数据

在汇编语言中，用`'...'`的方式指明的数据是以字符的形式给出的，编译器将把他们转化为`ascii码`
```asm
assume cs:code,ds:data
data segment
    db 'unIX'
    db 'foRK'
data ends

code segment

start:  mov al,'a'
        mov bl,'b'
        mov ax,4c00H
        int 21H
code ends
end start
```

>### [bx + idata]

在前面，我们用`[bx]`的方式来指明一个内存单元，还可以用一种更为灵活的方式来指明内存单元：`[bx + idata]`表示一个内存单元，它的偏移地址为`(bx)+idata`(bx 中的数值加上idata)

我们看一下指令`mov ax,[bx+200]`的含义：

将一个内存单元的内容送入ax，这个内存单元的长度为2个字节(字单元)，存放一个字，偏移地址为bx 中的数值加上 200，段地址在ds中。

数学化的描述为：(ax)=((ds)*16+(bx)+200)

该指令也可以写成如下格式（常用）：
```asm
mov ax, [200+bx]
mov ax, 200 [bx]
mov ax, [bx].200
```



>### SI DI寄存器

SI 和 DI寄存器是8086寄存器中和bx功能相似的寄存器，但SI、DI寄存器无法分成两个8位寄存器来使用

>### [bx+si] 和 [bx+di]

在前面，我们用`[bx(si或di)]`和`[bx(si或di)+idata]`的方式来指明一个内存单元，我们还可以用更为灵活的方式：`[bx+si]`和`[bxtdi]`。

`[bxtsi]`和`[bx+di]`的含义相似，我们以`[bx+si]`为例进行讲解。

`bx+si`表示一个内存单元，它的偏移地址为`(bx)+(si)`（即 bx 中的数值加上 si 中的数值）。

指令 mov ax,[bx+si]的含义如下：

将一个内存单元的内容送入 `ax`，这个内存单元的长度为 2字节(字单元)，存放一个字，偏移地址为 `bx`中的数值加上`si`中的数值，段地址在`ds`中。
数学化的描述为：(ax)=((ds)*16+(bx)+(si))
该指令也可以写成如下格式(常用)：
```asm
mov ax, [bx][si]
```

代码示例

```asm
assume cs:code,ds:data
data segment 
    db 'welcome to masm'
    db '...............'
data ends
code segment
start:  
        mov ax, data
        mov ds, ax
        mov di, 0
        mov si, 16
        mov cx, 8
s:      mov ax, ds:[di]
        mov ds:[si], ds:[di]
        add si, 2
        add di, 2
        loop s
        mov ax, 4c00H
        int 21H
code ends
end start
```

>### [bx+si+idata] 和 [bx+di+idata]

将`si`和`idata`结合起来，可以做到更加灵活的内存取地址方式

代码示例

```asm
assume cs:code,ds:data
data segment 
    db 'welcome to masm'
    db '...............'
data ends
code segment
start:        
        mov ax, data
        mov ds, ax
        mov di, 0
        mov cx, 8
s:      mov ax, ds:[di]
        mov ds:[di+16], ax
        add di, 2
        loop s
        mov ax, 4c00H
        int 21H
code ends
end start
```


>### 不同寻址方式的灵活应用

1. `[idata]`用一个常量来表示地址，可用于直接定位一个内存单元；
2. `[bx]`用一个变量来表示内存地址，可用于间接定位一个内存单元；
3. `[bx+idata]`用一个变量和常量表示地址，可在一个起始地址的基础上用变量间接定位一个内存单元；
4. `[bx+si]`用两个变量表示地址：
5. `[bx+si+idata]`用两个变量和一个常量表示地址。
可以看到，从`[idata]`一直到`[bx+si+idata]`，我们可以用更加灵活的方式来定位一个内存单元的地址。这使我们可以从更加结构化的角度来看待所要处理的数据。

```asm
; 编程，将 data 段中每个单词的头一个字母改为大写字母。
assume cs:code, ds:data
data segment
    db '1. file      '
    db '2. edit      '
    db '3. search    '
    db '4. view      '
    db '5. options   '
    db '6. help      '
data ends
code segment
start:  mov ax, data
        mov ds, ax
        mov cx, 6
        mov si, 13
        mov di, 3
    s:  mov al, ds:[bx + di]
        and al, 11011111b
        mov ds:[bx + di], al
        add bx, si
        loop s
        mov ax, 4c00h
        int 21H
code ends
end start    
    
```

## 第8章 数据处理的两个基本问题


本章是对前面的所有内容是具有总结性的。我们知道，计算机是进行数据处理、运算的机器，那么有两个基本的问题就包含在其中：
1. 处理的数据在什么地方？
2. 要处理的数据有多长？
这两个问题，在机器指令中必须给以明确或隐含的说明，否则计算机就无法工作。本章中，我们就要针对 8086CPU 对这两个基本问题进行讨论。虽然讨论是在 8086CPU 的基础上进行的，但是这两个基本问题却是普遍的，对任何一个处理机都存在。
我们定义的描述性符号：`reg` 和`sreg`

为了描述上的简洁，在以后的课程中，我们将使用两个描述性的符号`reg` 来表示一个寄存器，用`sreg` 表示一个段寄存器。

reg的集合包括：

`ax bx ex dx ah al bh bl ch cl dh dl sp bp si di`

sreg 的集合包括：

`cs ds ss es`

>### bx,si,di和bp

- 在8086cpu中，只有这4个寄存器可以用在`[...]`中来进行内存单元的寻址
    ```asm
    ; 比如下面的指令都是正确的
    mov ax, [bx]
    mov ax, [bx+si]
    mov ax, [bx+di]
    mov ax, [bp]
    mov ax, [bp+si]
    mov ax, [bp+di]
    ```
    ```asm
    ; 而下面的指令是错误的
    mov ax, [cx]
    mov ax, [ax]
    mov ax, [bx]
    mov ax, [dx]
    
    ```
- 在[...]中，这4个寄存器可以单个出现，或只能以4种组合出现:
  `bx和si` `bx和di` `bp和si` `bp和di`
    ```asm
    ; 如下指令是正确的
    mov ax, [bx] 
    mov ax, [si] 
    mov ax, [di] 
    mov ax, [bp] 
    mov ax, [bx+si] 
    mov ax, [bx+di] 
    mov ax, [bp+sil] 
    mov ax, [bptdi] 
    mov ax, [bx+si+idata] 
    mov ax, [bx+di+idata] 
    mov ax, [bp+si+idata]
    mov ax, [bp+di+idata]
    ; 下面的指令是错误的：
    mov ax, [bx+bp]
    mov ax, [si+di]
    ```
- 只要在`[...]`中使用寄存器`bp`, 而指令中没有显示的给出段地址, 其段地址就默认在ss中。

>### 汇编语言中数据的位置的表达

汇编语言用3个概念来表达数据的位置
1. 立即数

对于直接包含在机器指令中的数据(执行前在 CPU 的指令缓冲器中)，在汇编语言中称为：立即数(`idata`)，在汇编指令中直接给出。

2. 寄存器

指令要处理的数据在寄存器中，在汇编指令中给出相应的寄存器名

3. 段地址(SA)和偏移地址(EA)

    指令要处理的数据在内存中，在汇编指令中，汇编指令中可用[X]的格式给出`EA`。`SA`在某个段寄存器中

    存放段地址的寄存器可以是默认的，比如：
    ```asm
    mov ax, [0] 
    mov ax, [di] 
    mov ax, [bx+8] 
    mov ax, [bx+si]
    mov ax, [bx+si+8]
    ; 等指令，段地址默认在ds中；
    mov ax, [bp] 
    mov ax, [bp+8] 
    mov ax, [bp+si]
    mov ax, [bp+si+8]
    ; 等指令，段地址默认在 ss中。
    
    ; 存放段地址的寄存器也可以是显性给出的，比如以下的指令。
    mov ax, ds:[bp]  ; 含义：(ax)=((ds) *16+ (bp)）
    mov ax, es:[bx]  ; 含义：(ax)=((es)*16+(bx)）
    mov ax, ss: [bx+si]  ; 含义：(ax)=((ds*16 + (bx) + (si)）
    mov ax, cs: [bx+si+8]  ; 含义：（ax)=((cs)*16+(bx)+(si)+8）
    ```


>### 寻址方式

我们可以用多种方式来给定这个内存单元的偏移地址，这种定位内存单元的方法一般被称为寻址方式。



|   寻址方式    |       名称       | 常用格式举例                   |
| :-----------: | :--------------: | ------------------------------ |
|    [idata]    |     直接寻址     | [idata]                        |
|     [bx]      |        ⌉         |                                |
|     [si]      |  寄存器间接寻址  | [bx]                           |
|     [di]      |        \|        |                                |
|     [bp]      |        ⌋         |                                |
|  [bx+idata]   |        ⌉         | 用于结构体[bx].idata           |
|  [si+idata]   |        \|        | 用于数组idata[si], idata[di]   |
|  [di+idata]   |  寄存器相对寻址  | 用于二维数组 \[bx\]\[idata\]   |
|  [bp+idata]   |        ⌋         |                                |
|    [bx+si]    |        ⌉         |                                |
|    [bx+di]    |        \|        |                                |
|    [bp+si]    |   基址变址寻址   | 用于二维数组 \[bx\]\[si\]      |
|    [bp+di]    |        ⌋         |                                |
| [bx+si+idata] |        ⌉         | 用于结构中的数组               |
| [bx+di+idata] |        \|        | [bx].idata[si]                 |
| [bp+si+idata] | 相对基址变址寻址 | 用于二维数组 idata\[bx\]\[si\] |
| [bp+di+idata] |        ⌋         |                                |



>### 指令处理的数据长度

8086CPU 的指令，可以处理两种尺寸的数据，`byte`和`word`。所以在机器指令中要指明，指令进行的是字操作还是字节操作。对于这个问题，汇编语言中用以下方法处理。

1. 通过寄存器名指明要处理的数据的尺寸
```asm
; 例如，下面的指令中，寄存器指明了指令进行的是宇操作。
    mov ax, 1 
    mov bx,ds: [0] 
    mov ds, ax 
    mov ds: [0], ax 
    inc ax add 
    ax, 1000
; 下面的指令中，寄存器指明了指令进行的是字节操作。
    mov al, 1 
    mov al,bl
    mov al, ds: [0] 
    mov ds: [0], al 
    inc al 
    add al, 100

```

2. 在没有寄存器名存在的情况下，用操作符`X ptr`指明内存单元的长度，`X`在汇编指令中可以为`word`或`byte`
```asm
; 例如，下面的指令中，用word ptr 指明了指令访问的内存单元是一个字单元。
    mov word ptr ds:[0], 1 
    inc word ptr [bx] 
    inc word ptr ds: [0] 
    add word ptr [bx], 2
; 下面的指令中，用byte ptr 指明了指令访问的内存单元是一个字节单元。
    mov byte ptr ds: [0], 1 
    inc byte ptr [bx] 
    inc byte ptr ds: [0] 
    add byte ptr [bx], 2
```
**在没有寄存器参与的内存单元访问指令中，用`word ptr` 或 `byte ptr` 显性地指明所要访问的内存单元的长度是很必要的。否则，CPU 无法得知所要访问的单元是字单元，还是字节单元。**




### div指令

div 是除法指令，使用 div 做除法的时候应注意以下问题。

- 除数：有8位和16位两种，在一个reg 或内存单元中。
- 被除数：默认放在 AX或DX和AX 中，如果除数为 8位，被除数则为 16位，默认在 AX 中存放；如果除数为 16位，被除数则为 32位，在DX和 AX 中存放，DX存放高16位，AX 存放低 16位。
- 结果：如果除数为8位，则 AL 存储除法操作的商，AH 存储除法操作的余数；
如果除数为16位，则AX 存储除法操作的商，DX 存储除法操作的余数。


### 伪指令 db dw dd

>- db 定义字节型数据
>- dw 定义字型数据
>- dd 定义双字型数据

```asm
; 用div计算data段中得到第一个数据除以第二个数据后的结果，商存在第3个数据单元中
assume cs:code,ds:data

data segment
    dd 100001
    dw 100
    dw 0
data ends

code segment 
start:  mov ax, data
        mov ds, ax

        mov  ax, ds:[0]     ; 初始化被除数， 对于32位被除数，ax放低位，dx放高位，
        mov  dx, ds:[2]     ; 对于16位及以下被除数，均放置ax寄存器中
                            ; 运算结果， 低位放置商，高位放置余数
        div word ptr ds:[4]
        mov ds:[6], ax

        mov ax, 4c00H
        int 21H

code ends

end start
 
```



### dup

`dup`是一个操作符，在汇编语言中同 `db`、`dw`、`dd`等一样，也是由编译器识别处理的符号。它是和`db`、`dw`、`dd` 等数据定义伪指令配合使用的，用来进行数据的重复。比如：

`db 3 dup (0)`
定义了了个字节，它们的值都是0，相当于 `db 0,0,0`。
`db 3 dup (0,1,2)`
定义了9个字节，它们是 0,1,2,0,1,2,0,1,2，相当于 `db 0,1,2,0,1,2,0,1,2`。

`db 3 dup ('abc', 'ABC')`
定义了18个字节，它们是'abcABCabcABCabcABC'，相当于 db'abcABCabcABCabcABC'。
可见，dup 的使用格式如下。
db 重复的次数dup（重复的字节型数据）
dw 重复的次数dup（重复的字型数据）
ad 重复的次数dup（重复的双字型数据）
dup是一个十分有用的操作符，比如要定义一个容量为200 个字节的栈段，如果不用
dup，则必须：
```asm
stack segment
dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
stack ends
```
当然，你可以用 `dd`，使程序变得简短一些，但是如果要求定义一个容量为 1000 字节或 10000字节的呢？如果没有`dup`，定义部分的程序就变得太长了，有了 `dup` 就可以轻松解决。











## 实验代码

### 实验5
(5):

```asm
assume cs:code

a segment
    db 1,2,3,4,5,6,7,8
a ends

b segment 
    db 1,2,3,4,5,6,7,8
b ends

c segment
    db 0,0,0,0,0,0,0,0
c ends

code segment 

start:  mov cx, 8
        mov ax, a
        mov ds, ax
        mov ax, b
        mov es, ax
        mov bx, 0

    S:  mov al, es:[bx]
        add ds:[bx], al
        inc bx
        loop s

        mov cx, 8
        mov bx, 0
        mov ax, c
        mov es, ax
 move:  mov al, ds:[bx]
        mov es:[bx], al
        inc bx
        loop move      

        mov ax, 4c00H
        int 21H

code ends
end start
```

(6):

```asm
assume cs:code

a segment 
    dw 1,2,3,4,5,6,7,8,9,0aH,0bH,0ch
a ends

b segment
    dw 0,0,0,0,0,0,0,0,0
b ends

code segment 
start:  
    mov ax, a
    mov ds, ax
    mov ax, b
    mov ss, ax
    mov sp, 17
    mov cx, 8
s:  mov ax, ds:[bx]
    push ax
    add bx, 2
    loop s

    mov ax, 4c00H
    int 21H

code ends

end start
```


### 实验6

(1) 
```asm
; 将数据段中的小写字母转成大写字母
; 重点在于两次循环中cx的处理
assume cs:code, ds:data

data segment 
    db '1. display     '
    db '2. brows       '
    db '3. replace     '
    db '4. modify      '

data ends

code segment
start:  mov ax, data  ; 使ds寄存器指向设定好的数据段
        mov ds, ax

        mov ax, 4H         
        mov ds:[40H], ax
        mov ax, 3H
        mov ds:[42H], ax   ; 将两层循环的循环次数存入内存中，从而减少寄存器的占用

        mov cx, ds:[40H]   ; 先看下一条，   因为最初cx未初始化，所以模拟一次外层循环结束
    s1:     
        mov ds:[40H], cx   ; 在外层每次循环结束，将外层的cx放入内存
        mov cx, ds:[42H]
        mov si, 0

    s2: mov al, ds:[bx+si]  ; 将指定数据段中的小写字母转化为大写 
        and al, 11011111b
        mov ds:[bx+si], al
        inc si
        loop s2

        mov cx, ds:[40H]    ; 将外层循环的cx取出 
        add bx, 10H         ; 转到下一行数据
        loop s1


    mov ax, 4c00H
    int 21H

code ends
end start
```

(2)

```asm
; 将数据段中的每行前4个字母转化成大写， 用栈实现cx的存储
; 对于内部循环的cx可以使用立即数在每次循环前进行初始化，
; 而内部循环直接push进栈中，就不用记忆数据存储在内存的地址了

; 代码结构与上节代码类似，就不再赘述
assume cs:code,ds:data,ss:stack
data segment 
    db '1. display      '
    db '2. brows        '
    db '3. replace      '
    db '4. modify       '
data ends

stack segment
    dw 0,0,0,0,0,0,0,0
stack ends

code segment
start:  mov ax, data
        mov ds, ax

        mov ax, stack
        mov ss, ax
        mov sp, 10H

        mov cx, 4
    
    s1: push cx

        mov cx, 4
        mov si, 0
    s2: mov al, ds:[bx+si+3] 
        and al, 11011111b
        mov ds:[bx+si+3], al

        inc si
        loop s2

        pop cx       
        add bx, 10H
        loop s1

    mov ax, 4c00H
    int 21H
code ends
end start
```







