/*
代码的视频讲解：https://www.bilibili.com/video/BV1ud4y1B7rW/?vd_source=a8f594abdedb71183aba33bcd425efd9
作者：偶尔有点小迷糊

这是一个demo，用来改写【32位】程序中的【无符号字节型】变量的地址。

*/


#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
	// 1）由进程PID得到进程句柄
	cout << "1）请输入目标进程的ID（PID）：";
	DWORD dwTargetPID;
	cin >> dwTargetPID;
	
	HANDLE  hProcess = OpenProcess(	PROCESS_ALL_ACCESS, 
															FALSE, 
															dwTargetPID);
 
	// 2）输入内存地址
	DWORD dwAddr;
	cout << "2）输入你要修改的变量地址（16进制）：";
	cin >> hex >> dwAddr;

	// 3）输入新的数值
	DWORD dwNewVal;
	cout << "3）将该地址处的值修改为（10进制）：";
	cin >> dec >> dwNewVal;
 
	// 4）写入新数值到内存地址
	if( WriteProcessMemory(hProcess, (LPVOID)dwAddr, &dwNewVal, sizeof(unsigned char), NULL) )
		cout << "修改内存成功!" << endl;

	return 0;
}

