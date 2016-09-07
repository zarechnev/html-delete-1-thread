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

vector<string> arr; // mass of html file

struct Params {
	unsigned int f_index, count; // first index and index counts
};

DWORD WINAPI thread_func(void *p) {
	unsigned int first_index = ((Params *)p)->f_index;
	unsigned int index_counts = ((Params *)p)->count;
	for (unsigned int k = first_index; k < first_index + index_counts; k++) {

		//cycle_BEGIN
		string text = "";
		char ch = '\0';
		bool b = false;

		for (unsigned int i = 0; i < arr[k].length(); i++) {
			if (ch == '>') b = true;
			if (b) {
				if (ch != '<' && ch != '>') text += ch;
			}
			if (ch == '<') b = false;
		}
		arr[k] = text;
		//cycle_END
	}

	return 0;
}

int main(int argc, char* argv[]) {
	ifstream html_file("F:\\vs\\лр_1-многопоточ\\html-delete-multythread\\Debug\\1.txt");
	unsigned int thread_count = 2;
	
	//if (argc > 1) {
	//	html_file.open(argv[1]);
	//	if (!html_file.is_open()) {
	//		cout << "HTML file can not be opened." << endl;
	//		return -1;
	//	}
	//	if (argv[2]) thread_count = int(argv[2]);
	//	else thread_count = 2;
	//}
	//else {
	//	cout << "Enter HTML file, please." << endl;
	//	system("pause");
	//	return -2;
	//}
	
	string str;

	ofstream out_file("out.txt");

	while (!html_file.eof()) {
		getline(html_file, str);
		arr.push_back(str);
	}

	//threads_count <= strings in file
	if (thread_count > arr.size()) thread_count = arr.size();
	
	HANDLE *threadProducer = new HANDLE[thread_count]; // threads
	Params *thread_args = new Params[thread_count]; // thread params

	for (unsigned int k = 0; k < thread_count; k++) {
		//cout << "thread" << endl;
		int count_el_one_thr = arr.size() / thread_count;
		//first element index
		thread_args[k].f_index = k * count_el_one_thr;
		//elements count
		thread_args[k].count = count_el_one_thr;
		threadProducer[k] = CreateThread(0, 0, &thread_func, &thread_args[k], 0, 0);
	}

	WaitForMultipleObjects(thread_count, threadProducer, true, INFINITE);

	system("pause");

	for (unsigned int i = 0; i < arr.size(); i++) {
		out_file << arr[i] << endl;
	}
		
	html_file.close();
	out_file.close();

	return 0;
}