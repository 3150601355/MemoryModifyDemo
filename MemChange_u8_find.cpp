/*
相关视频讲解：https://www.bilibili.com/video/BV1ud4y1B7rW/?vd_source=a8f594abdedb71183aba33bcd425efd9
作者：偶尔有点小迷糊

这是一个demo，用来查找【32位】程序中的【无符号字节型】变量的地址。

*/

#include <iostream>
#include <set>
#include <Windows.h>

using namespace std;
 
#define GB_SIZE			1024 * 1024 * 1024  		// 1GB
#define PAGE_SIZE		4 * 1024    							// 4KB

set<unsigned char *>	gSetAddr;							// 存储可能的变量地址
HANDLE g_hProcess;												// 目标进程的句柄
 

void GlobalScan(unsigned char val_u8) {
	unsigned char arBytes[PAGE_SIZE];					// 4KB的数据缓冲区，用来存放一个内存页
	// 对于演示用的32位老游戏，搜索低2GB的内存地址就够了
	for (unsigned long dwStartAddr = 0; dwStartAddr < 2*GB_SIZE; dwStartAddr += PAGE_SIZE) {
		if ( ReadProcessMemory(g_hProcess, (LPCVOID)dwStartAddr, arBytes, sizeof(arBytes), NULL) ){
			// 地毯式所搜。这部分代码可以优化以提高速度
			for (int i = 0; i < PAGE_SIZE; i++) {
				if ( arBytes[i] == val_u8)
					gSetAddr.insert((unsigned char *) (dwStartAddr + i) );
			}//for (int i 
		}
	} // for (unsigned long 
}


void RescanList(unsigned char val_u8) {
	unsigned char valMemory;
	
	// 看看集合里的哪个地址的值变成最新的值了，不变的就删掉
	for(set<unsigned char *>::iterator it = gSetAddr.begin(); it != gSetAddr.end(); ){
		ReadProcessMemory(g_hProcess, (LPVOID)*it, &valMemory, sizeof(valMemory), NULL);
		if (valMemory == val_u8)
			it++;
		else
			gSetAddr.erase(it++);
	}
}


void ShowList() {
	cout << "可能的地址为：" ;
	for(set<unsigned char *>::iterator it = gSetAddr.begin(); it != gSetAddr.end(); it++){
		cout << (void *)*it << " ";
	}
	cout << endl << endl;
}


int main() {
	// 1）由进程PID得到进程句柄
	cout << "1）请输入目标进程的PID：";
	DWORD dwTargetPID;
	cin >> dwTargetPID;
	g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTargetPID);
 
	// 2）输入要扫描的值
	int iVal;
	cout << "2）输入要扫描的值：";
	cin >> iVal;
 
	// 3）进行第一次查找
	GlobalScan(iVal);
 
	// 4）打印初步地址扫描的结果（太多就不打印了）
	if (gSetAddr.size() < 200)
		ShowList();
	else
		cout << "3）初步扫描得到" << gSetAddr.size() << "个可能的地址" << endl;
 
	// 5）循环查找，直至得到唯一地址
	while (gSetAddr.size() > 1) {
		cout << "4）再次输入要扫描的值：";
		cin >> iVal;
		RescanList(iVal);
		ShowList();
	}

	// 6）关闭进程
	CloseHandle(g_hProcess);  

	return 0;
}

