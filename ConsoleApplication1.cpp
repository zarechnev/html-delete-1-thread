#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> arr;
vector<string> ans[10];

struct Params {
	unsigned int f_index, count, thread_index;
};

DWORD WINAPI thread_func(void *p) {
	unsigned int first_index = ((Params *)p)->f_index;
	unsigned int index_counts = ((Params *)p)->count;
	unsigned int thread_index = ((Params *)p)->thread_index;

	for (unsigned int k = first_index, j = 0; k < first_index + index_counts; ++k, j++) {
		string text = "";
		char ch = '\0';
		bool b = true;
		for (unsigned int i = 0; i < arr[k].length(); i++) {
			ch = arr[k][i];
			if (ch == '<') b = false;
			if (ch == '>') {
				b = true;
				continue;
			}
			if (b) text += ch;
		}
		ans[thread_index].push_back(text);
	}
	return 0;
}

int main(int argc, char* argv[]) {
	int t = clock();
	ifstream html_file;
	ofstream out_file;
	unsigned int thread_count=2;
	string str_tmp;
	
	if (argc > 1) {
		html_file.open(argv[1]);
		if (!html_file.is_open()) {
			cout << "HTML file can not be opened." << endl;
			return -1;
		}
		if (argc > 2) thread_count = (stoi(argv[2]) > 10 ? 10 : stoi(argv[2]));
	}
	
	else {
		cout << "Enter HTML file, please." << endl;
		system("pause");
		return -2;
	}
	
	out_file.open("out.txt", ios::trunc);

	while (!html_file.eof()) {
		getline(html_file, str_tmp);
		arr.push_back(str_tmp);
	}

	HANDLE *threadProducer = new HANDLE[thread_count]; // threads
	Params *thread_args = new Params[thread_count]; // thread params

	for (unsigned int k = 0; k < thread_count; k++) {
		unsigned int count_el_one_thr = arr.size() / thread_count;
		thread_args[k].f_index = k * count_el_one_thr;
		thread_args[k].thread_index = k;
		thread_args[k].count = (count_el_one_thr < (arr.size() - count_el_one_thr * k) ? count_el_one_thr : (arr.size() - count_el_one_thr * k));
		threadProducer[k] = CreateThread(0, 0, &thread_func, &thread_args[k], 0, 0);
	}

	WaitForMultipleObjects(thread_count, threadProducer, true, INFINITE);

	for (unsigned int i = 0; i < thread_count; i++) {
		for (unsigned int k = 0; k < ans[i].size(); k++)
			out_file << ans[i][k] << endl;
	}

	html_file.close();
	out_file.close();

	cout << clock() - t << " - exit." << endl;

	cout << "threads count: " << thread_count << endl;

	system("pause");

	arr.clear();
	for (unsigned int i = 0; i < ans[i].size(); i++) ans[i].clear();

	system("pause");

	return 0;
}