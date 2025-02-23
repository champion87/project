#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf_algo.h"







PyObject * build_py_FloatList(double * cords, size_t len) {
    PyObject * ans;
    
    ans = PyList_New(len);
    if (!ans) {
        return PyErr_NoMemory(); // Raise a memory error if list creation fails
    }
    
    // Fill the list with the given double value
    for (size_t i = 0; i < len; i++) {
        PyObject* py_float = PyFloat_FromDouble(cords[i]);
        if (!py_float) {
            Py_DECREF(ans);
            return PyErr_NoMemory(); // Raise a memory error if float creation fails
        }
        PyList_SetItem(ans, i, py_float);
    }

    return ans;
}


PyObject * build_list_list_float(matrix_t mat) {
    PyObject * ans;
    ans = PyList_New(mat.height);
    if (!ans) {
        return PyErr_NoMemory(); // Raise a memory error if list creation fails
    }
    for (size_t i = 0; i < mat.height; i++) {
        PyObject * row = build_py_FloatList(mat.data[i], mat.width);
        if (!row) {
            Py_DECREF(ans);
            return row;
        }

        PyList_SetItem(ans, i, row);
    }
    return ans;
}

matrix_t convert_to_matrix(PyObject* float_matrix) {
    PyObject *list_of_lists = NULL;
    if (!PyArg_Parse(float_matrix, "O", &list_of_lists)) {
        return ERR_MATRIX;
    }

    if (!PyList_Check(list_of_lists)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list of lists");
        return ERR_MATRIX;
    }

    matrix_t ans = (matrix_t){ 0 };
    ans.height = PyList_Size(list_of_lists);

    // verify input correctness
    for (size_t i = 0; i < ans.height; i++) {
        PyObject *inner_list = PyList_GetItem(list_of_lists, i);
        if (!PyList_Check(inner_list)) {
            PyErr_SetString(PyExc_TypeError, "Inner elements must be lists");
            return ERR_MATRIX;
        }
        Py_ssize_t inner_size = PyList_Size(inner_list);
        if (i == 0) {
            ans.width = inner_size;
        }
        else if (ans.width != (size_t)inner_size) {
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

    for (size_t i = 0; i < ans.height; i++) {
        PyObject *inner_list = PyList_GetItem(list_of_lists, i);
        ans.data[i] = ans.raw_data + (ans.width * i);
        for (size_t j = 0; j < ans.width; j++) {
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

static PyObject * c_sym(PyObject * self, PyObject * args) {
    PyObject * py_matrix = NULL;

    if(!PyArg_Parse(args, "O", &py_matrix)) {
        PyErr_SetString(PyExc_TypeError, "failure parsing parameters");
        return NULL;
    }
    matrix_t c_matrix = convert_to_matrix(py_matrix);
    if (c_matrix.data == NULL) {
        return NULL;
    }

    matrix_t c_result = sym(c_matrix);
    free_matrix(c_matrix);
    if (c_result.data == NULL) {
        PyErr_SetString(PyExc_ValueError, "error calculatting the Similarity Matrix");
        return NULL;
    }
    PyObject * py_result = build_list_list_float(c_result);
    free_matrix(c_result);
    return py_result;
}

static PyObject * c_ddg(PyObject * self, PyObject * args) {
    PyObject * py_matrix = NULL;

    if(!PyArg_Parse(args, "O", &py_matrix)) {
        PyErr_SetString(PyExc_TypeError, "failure parsing parameters");
        return NULL;
    }
    matrix_t c_matrix = convert_to_matrix(py_matrix);
    if (c_matrix.data == NULL) {
        return NULL;
    }

    matrix_t c_result = ddg(c_matrix);
    free_matrix(c_matrix);
    if (c_result.data == NULL) {
        PyErr_SetString(PyExc_ValueError, "error calculatting the diagonal degree Matrix");
        return NULL;
    }
    PyObject * py_result = build_list_list_float(c_result);
    free_matrix(c_result);
    return py_result;
}

static PyObject * c_norm(PyObject * self, PyObject * args) {
    PyObject * py_matrix = NULL;

    if(!PyArg_Parse(args, "O", &py_matrix)) {
        PyErr_SetString(PyExc_TypeError, "failure parsing parameters");
        return NULL;
    }
    matrix_t c_matrix = convert_to_matrix(py_matrix);
    if (c_matrix.data == NULL) {
        return NULL;
    }

    matrix_t c_result = norm(c_matrix);
    free_matrix(c_matrix);
    if (c_result.data == NULL) {
        PyErr_SetString(PyExc_ValueError, "error calculatting the normalized similarity matrix");
        return NULL;
    }
    PyObject * py_result = build_list_list_float(c_result);
    free_matrix(c_result);
    return py_result;
}

static PyObject * c_symnmf(PyObject * self, PyObject * args) {
    PyObject * py_H = NULL;
    PyObject * py_W = NULL;
    double eps = 0;
    size_t max_iter = 0;
    double beta = 0;
    if(!PyArg_ParseTuple(args, "OOdnd", &py_H, &py_W, &eps, &max_iter, &beta)) {
        PyErr_SetString(PyExc_TypeError, "failure parsing parameters");
        return NULL;
    }
    matrix_t c_H = convert_to_matrix(py_H);
    if (IS_ERR_MAT(c_H)) {
        return NULL;
    }
    matrix_t c_W = convert_to_matrix(py_W);
    if (IS_ERR_MAT(c_W)) {
        free_matrix(c_H);
        return NULL;
    }
    matrix_t ans = symnmf(c_H, c_W, eps, max_iter, beta);
    if (IS_ERR_MAT(ans)) {
        PyErr_SetString(PyExc_ValueError, "failure performing the symnmf algorithm for clustering");
    }
    free_matrix(c_H);
    free_matrix(c_W);
    return build_list_list_float(ans);
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