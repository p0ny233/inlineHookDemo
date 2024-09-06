#pragma once
#include <Windows.h>
#include <libloaderapi.h>
#include <iostream>

#define COUTBEGIN (std::cout << )
#define COUTEND (<< std::endl;)

struct Test
{
    int a;
    int b;
};
// 获取指定模块的基址
HMODULE GetModuleAddr(WCHAR* moduleName);

// 根据基址计算偏移量
BYTE* GetTargetFunc(BYTE*);

BOOL Hook();

// 构造跳转指令
void BuildNewInst(BYTE* newBytes, PROC dstfuncAddr, PROC InlineHookAddr, SIZE_T size);

void InlineHook();
