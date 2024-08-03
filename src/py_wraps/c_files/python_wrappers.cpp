#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <vector>
#include <string>
#include <iostream>
#include <array>

#include "python_wrappers.hpp"


using namespace std;


vector<playlist_info> get_playlists() {
    vector<playlist_info> result;
    
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule("wrappers");
    
    if (!pModule) return result;

    PyObject* pArgs = PyTuple_New(0);

    PyObject* pFunc = PyObject_GetAttrString(pModule, "get_playlists");
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs); // Call

    if (!PyList_Check(pValue)) { cout << "Not of type list\n"; return result;}

    Py_ssize_t n = PyList_Size(pValue);
    for (Py_ssize_t i = 0; i < n; i++) {
        result.push_back(playlist_info{
            .id    = string(PyUnicode_AsUTF8(PyTuple_GetItem(PyList_GetItem(pValue, i), 0))),
            .title = string(PyUnicode_AsUTF8(PyTuple_GetItem(PyList_GetItem(pValue, i), 1))),
        });
    }

    // Cleanup
    Py_FinalizeEx();

    return result;
}


vector<string> get_songs(string playlist_id) {
    vector<string> result;
    
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule("wrappers");
    
    if (!pModule) {
        cout << "Could not find module";
        return result;
    }

    PyObject* pArgs = PyTuple_New(0); // No args
    PyObject* pFunc = PyObject_GetAttrString(pModule, "get_playlists");

    // Call function
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

    if (!PyList_Check(pValue)) { cout << "Not of type list\n"; return result;}

    Py_ssize_t n = PyList_Size(pValue);
    for (Py_ssize_t i = 0; i < n - 1; i++) {
        // I should technically check the code here, but oh well.
        result.push_back(string(PyUnicode_AsUTF8(PyList_GetItem(pValue, i))));
    }
    // Cleanup
    Py_FinalizeEx();

    return result;
}

