// BotTry_v0.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>

#define FIELD_PLACE_IN_MEMORY 0x01005340 //top left corner
#define FIELD_STEP_VERTICAL 0x20
#define FIELD_STEP_HORIZONTAL 0x1
#define BOMB_MASK 0x80
#define MARK_MASK 0x0E
#define BOMB_MARK "X"
#define FIELD_MARK "0"

void printField(int x, int y, void *address, HANDLE phandle);
void placeMarks(int x, int y, void *address, HANDLE phandle);

int main()
{
	DWORD pid; //This will store our Process ID, used to read/write into the memory
	HWND hwnd; //Finally a handle to our window
	int value = 666; // some random number to initialize
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
			ReadProcessMemory(phandle, (LPWORD)(FIELD_PLACE_IN_MEMORY + FIELD_STEP_VERTICAL * i), &buff, sizeof(buff), 0);
			i++;
		} while (buff == 0x10);
		i = 1;
		do {
			x++;
			ReadProcessMemory(phandle, (LPWORD)(FIELD_PLACE_IN_MEMORY + FIELD_STEP_HORIZONTAL * i), &buff, sizeof(buff), 0);
			i++;
		} while (buff == 0x10);
		std::cout << "Detected field size:\n";
		std::cout << "Width: " << x << std::endl;
		std::cout << "Height: " << y << std::endl;
		std::cout << "\nRemember! Before you didnt click a cell of field, mine places unknown\n";
		char comm = '\0';
		do {
			bool flag = false;
			switch (comm) {
				case 'q': flag = true; break;
				case 'p': placeMarks(x, y, (LPWORD)FIELD_PLACE_IN_MEMORY, phandle); break;
			}
			if (flag)
				break;
			printField(x, y, (LPWORD)FIELD_PLACE_IN_MEMORY, phandle);
			std::cout << "To refresh field,  press any key\n";
			std::cout << "If you want quit, press q\n";
			std::cout << "To place marks on field of minesweeper, press p\n";
			std::cin.get(comm);
		} while (comm);
	}
}

void printField(int x, int y, void *address, HANDLE phandle) {
	for (int i = 0; i < y - 2; i++) {
		for (int j = 0; j < x - 2; j++)
		{
			int buff = 0;
			ReadProcessMemory(phandle, (LPWORD)((int)address + (i + 1) * FIELD_STEP_VERTICAL 
										+ FIELD_STEP_HORIZONTAL * (j + 1)), &buff, 1, 0);
			if (buff & BOMB_MASK)
				std::cout << BOMB_MARK <<" ";
			else
				std::cout << FIELD_MARK << " ";
		}
		std::cout << std::endl;
	}
}

void placeMarks(int x, int y, void *address, HANDLE phandle) {
	for (int i = 0; i < y - 2; i++) {
		for (int j = 0; j < x - 2; j++)
		{
			int buff = 0;
			ReadProcessMemory(phandle, (LPWORD)((int)address + (i + 1) * FIELD_STEP_VERTICAL
						+ FIELD_STEP_HORIZONTAL * (j + 1)), &buff, 1, 0);
			if (buff & BOMB_MASK) {
				buff = BOMB_MASK | MARK_MASK;
				WriteProcessMemory(phandle, (LPWORD)((int)address + (i + 1) * FIELD_STEP_VERTICAL
					+ FIELD_STEP_HORIZONTAL * (j + 1)), &buff, 1, 0);
			}
		}
		std::cout << std::endl;
	}
}