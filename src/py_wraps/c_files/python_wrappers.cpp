#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <vector>
#include <string>
#include <iostream>

#include "python_wrappers.hpp"


using namespace std;
/**
Calls a python function (because I'd have to rewrite the API and no one on the internet apparently wants a CPP YouTube API WHYYYYYYY)
Returns a list of playlists
 */
vector<string> get_playlists() {
    vector<string> result;

    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule("wrappers");
    
    if (!pModule) {
        cout << "Could not find module";
        return result;
    }

    PyObject* pArgs = PyTuple_New(0); // No args
    PyObject* pFunc = PyObject_GetAttrString(pModule, "get_playlist");

    // Call function
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

    if (!PyList_Check(pValue)) { cout << "Not of type list\n"; return result;}

    Py_ssize_t n = PyList_Size(pValue);
    
    for (Py_ssize_t i = 0; i < n - 1; i++) {
        // I should technically check the code here, but oh well.
        result.push_back(string(PyUnicode_AsUTF8(PyList_GetItem(pValue, i))));
    }


    return result;

    

}