/*
�������Ƶ���⣺https://www.bilibili.com/video/BV1ud4y1B7rW/?vd_source=a8f594abdedb71183aba33bcd425efd9
���ߣ�ż���е�С�Ժ�

����һ��demo��������д��32λ�������еġ��޷����ֽ��͡������ĵ�ַ��

*/


#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
	// 1���ɽ���PID�õ����̾��
	cout << "1��������Ŀ����̵�ID��PID����";
	DWORD dwTargetPID;
	cin >> dwTargetPID;
	
	HANDLE  hProcess = OpenProcess(	PROCESS_ALL_ACCESS, 
															FALSE, 
															dwTargetPID);
 
	// 2�������ڴ��ַ
	DWORD dwAddr;
	cout << "2��������Ҫ�޸ĵı�����ַ��16���ƣ���";
	cin >> hex >> dwAddr;

	// 3�������µ���ֵ
	DWORD dwNewVal;
	cout << "3�����õ�ַ����ֵ�޸�Ϊ��10���ƣ���";
	cin >> dec >> dwNewVal;
 
	// 4��д������ֵ���ڴ��ַ
	if( WriteProcessMemory(hProcess, (LPVOID)dwAddr, &dwNewVal, sizeof(unsigned char), NULL) )
		cout << "�޸��ڴ�ɹ�!" << endl;

	return 0;
}

