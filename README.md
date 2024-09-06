﻿
## Inline Hook

由于使用 jmp指令，不好在跳转后的函数中跳转回来，因此换成了 call指令

1. 使用裸函数

    ```c
    __declspec(naked)  // 声明为裸函数，编译器不会为该函数 产生保存栈帧以及开辟栈空间的指令
    void  InlineHook()
    {
        __asm
        {
            pop eax  // 函数返回地址 存储到 eax
            push ebp
            mov ebp, esp
            sub esp, 0xCC

            // ----------------- 具体实现操作指令---------------
            mov dword ptr ds:[ecx],6666  // 最好是用 汇编，而不是 C语言的变量
            // -------------------------------------------
        }
        __asm
        {
            push eax  // 让 retn 指令能够正确返回到上个函数的位置
            retn
        }
    }
    ```

2. 要注意替换指令后，影响了几条原先正常的指令。

3. 影响了几条原先正常的指令就要在跳转后的函数中实现几条原先正常的指令

4. 注意堆栈平衡问题，还要注意执行到 retn指令时，栈顶的值是不是函数返回地址
