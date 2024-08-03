#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <vector>
#include <string>
#include <iostream>
#include <array>

#include "python_wrappers.hpp"
#include "music_structs.hpp"

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


vector<track_info> get_songs(string playlist_id) {
    vector<track_info> result;
    
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule("wrappers");
    
    if (!pModule) {
        cout << "Could not find module";
        return result;
    }

    PyObject* pArgs = PyTuple_New(1); // 1 Argument, PlaylistID from which we want to get the song from
    PyObject* pPlayID = PyBytes_FromString(playlist_id.c_str());
    PyTuple_SetItem(pArgs, 0, pPlayID);

    PyObject* pFunc = PyObject_GetAttrString(pModule, "get_songs");

    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

    if (!PyList_Check(pValue)) { cout << "Not of type list\n"; return result;}

    Py_ssize_t n = PyList_Size(pValue);
    PyObject* pTrackTup;

    for (Py_ssize_t i = 0; i < n; i++) {
        pTrackTup = PyList_GetItem(pValue, i);
        result.push_back(track_info{
            .id     = string(PyUnicode_AsUTF8(PyTuple_GetItem(pTrackTup, 0))),
            .title  = string(PyUnicode_AsUTF8(PyTuple_GetItem(pTrackTup, 1))),
            .artist = string(PyUnicode_AsUTF8(PyTuple_GetItem(pTrackTup, 2))),
            .album  = string(PyUnicode_AsUTF8(PyTuple_GetItem(pTrackTup, 3))),
            .mins   = PyLong_AsSize_t(PyTuple_GetItem(pTrackTup, 4)),
            .secs   = PyLong_AsSize_t(PyTuple_GetItem(pTrackTup, 5)) 
        });
    }

    // Cleanup
    Py_FinalizeEx();
    return result;
}

