// BotTry_v0.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>

void printField(int x, int y, void *address, HANDLE phandle);

int main()
{
	LPWORD  address = (LPWORD)0x01005340;
	DWORD pid; //This will store our Process ID, used to read/write into the memory
	HWND hwnd; //Finally a handle to our window
	int value = 666;
	int i = 0;
	int *buffaddr = 0;
	hwnd = FindWindow(NULL, L"Minesweeper"); //Finds the Window called "Minesweeper"
	if (!hwnd) //If none, display an error
	{
		std::cout << "Window not found! >:(\n";
	}
	else {
		std::cout << "Window founded >:)\n";
	}
	GetWindowThreadProcessId(hwnd, &pid);
	std::cout << pid << std::endl;
	//01005340
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (!phandle)
	{
		std::cout << "Could not get handle!\n";
	}
	else
	{
		int x = 0, y = 0;
		char buff = 0;
		i = 1;
		do {
			y++;
			ReadProcessMemory(phandle, (LPWORD)((int)address + 0x20 * i), &buff, sizeof(buff), 0);
			i++;
		} while (buff == 0x10);
		i = 1;
		do {
			x++;
			ReadProcessMemory(phandle, (LPWORD)((int)address + i), &buff, sizeof(buff), 0);
			i++;
		} while (buff == 0x10);
		std::cout << "Detected field size:\n";
		std::cout << "Width: " << x << std::endl;
		std::cout << "Height: " << y << std::endl;
		std::cout << "\nRemember! Before you didnt click a cell of field, mine places unknown\n";
		do {
			printField(x, y, address, phandle);
			std::cout << "To refresh field,  press any key\n";
		} while (std::cin.get());
	}
}

void printField(int x, int y, void *address, HANDLE phandle) {
	for (int i = 0; i < y - 2; i++) {
		for (int j = 0; j < x - 2; j++)
		{
			int buff = 0;
			ReadProcessMemory(phandle, (LPWORD)((int)address + (i + 1) * 0x20 + (j + 1)), &buff, 1, 0);
			if ((buff / 0x80) == 1)
				std::cout << "X ";
			else
				std::cout << "0 ";
		}
		std::cout << std::endl;
	}
}