//防止头文件重复包含

#pragma once
#include <stdio.h>

//用一个结构体表示报头
struct MyTCPHeader
{
	//报头长度
	int data_len_;
};
