#include "inlinehook.h"
#include "logutil.h"


BYTE* oldBytes = new BYTE[20];
BYTE* newBytes = new BYTE[20];
BOOL g_IsHook = false;
DWORD g_protectOriginal;


void* GetModuleAddr(const WCHAR* moduleName)
{
    void* handle = nullptr;
    handle = (void*)GetModuleHandle(moduleName);
    return handle;
}

// 计算函数具体位置
BYTE* GetTargetFunc(BYTE* ModuleAddr)
{
    BYTE* FuncBegin = nullptr;
    FuncBegin = (BYTE*)(((DWORD)ModuleAddr + 0x11000 + 0x1570));
    return FuncBegin;
}

BOOL BackInsts(BYTE* func, BYTE* bytesArr , SIZE_T size)
{
    SIZE_T dwRet = 0;
    BOOL bRet = ReadProcessMemory(GetCurrentProcess(), func, bytesArr, size, &dwRet);
    if (!(bRet && (dwRet == size)))
        return false;
    return true;
}

BOOL SetupHook(BYTE* func, BYTE* bytesArr, SIZE_T size)
{
    HANDLE CurHandle = GetCurrentProcess();
    SIZE_T dwRet = 0;
    VirtualProtectEx(CurHandle, func, size,PAGE_EXECUTE_READWRITE,&g_protectOriginal);
    BOOL bRet = WriteProcessMemory(CurHandle, func, bytesArr, size, &dwRet);
    if (!(bRet && (dwRet == size)))
        return false;
    return true;
}


BOOL Hook()
{
    std::string msg;
    SIZE_T dwRet;
    // 1. 获取要被hook函数的起始地址
    void* ModuleAddr = GetModuleAddr(L"Demo_add_func.exe");
    if (ModuleAddr == nullptr)
        return false;
    LogUtils::DEBUG(msg);
    msg.append("ModuleAddr: ");
    msg += (DWORD)ModuleAddr;
    LogUtils::DEBUG(msg);
    msg.clear();

    BYTE* FuncBeginPos = GetTargetFunc((BYTE*)ModuleAddr);
    msg.append("FuncBegin Pos:");
    msg += (DWORD)FuncBeginPos;
    LogUtils::DEBUG(msg);
    msg.clear();

    msg.append("oldBytes Pos:");
    msg += (DWORD)oldBytes;
    LogUtils::DEBUG(msg);
    msg.clear();

    msg.append("newBytes Pos:");
    msg += (DWORD)newBytes;
    LogUtils::DEBUG(msg);
    msg.clear();

    // 2. 备份原函数开头指令
    BackInsts(FuncBeginPos, oldBytes, 5);  // 将 FuncBegin 位置上的 5 个字节 备份到 oldBytes中
    // 3. 构造跳转指令
    BuildNewInst(newBytes, (PROC)FuncBeginPos, (PROC)InlineHook, 5);
    // 4. 修改内存页保护属性，有可写权限
    g_IsHook = SetupHook(FuncBeginPos, newBytes, 9);
}

__declspec(naked)  // 声明为裸函数，编译器不会为该函数 产生保存栈帧以及开辟栈空间的指令
void  InlineHook()
{
    __asm
    {
        pop eax  // 由于上一条指令为 call，栈顶为函数返回地址，为了能够正确返回call指令的下一条指令的位置，因此先pop 存储到 eax
        push ebp
        mov ebp, esp
        sub esp, 0xCC

        // ----------------- 操作指令---------------
        mov dword ptr ds:[ecx],6666  // 最好是用 汇编，而不是 C语言的变量


        // -------------------------------------------
    }



    __asm
    {
        push eax  // 让 retn 指令能够恢复上个函数的返回地址
        retn
    }
}

void BuildNewInst(BYTE* newBytes, PROC dstfuncAddr, PROC InlineHookAddr, SIZE_T size)
{
    std::cout << "dstfuncAddr: " << dstfuncAddr << std::endl;
    std::cout << "InlineHookAddr: " << InlineHookAddr << std::endl;
    
    *newBytes = 0xE8;  // 近跳转, 这里是不是 E8或者 E9 都可以？
    *(DWORD*)(newBytes + 1) = ((DWORD)InlineHookAddr - (DWORD)dstfuncAddr - size);
    
    newBytes[5] = 0x90;
    newBytes[6] = 0x90;
    newBytes[7] = 0x90;
    newBytes[8] = 0x90;
}