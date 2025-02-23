#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf_algo.h"





static PyObject * c_sym(PyObject * self, PyObject * args) {

}

static PyObject * c_ddg(PyObject * self, PyObject * args) {

}

static PyObject * c_norm(PyObject * self, PyObject * args) {

}

static PyObject * c_symnmf(PyObject * self, PyObject * args) {

}

matrix_t convert_to_matrix(PyObject* float_matrix) {
    PyObject *list_of_lists = NULL;
    if (!PyArg_ParseTuple(float_matrix, "O", &list_of_lists)) {
        return ERR_MATRIX;
    }

    if (!PyList_Check(list_of_lists)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list of lists");
        return ERR_MATRIX;
    }

    matrix_t ans = (matrix_t){ 0 };
    ans.height = PyList_Size(list_of_lists);

    // verify input correctness
    for (Py_ssize_t i = 0; i < ans.height; i++) {
        PyObject *inner_list = PyList_GetItem(list_of_lists, i);
        if (!PyList_Check(inner_list)) {
            PyErr_SetString(PyExc_TypeError, "Inner elements must be lists");
            return ERR_MATRIX;
        }
        Py_ssize_t inner_size = PyList_Size(inner_list);
        if (i == 0) {
            ans.width = inner_size;
        }
        else if (ans.width != inner_size) {
            PyErr_SetString(PyExc_TypeError, "Inner elements must be lists of the same length");
            return ERR_MATRIX; 
        }
    }

    ans.data = (double **)calloc(ans.height, sizeof(*ans.data));
    if (!ans.data) {
        PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
        return ERR_MATRIX;
    }

    ans.raw_data = (double *)calloc(ans.height * ans.width, sizeof(*ans.raw_data));
    if (!ans.raw_data) {
        PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
        free(ans.data);
        return ERR_MATRIX;
    }

    double *data_ptr =  ans.raw_data;
    for (Py_ssize_t i = 0; i < ans.height; i++) {
        PyObject *inner_list = PyList_GetItem(list_of_lists, i);
        ans.data[i] = ans.raw_data + (ans.width * i);
        for (Py_ssize_t j = 0; j < ans.width; j++) {
            PyObject *item = PyList_GetItem(inner_list, j);
            if (!PyFloat_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "All elements must be floats");
                free(ans.data);
                free(ans.raw_data);
                return ERR_MATRIX;
            }
            ans.data[i][j] = PyFloat_AsDouble(item);
        }
    }

    return ans;
}


static PyMethodDef symnmfMethods[] = {
    {
        "sym",
        (PyCFunction) c_sym,
        METH_VARARGS,
        PyDoc_STR("calculate the Similarity Matrix")
    },
    {
        "ddg",
        (PyCFunction) c_ddg,
        METH_VARARGS,
        PyDoc_STR("calculate the diagonal degree Matrix")
    },
    {
        "norm",
        (PyCFunction) c_norm,
        METH_VARARGS,
        PyDoc_STR("calculate the normalized similarity matrix")
    },
    {
        "symnmf",
        (PyCFunction) c_symnmf,
        METH_VARARGS,
        PyDoc_STR("perform the symnmf algorithm for clustering")
    },
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf", /* name of module */
    "TODO", /* TODO */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    symnmfMethods /* the PyMethodDef array from before containing the methods of the extension */
};


PyMODINIT_FUNC PyInit_symnmf(void)
{
    PyObject *py_module;
    py_module = PyModule_Create(&symnmfmodule);
    if (!py_module) {
        PyErr_SetString(PyExc_RuntimeError, "error initailizing symnmf module");
        return NULL;
    }
    return py_module;
}