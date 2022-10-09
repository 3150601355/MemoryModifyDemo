/*
�����Ƶ���⣺https://www.bilibili.com/video/BV1ud4y1B7rW/?vd_source=a8f594abdedb71183aba33bcd425efd9
���ߣ�ż���е�С�Ժ�

����һ��demo���������ҡ�32λ�������еġ��޷����ֽ��͡������ĵ�ַ��

*/

#include <iostream>
#include <set>
#include <Windows.h>

using namespace std;
 
#define GB_SIZE			1024 * 1024 * 1024  		// 1GB
#define PAGE_SIZE		4 * 1024    							// 4KB

set<unsigned char *>	gSetAddr;							// �洢���ܵı�����ַ
HANDLE g_hProcess;												// Ŀ����̵ľ��
 

void GlobalScan(unsigned char val_u8) {
	unsigned char arBytes[PAGE_SIZE];					// 4KB�����ݻ��������������һ���ڴ�ҳ
	// ������ʾ�õ�32λ����Ϸ��������2GB���ڴ��ַ�͹���
	for (unsigned long dwStartAddr = 0; dwStartAddr < 2*GB_SIZE; dwStartAddr += PAGE_SIZE) {
		if ( ReadProcessMemory(g_hProcess, (LPCVOID)dwStartAddr, arBytes, sizeof(arBytes), NULL) ){
			// ��̺ʽ���ѡ��ⲿ�ִ�������Ż�������ٶ�
			for (int i = 0; i < PAGE_SIZE; i++) {
				if ( arBytes[i] == val_u8)
					gSetAddr.insert((unsigned char *) (dwStartAddr + i) );
			}//for (int i 
		}
	} // for (unsigned long 
}


void RescanList(unsigned char val_u8) {
	unsigned char valMemory;
	
	// ������������ĸ���ַ��ֵ������µ�ֵ�ˣ�����ľ�ɾ��
	for(set<unsigned char *>::iterator it = gSetAddr.begin(); it != gSetAddr.end(); ){
		ReadProcessMemory(g_hProcess, (LPVOID)*it, &valMemory, sizeof(valMemory), NULL);
		if (valMemory == val_u8)
			it++;
		else
			gSetAddr.erase(it++);
	}
}


void ShowList() {
	cout << "���ܵĵ�ַΪ��" ;
	for(set<unsigned char *>::iterator it = gSetAddr.begin(); it != gSetAddr.end(); it++){
		cout << (void *)*it << " ";
	}
	cout << endl << endl;
}


int main() {
	// 1���ɽ���PID�õ����̾��
	cout << "1��������Ŀ����̵�PID��";
	DWORD dwTargetPID;
	cin >> dwTargetPID;
	g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTargetPID);
 
	// 2������Ҫɨ���ֵ
	int iVal;
	cout << "2������Ҫɨ���ֵ��";
	cin >> iVal;
 
	// 3�����е�һ�β���
	GlobalScan(iVal);
 
	// 4����ӡ������ַɨ��Ľ����̫��Ͳ���ӡ�ˣ�
	if (gSetAddr.size() < 200)
		ShowList();
	else
		cout << "3������ɨ��õ�" << gSetAddr.size() << "�����ܵĵ�ַ" << endl;
 
	// 5��ѭ�����ң�ֱ���õ�Ψһ��ַ
	while (gSetAddr.size() > 1) {
		cout << "4���ٴ�����Ҫɨ���ֵ��";
		cin >> iVal;
		RescanList(iVal);
		ShowList();
	}

	// 6���رս���
	CloseHandle(g_hProcess);  

	return 0;
}

