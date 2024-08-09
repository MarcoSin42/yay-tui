#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <vector>
#include <string>
#include <stdexcept>

#include "python_wrappers.hpp"
#include "music_structs.hpp"

using namespace std;

namespace pyapi {

vector<playlist_info> get_playlists() {
    vector<playlist_info> result;
    PyObject *pModule, *pArgs, *pFunc, *pValue;

    if (!Py_IsInitialized()) 
        throw runtime_error("Python was not initialized prior to calling 'get_playlists'");
    
    pModule = PyImport_ImportModule("wrappers");
    if (!pModule) 
        throw runtime_error("wrappers module was unable to be imported");

    pArgs = PyTuple_New(0);

    pFunc = PyObject_GetAttrString(pModule, "get_playlists");
    pValue = PyObject_CallObject(pFunc, pArgs); // Call

    if (!PyList_Check(pValue)) 
        throw new runtime_error("Function did not return type 'list'");

    Py_ssize_t n = PyList_Size(pValue);
    for (Py_ssize_t i = 0; i < n; i++) {
        result.push_back(playlist_info{
            .title    = string(PyUnicode_AsUTF8(PyTuple_GetItem(PyList_GetItem(pValue, i), 0))),
            .id       = string(PyUnicode_AsUTF8(PyTuple_GetItem(PyList_GetItem(pValue, i), 1))),
        });
    }

    // Cleanup
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
    Py_DECREF(pValue);

    return result;
}


vector<track_info> get_songs(string playlist_id) {
    vector<track_info> result;
    PyObject *pModule, *pFunc, *pValue, *pPlayID;

    if (!Py_IsInitialized()) 
        throw runtime_error("Python was not initialized prior to calling 'get_songs'");
    
    pModule = PyImport_ImportModule("wrappers");
    if (!pModule) 
        throw runtime_error("wrappers module was unable to be imported");

    pPlayID = PyUnicode_FromString(playlist_id.c_str());

    pFunc = PyObject_GetAttrString(pModule, "get_songs");

    if (!PyCallable_Check(pFunc))
        throw runtime_error("'get_songs' is not callable");

    pValue = PyObject_CallOneArg(pFunc, pPlayID);

    if (!PyList_Check(pValue)) 
        throw runtime_error("Function did not return type 'list'");

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
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
    Py_DECREF(pValue);
    Py_DECREF(pPlayID);
    Py_DECREF(pTrackTup);

    return result;
}
}
