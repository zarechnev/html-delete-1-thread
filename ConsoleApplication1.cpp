#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <clocale>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Params {
	vector<string> *dataArray; // указатель на фрагмент массива
	int count, indexThread; // длина массива и индекс потока
	vector<string> *ans; // указатель на массив ответов
};

DWORD WINAPI thread_func(void *p) {
	vector<string> *data = ((Params *)p)->dataArray;
	int lenth = ((Params *)p)->count;
	
	vector<string> *ans = ((Params *)p)->ans;

	ans = data;

	delete(p);
	return 0;
	}

DWORD main(DWORD argc, char* argv[]) {
	setlocale(LC_ALL, "rus");
	ifstream html_file;
	int thread_count;

	if (argc > 1) {
		html_file.open(argv[1]);
		if (!html_file.is_open()) {
			cout << "‘айл не может быть открыт." << endl;
			return -1;
		}
		if (argv[2]) thread_count = int(argv[2]);
		else thread_count = 2;
	}
	else {
		cout << "”кажите html файл и количество потоков в качестве параметров." << endl;
		system("pause");
		return -2;
	}
	
	vector<string> arr;
	string str;
	ofstream out_file("out.txt");

	while (!html_file.eof()) {
		getline(html_file, str);
		arr.push_back(str);
	}

	cout << arr.size();

	HANDLE *threadProducer = new HANDLE[thread_count]; // потоки
	DWORD *pdwThreadId = new DWORD[thread_count];

	vector<string> *ans = new vector<string>[thread_count];


	for (int k = 1; k < thread_count; k++) {
		Params *thread_args = new Params;
		thread_args->dataArray = &arr[k*(arr.size() / thread_count)];
		if (k < thread_count - 1) thread_args->count = arr.size() / thread_count;
		else thread_args->count = arr.size() - (arr.size() / thread_count) * k;
		thread_args->indexThread = k;

		threadProducer[k] = CreateThread(0, 0, &thread_func, (void *)thread_args, 0, &pdwThreadId[k]);
	}
	
	html_file.close();
	out_file.close();

	return 0;
}