#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H
#include <vector>
#include <string>

using namespace std;
vector<string> call_pyfunc(const char* py_func_name);
vector<string> get_playlist();


#endif