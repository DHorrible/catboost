/* File: mvnmodule.c
 * This file is auto-generated with f2py (version:1.26.4).
 * f2py is a Fortran to Python Interface Generator (FPIG), Second Edition,
 * written by Pearu Peterson <pearu@cens.ioc.ee>.
 * Generation date: Wed Nov 24 04:33:34 2021
 * Do not edit this file directly unless you know what you are doing!!!
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif /* PY_SSIZE_T_CLEAN */

/* Unconditionally included */
#include <Python.h>
#include <numpy/npy_os.h>

/*********************** See f2py2e/cfuncs.py: includes ***********************/
#include "fortranobject.h"
#include <math.h>

/**************** See f2py2e/rules.py: mod_rules['modulebody'] ****************/
static PyObject *mvn_error;
static PyObject *mvn_module;

/*********************** See f2py2e/cfuncs.py: typedefs ***********************/
/*need_typedefs*/

/****************** See f2py2e/cfuncs.py: typedefs_generated ******************/
/*need_typedefs_generated*/

/********************** See f2py2e/cfuncs.py: cppmacros **********************/

#if defined(PREPEND_FORTRAN)
#if defined(NO_APPEND_FORTRAN)
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) _##F
#else
#define F_FUNC(f,F) _##f
#endif
#else
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) _##F##_
#else
#define F_FUNC(f,F) _##f##_
#endif
#endif
#else
#if defined(NO_APPEND_FORTRAN)
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) F
#else
#define F_FUNC(f,F) f
#endif
#else
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) F##_
#else
#define F_FUNC(f,F) f##_
#endif
#endif
#endif
#if defined(UNDERSCORE_G77)
#define F_FUNC_US(f,F) F_FUNC(f##_,F##_)
#else
#define F_FUNC_US(f,F) F_FUNC(f,F)
#endif


/* See fortranobject.h for definitions. The macros here are provided for BC. */
#define rank f2py_rank
#define shape f2py_shape
#define fshape f2py_shape
#define len f2py_len
#define flen f2py_flen
#define slen f2py_slen
#define size f2py_size


#ifdef DEBUGCFUNCS
#define CFUNCSMESS(mess) fprintf(stderr,"debug-capi:"mess);
#define CFUNCSMESSPY(mess,obj) CFUNCSMESS(mess) \
    PyObject_Print((PyObject *)obj,stderr,Py_PRINT_RAW);\
    fprintf(stderr,"\n");
#else
#define CFUNCSMESS(mess)
#define CFUNCSMESSPY(mess,obj)
#endif


#ifndef max
#define max(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a < b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a < b) ? (a) : (b))
#endif


/************************ See f2py2e/cfuncs.py: cfuncs ************************/

static int
double_from_pyobj(double* v, PyObject *obj, const char *errmess)
{
    PyObject* tmp = NULL;
    if (PyFloat_Check(obj)) {
        *v = PyFloat_AsDouble(obj);
        return !(*v == -1.0 && PyErr_Occurred());
    }

    tmp = PyNumber_Float(obj);
    if (tmp) {
        *v = PyFloat_AsDouble(tmp);
        Py_DECREF(tmp);
        return !(*v == -1.0 && PyErr_Occurred());
    }

    if (PyComplex_Check(obj)) {
        PyErr_Clear();
        tmp = PyObject_GetAttrString(obj,"real");
    }
    else if (PyBytes_Check(obj) || PyUnicode_Check(obj)) {
        /*pass*/;
    }
    else if (PySequence_Check(obj)) {
        PyErr_Clear();
        tmp = PySequence_GetItem(obj, 0);
    }

    if (tmp) {
        if (double_from_pyobj(v,tmp,errmess)) {Py_DECREF(tmp); return 1;}
        Py_DECREF(tmp);
    }
    {
        PyObject* err = PyErr_Occurred();
        if (err==NULL) err = mvn_error;
        PyErr_SetString(err,errmess);
    }
    return 0;
}


static int
int_from_pyobj(int* v, PyObject *obj, const char *errmess)
{
    PyObject* tmp = NULL;

    if (PyLong_Check(obj)) {
        *v = Npy__PyLong_AsInt(obj);
        return !(*v == -1 && PyErr_Occurred());
    }

    tmp = PyNumber_Long(obj);
    if (tmp) {
        *v = Npy__PyLong_AsInt(tmp);
        Py_DECREF(tmp);
        return !(*v == -1 && PyErr_Occurred());
    }

    if (PyComplex_Check(obj)) {
        PyErr_Clear();
        tmp = PyObject_GetAttrString(obj,"real");
    }
    else if (PyBytes_Check(obj) || PyUnicode_Check(obj)) {
        /*pass*/;
    }
    else if (PySequence_Check(obj)) {
        PyErr_Clear();
        tmp = PySequence_GetItem(obj, 0);
    }

    if (tmp) {
        if (int_from_pyobj(v, tmp, errmess)) {
            Py_DECREF(tmp);
            return 1;
        }
        Py_DECREF(tmp);
    }

    {
        PyObject* err = PyErr_Occurred();
        if (err == NULL) {
            err = mvn_error;
        }
        PyErr_SetString(err, errmess);
    }
    return 0;
}


/********************* See f2py2e/cfuncs.py: userincludes *********************/
/*need_userincludes*/

/********************* See f2py2e/capi_rules.py: usercode *********************/


/* See f2py2e/rules.py */
extern void F_FUNC(mvnun,MVNUN)(int*,int*,double*,double*,double*,double*,int*,double*,double*,double*,int*);
extern void F_FUNC_US(mvnun_weighted,MVNUN_WEIGHTED)(int*,int*,double*,double*,double*,double*,double*,int*,double*,double*,double*,int*);
extern void F_FUNC(mvndst,MVNDST)(int*,double*,double*,int*,double*,int*,double*,double*,double*,double*,int*);
/*eof externroutines*/

/******************** See f2py2e/capi_rules.py: usercode1 ********************/


/******************* See f2py2e/cb_rules.py: buildcallback *******************/
/*need_callbacks*/

/*********************** See f2py2e/rules.py: buildapi ***********************/

/*********************************** mvnun ***********************************/
static char doc_f2py_rout_mvn_mvnun[] = "\
value,inform = mvnun(lower,upper,means,covar,[maxpts,abseps,releps])\n\nWrapper for ``mvnun``.\
\n\nParameters\n----------\n"
"lower : input rank-1 array('d') with bounds (d)\n"
"upper : input rank-1 array('d') with bounds (d)\n"
"means : input rank-2 array('d') with bounds (d,n)\n"
"covar : input rank-2 array('d') with bounds (d,d)\n"
"\nOther Parameters\n----------------\n"
"maxpts : input int, optional\n    Default: d*1000\n"
"abseps : input float, optional\n    Default: 1e-06\n"
"releps : input float, optional\n    Default: 1e-06\n"
"\nReturns\n-------\n"
"value : float\n"
"inform : int";
/* extern void F_FUNC(mvnun,MVNUN)(int*,int*,double*,double*,double*,double*,int*,double*,double*,double*,int*); */
static PyObject *f2py_rout_mvn_mvnun(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(int*,int*,double*,double*,double*,double*,int*,double*,double*,double*,int*)) {
    PyObject * volatile capi_buildvalue = NULL;
    volatile int f2py_success = 1;
/*decl*/

    int d = 0;
    int n = 0;
    double *lower = NULL;
    npy_intp lower_Dims[1] = {-1};
    const int lower_Rank = 1;
    PyArrayObject *capi_lower_as_array = NULL;
    int capi_lower_intent = 0;
    PyObject *lower_capi = Py_None;
    double *upper = NULL;
    npy_intp upper_Dims[1] = {-1};
    const int upper_Rank = 1;
    PyArrayObject *capi_upper_as_array = NULL;
    int capi_upper_intent = 0;
    PyObject *upper_capi = Py_None;
    double *means = NULL;
    npy_intp means_Dims[2] = {-1, -1};
    const int means_Rank = 2;
    PyArrayObject *capi_means_as_array = NULL;
    int capi_means_intent = 0;
    PyObject *means_capi = Py_None;
    double *covar = NULL;
    npy_intp covar_Dims[2] = {-1, -1};
    const int covar_Rank = 2;
    PyArrayObject *capi_covar_as_array = NULL;
    int capi_covar_intent = 0;
    PyObject *covar_capi = Py_None;
    int maxpts = 0;
    PyObject *maxpts_capi = Py_None;
    double abseps = 0;
    PyObject *abseps_capi = Py_None;
    double releps = 0;
    PyObject *releps_capi = Py_None;
    double value = 0;
    int inform = 0;
    static char *capi_kwlist[] = {"lower","upper","means","covar","maxpts","abseps","releps",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
    if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
        "OOOO|OOO:mvn.mvnun",\
        capi_kwlist,&lower_capi,&upper_capi,&means_capi,&covar_capi,&maxpts_capi,&abseps_capi,&releps_capi))
        return NULL;
/*frompyobj*/
    /* Processing variable means */
    ;
    capi_means_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun: failed to create array from the 3rd argument `means`";
    capi_means_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,means_Dims,means_Rank,  capi_means_intent,means_capi,capi_errmess);
    if (capi_means_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        means = (double *)(PyArray_DATA(capi_means_as_array));

    /* Processing variable abseps */
    if (abseps_capi == Py_None) abseps = 1e-06; else
        f2py_success = double_from_pyobj(&abseps,abseps_capi,"mvn.mvnun() 2nd keyword (abseps) can't be converted to double");
    if (f2py_success) {
    /* Processing variable releps */
    if (releps_capi == Py_None) releps = 1e-06; else
        f2py_success = double_from_pyobj(&releps,releps_capi,"mvn.mvnun() 3rd keyword (releps) can't be converted to double");
    if (f2py_success) {
    /* Processing variable value */
    /* Processing variable inform */
    /* Processing variable d */
    d = shape(means,0);
    /* Processing variable n */
    n = shape(means,1);
    /* Processing variable lower */
    lower_Dims[0]=d;
    capi_lower_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun: failed to create array from the 1st argument `lower`";
    capi_lower_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,lower_Dims,lower_Rank,  capi_lower_intent,lower_capi,capi_errmess);
    if (capi_lower_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        lower = (double *)(PyArray_DATA(capi_lower_as_array));

    /* Processing variable upper */
    upper_Dims[0]=d;
    capi_upper_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun: failed to create array from the 2nd argument `upper`";
    capi_upper_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,upper_Dims,upper_Rank,  capi_upper_intent,upper_capi,capi_errmess);
    if (capi_upper_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        upper = (double *)(PyArray_DATA(capi_upper_as_array));

    /* Processing variable covar */
    covar_Dims[0]=d,covar_Dims[1]=d;
    capi_covar_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun: failed to create array from the 4th argument `covar`";
    capi_covar_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,covar_Dims,covar_Rank,  capi_covar_intent,covar_capi,capi_errmess);
    if (capi_covar_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        covar = (double *)(PyArray_DATA(capi_covar_as_array));

    /* Processing variable maxpts */
    if (maxpts_capi == Py_None) maxpts = d*1000; else
        f2py_success = int_from_pyobj(&maxpts,maxpts_capi,"mvn.mvnun() 1st keyword (maxpts) can't be converted to int");
    if (f2py_success) {
/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
                (*f2py_func)(&d,&n,lower,upper,means,covar,&maxpts,&abseps,&releps,&value,&inform);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
        if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
        CFUNCSMESS("Building return value.\n");
        capi_buildvalue = Py_BuildValue("di",value,inform);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
        } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
    } /*if (f2py_success) of maxpts*/
    /* End of cleaning variable maxpts */
    if((PyObject *)capi_covar_as_array!=covar_capi) {
        Py_XDECREF(capi_covar_as_array); }
    }  /* if (capi_covar_as_array == NULL) ... else of covar */
    /* End of cleaning variable covar */
    if((PyObject *)capi_upper_as_array!=upper_capi) {
        Py_XDECREF(capi_upper_as_array); }
    }  /* if (capi_upper_as_array == NULL) ... else of upper */
    /* End of cleaning variable upper */
    if((PyObject *)capi_lower_as_array!=lower_capi) {
        Py_XDECREF(capi_lower_as_array); }
    }  /* if (capi_lower_as_array == NULL) ... else of lower */
    /* End of cleaning variable lower */
    /* End of cleaning variable n */
    /* End of cleaning variable d */
    /* End of cleaning variable inform */
    /* End of cleaning variable value */
    } /*if (f2py_success) of releps*/
    /* End of cleaning variable releps */
    } /*if (f2py_success) of abseps*/
    /* End of cleaning variable abseps */
    if((PyObject *)capi_means_as_array!=means_capi) {
        Py_XDECREF(capi_means_as_array); }
    }  /* if (capi_means_as_array == NULL) ... else of means */
    /* End of cleaning variable means */
/*end of cleanupfrompyobj*/
    if (capi_buildvalue == NULL) {
/*routdebugfailure*/
    } else {
/*routdebugleave*/
    }
    CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
    return capi_buildvalue;
}
/******************************** end of mvnun ********************************/

/******************************* mvnun_weighted *******************************/
static char doc_f2py_rout_mvn_mvnun_weighted[] = "\
value,inform = mvnun_weighted(lower,upper,means,weights,covar,[maxpts,abseps,releps])\n\nWrapper for ``mvnun_weighted``.\
\n\nParameters\n----------\n"
"lower : input rank-1 array('d') with bounds (d)\n"
"upper : input rank-1 array('d') with bounds (d)\n"
"means : input rank-2 array('d') with bounds (d,n)\n"
"weights : input rank-1 array('d') with bounds (n)\n"
"covar : input rank-2 array('d') with bounds (d,d)\n"
"\nOther Parameters\n----------------\n"
"maxpts : input int, optional\n    Default: d*1000\n"
"abseps : input float, optional\n    Default: 1e-06\n"
"releps : input float, optional\n    Default: 1e-06\n"
"\nReturns\n-------\n"
"value : float\n"
"inform : int";
/* extern void F_FUNC_US(mvnun_weighted,MVNUN_WEIGHTED)(int*,int*,double*,double*,double*,double*,double*,int*,double*,double*,double*,int*); */
static PyObject *f2py_rout_mvn_mvnun_weighted(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(int*,int*,double*,double*,double*,double*,double*,int*,double*,double*,double*,int*)) {
    PyObject * volatile capi_buildvalue = NULL;
    volatile int f2py_success = 1;
/*decl*/

    int d = 0;
    int n = 0;
    double *lower = NULL;
    npy_intp lower_Dims[1] = {-1};
    const int lower_Rank = 1;
    PyArrayObject *capi_lower_as_array = NULL;
    int capi_lower_intent = 0;
    PyObject *lower_capi = Py_None;
    double *upper = NULL;
    npy_intp upper_Dims[1] = {-1};
    const int upper_Rank = 1;
    PyArrayObject *capi_upper_as_array = NULL;
    int capi_upper_intent = 0;
    PyObject *upper_capi = Py_None;
    double *means = NULL;
    npy_intp means_Dims[2] = {-1, -1};
    const int means_Rank = 2;
    PyArrayObject *capi_means_as_array = NULL;
    int capi_means_intent = 0;
    PyObject *means_capi = Py_None;
    double *weights = NULL;
    npy_intp weights_Dims[1] = {-1};
    const int weights_Rank = 1;
    PyArrayObject *capi_weights_as_array = NULL;
    int capi_weights_intent = 0;
    PyObject *weights_capi = Py_None;
    double *covar = NULL;
    npy_intp covar_Dims[2] = {-1, -1};
    const int covar_Rank = 2;
    PyArrayObject *capi_covar_as_array = NULL;
    int capi_covar_intent = 0;
    PyObject *covar_capi = Py_None;
    int maxpts = 0;
    PyObject *maxpts_capi = Py_None;
    double abseps = 0;
    PyObject *abseps_capi = Py_None;
    double releps = 0;
    PyObject *releps_capi = Py_None;
    double value = 0;
    int inform = 0;
    static char *capi_kwlist[] = {"lower","upper","means","weights","covar","maxpts","abseps","releps",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
    if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
        "OOOOO|OOO:mvn.mvnun_weighted",\
        capi_kwlist,&lower_capi,&upper_capi,&means_capi,&weights_capi,&covar_capi,&maxpts_capi,&abseps_capi,&releps_capi))
        return NULL;
/*frompyobj*/
    /* Processing variable means */
    ;
    capi_means_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun_weighted: failed to create array from the 3rd argument `means`";
    capi_means_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,means_Dims,means_Rank,  capi_means_intent,means_capi,capi_errmess);
    if (capi_means_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        means = (double *)(PyArray_DATA(capi_means_as_array));

    /* Processing variable abseps */
    if (abseps_capi == Py_None) abseps = 1e-06; else
        f2py_success = double_from_pyobj(&abseps,abseps_capi,"mvn.mvnun_weighted() 2nd keyword (abseps) can't be converted to double");
    if (f2py_success) {
    /* Processing variable releps */
    if (releps_capi == Py_None) releps = 1e-06; else
        f2py_success = double_from_pyobj(&releps,releps_capi,"mvn.mvnun_weighted() 3rd keyword (releps) can't be converted to double");
    if (f2py_success) {
    /* Processing variable value */
    /* Processing variable inform */
    /* Processing variable d */
    d = shape(means,0);
    /* Processing variable n */
    n = shape(means,1);
    /* Processing variable lower */
    lower_Dims[0]=d;
    capi_lower_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun_weighted: failed to create array from the 1st argument `lower`";
    capi_lower_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,lower_Dims,lower_Rank,  capi_lower_intent,lower_capi,capi_errmess);
    if (capi_lower_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        lower = (double *)(PyArray_DATA(capi_lower_as_array));

    /* Processing variable upper */
    upper_Dims[0]=d;
    capi_upper_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun_weighted: failed to create array from the 2nd argument `upper`";
    capi_upper_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,upper_Dims,upper_Rank,  capi_upper_intent,upper_capi,capi_errmess);
    if (capi_upper_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        upper = (double *)(PyArray_DATA(capi_upper_as_array));

    /* Processing variable weights */
    weights_Dims[0]=n;
    capi_weights_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun_weighted: failed to create array from the 4th argument `weights`";
    capi_weights_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,weights_Dims,weights_Rank,  capi_weights_intent,weights_capi,capi_errmess);
    if (capi_weights_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        weights = (double *)(PyArray_DATA(capi_weights_as_array));

    /* Processing variable covar */
    covar_Dims[0]=d,covar_Dims[1]=d;
    capi_covar_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvnun_weighted: failed to create array from the 5th argument `covar`";
    capi_covar_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,covar_Dims,covar_Rank,  capi_covar_intent,covar_capi,capi_errmess);
    if (capi_covar_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        covar = (double *)(PyArray_DATA(capi_covar_as_array));

    /* Processing variable maxpts */
    if (maxpts_capi == Py_None) maxpts = d*1000; else
        f2py_success = int_from_pyobj(&maxpts,maxpts_capi,"mvn.mvnun_weighted() 1st keyword (maxpts) can't be converted to int");
    if (f2py_success) {
/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
                (*f2py_func)(&d,&n,lower,upper,means,weights,covar,&maxpts,&abseps,&releps,&value,&inform);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
        if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
        CFUNCSMESS("Building return value.\n");
        capi_buildvalue = Py_BuildValue("di",value,inform);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
        } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
    } /*if (f2py_success) of maxpts*/
    /* End of cleaning variable maxpts */
    if((PyObject *)capi_covar_as_array!=covar_capi) {
        Py_XDECREF(capi_covar_as_array); }
    }  /* if (capi_covar_as_array == NULL) ... else of covar */
    /* End of cleaning variable covar */
    if((PyObject *)capi_weights_as_array!=weights_capi) {
        Py_XDECREF(capi_weights_as_array); }
    }  /* if (capi_weights_as_array == NULL) ... else of weights */
    /* End of cleaning variable weights */
    if((PyObject *)capi_upper_as_array!=upper_capi) {
        Py_XDECREF(capi_upper_as_array); }
    }  /* if (capi_upper_as_array == NULL) ... else of upper */
    /* End of cleaning variable upper */
    if((PyObject *)capi_lower_as_array!=lower_capi) {
        Py_XDECREF(capi_lower_as_array); }
    }  /* if (capi_lower_as_array == NULL) ... else of lower */
    /* End of cleaning variable lower */
    /* End of cleaning variable n */
    /* End of cleaning variable d */
    /* End of cleaning variable inform */
    /* End of cleaning variable value */
    } /*if (f2py_success) of releps*/
    /* End of cleaning variable releps */
    } /*if (f2py_success) of abseps*/
    /* End of cleaning variable abseps */
    if((PyObject *)capi_means_as_array!=means_capi) {
        Py_XDECREF(capi_means_as_array); }
    }  /* if (capi_means_as_array == NULL) ... else of means */
    /* End of cleaning variable means */
/*end of cleanupfrompyobj*/
    if (capi_buildvalue == NULL) {
/*routdebugfailure*/
    } else {
/*routdebugleave*/
    }
    CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
    return capi_buildvalue;
}
/*************************** end of mvnun_weighted ***************************/

/*********************************** mvndst ***********************************/
static char doc_f2py_rout_mvn_mvndst[] = "\
error,value,inform = mvndst(lower,upper,infin,correl,[maxpts,abseps,releps])\n\nWrapper for ``mvndst``.\
\n\nParameters\n----------\n"
"lower : input rank-1 array('d') with bounds (n)\n"
"upper : input rank-1 array('d') with bounds (n)\n"
"infin : input rank-1 array('i') with bounds (n)\n"
"correl : input rank-1 array('d') with bounds ((-n + n * n) / 2)\n"
"\nOther Parameters\n----------------\n"
"maxpts : input int, optional\n    Default: 2000\n"
"abseps : input float, optional\n    Default: 1e-06\n"
"releps : input float, optional\n    Default: 1e-06\n"
"\nReturns\n-------\n"
"error : float\n"
"value : float\n"
"inform : int";
/* extern void F_FUNC(mvndst,MVNDST)(int*,double*,double*,int*,double*,int*,double*,double*,double*,double*,int*); */
static PyObject *f2py_rout_mvn_mvndst(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(int*,double*,double*,int*,double*,int*,double*,double*,double*,double*,int*)) {
    PyObject * volatile capi_buildvalue = NULL;
    volatile int f2py_success = 1;
/*decl*/

    int n = 0;
    double *lower = NULL;
    npy_intp lower_Dims[1] = {-1};
    const int lower_Rank = 1;
    PyArrayObject *capi_lower_as_array = NULL;
    int capi_lower_intent = 0;
    PyObject *lower_capi = Py_None;
    double *upper = NULL;
    npy_intp upper_Dims[1] = {-1};
    const int upper_Rank = 1;
    PyArrayObject *capi_upper_as_array = NULL;
    int capi_upper_intent = 0;
    PyObject *upper_capi = Py_None;
    int *infin = NULL;
    npy_intp infin_Dims[1] = {-1};
    const int infin_Rank = 1;
    PyArrayObject *capi_infin_as_array = NULL;
    int capi_infin_intent = 0;
    PyObject *infin_capi = Py_None;
    double *correl = NULL;
    npy_intp correl_Dims[1] = {-1};
    const int correl_Rank = 1;
    PyArrayObject *capi_correl_as_array = NULL;
    int capi_correl_intent = 0;
    PyObject *correl_capi = Py_None;
    int maxpts = 0;
    PyObject *maxpts_capi = Py_None;
    double abseps = 0;
    PyObject *abseps_capi = Py_None;
    double releps = 0;
    PyObject *releps_capi = Py_None;
    double error = 0;
    double value = 0;
    int inform = 0;
    static char *capi_kwlist[] = {"lower","upper","infin","correl","maxpts","abseps","releps",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
    if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
        "OOOO|OOO:mvn.mvndst",\
        capi_kwlist,&lower_capi,&upper_capi,&infin_capi,&correl_capi,&maxpts_capi,&abseps_capi,&releps_capi))
        return NULL;
/*frompyobj*/
    /* Processing variable lower */
    ;
    capi_lower_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvndst: failed to create array from the 1st argument `lower`";
    capi_lower_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,lower_Dims,lower_Rank,  capi_lower_intent,lower_capi,capi_errmess);
    if (capi_lower_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        lower = (double *)(PyArray_DATA(capi_lower_as_array));

    /* Processing variable maxpts */
    if (maxpts_capi == Py_None) maxpts = 2000; else
        f2py_success = int_from_pyobj(&maxpts,maxpts_capi,"mvn.mvndst() 1st keyword (maxpts) can't be converted to int");
    if (f2py_success) {
    /* Processing variable abseps */
    if (abseps_capi == Py_None) abseps = 1e-06; else
        f2py_success = double_from_pyobj(&abseps,abseps_capi,"mvn.mvndst() 2nd keyword (abseps) can't be converted to double");
    if (f2py_success) {
    /* Processing variable releps */
    if (releps_capi == Py_None) releps = 1e-06; else
        f2py_success = double_from_pyobj(&releps,releps_capi,"mvn.mvndst() 3rd keyword (releps) can't be converted to double");
    if (f2py_success) {
    /* Processing variable error */
    /* Processing variable value */
    /* Processing variable inform */
    /* Processing variable n */
    n = len(lower);
    /* Processing variable upper */
    upper_Dims[0]=n;
    capi_upper_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvndst: failed to create array from the 2nd argument `upper`";
    capi_upper_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,upper_Dims,upper_Rank,  capi_upper_intent,upper_capi,capi_errmess);
    if (capi_upper_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        upper = (double *)(PyArray_DATA(capi_upper_as_array));

    /* Processing variable infin */
    infin_Dims[0]=n;
    capi_infin_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvndst: failed to create array from the 3rd argument `infin`";
    capi_infin_as_array = ndarray_from_pyobj(  NPY_INT,1,infin_Dims,infin_Rank,  capi_infin_intent,infin_capi,capi_errmess);
    if (capi_infin_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        infin = (int *)(PyArray_DATA(capi_infin_as_array));

    /* Processing variable correl */
    correl_Dims[0]=(-n + n * n) / 2;
    capi_correl_intent |= F2PY_INTENT_IN;
    const char * capi_errmess = "mvn.mvn.mvndst: failed to create array from the 4th argument `correl`";
    capi_correl_as_array = ndarray_from_pyobj(  NPY_DOUBLE,1,correl_Dims,correl_Rank,  capi_correl_intent,correl_capi,capi_errmess);
    if (capi_correl_as_array == NULL) {
        PyObject* capi_err = PyErr_Occurred();
        if (capi_err == NULL) {
            capi_err = mvn_error;
            PyErr_SetString(capi_err, capi_errmess);
        }
    } else {
        correl = (double *)(PyArray_DATA(capi_correl_as_array));

/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
                (*f2py_func)(&n,lower,upper,infin,correl,&maxpts,&abseps,&releps,&error,&value,&inform);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
        if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
        CFUNCSMESS("Building return value.\n");
        capi_buildvalue = Py_BuildValue("ddi",error,value,inform);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
        } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
    if((PyObject *)capi_correl_as_array!=correl_capi) {
        Py_XDECREF(capi_correl_as_array); }
    }  /* if (capi_correl_as_array == NULL) ... else of correl */
    /* End of cleaning variable correl */
    if((PyObject *)capi_infin_as_array!=infin_capi) {
        Py_XDECREF(capi_infin_as_array); }
    }  /* if (capi_infin_as_array == NULL) ... else of infin */
    /* End of cleaning variable infin */
    if((PyObject *)capi_upper_as_array!=upper_capi) {
        Py_XDECREF(capi_upper_as_array); }
    }  /* if (capi_upper_as_array == NULL) ... else of upper */
    /* End of cleaning variable upper */
    /* End of cleaning variable n */
    /* End of cleaning variable inform */
    /* End of cleaning variable value */
    /* End of cleaning variable error */
    } /*if (f2py_success) of releps*/
    /* End of cleaning variable releps */
    } /*if (f2py_success) of abseps*/
    /* End of cleaning variable abseps */
    } /*if (f2py_success) of maxpts*/
    /* End of cleaning variable maxpts */
    if((PyObject *)capi_lower_as_array!=lower_capi) {
        Py_XDECREF(capi_lower_as_array); }
    }  /* if (capi_lower_as_array == NULL) ... else of lower */
    /* End of cleaning variable lower */
/*end of cleanupfrompyobj*/
    if (capi_buildvalue == NULL) {
/*routdebugfailure*/
    } else {
/*routdebugleave*/
    }
    CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
    return capi_buildvalue;
}
/******************************* end of mvndst *******************************/
/*eof body*/

/******************* See f2py2e/f90mod_rules.py: buildhooks *******************/
/*need_f90modhooks*/

/************** See f2py2e/rules.py: module_rules['modulebody'] **************/

/******************* See f2py2e/common_rules.py: buildhooks *******************/

static FortranDataDef f2py_dkblck_def[] = {
  {"ivls",0,{{-1}},NPY_INT, 1},
  {NULL}
};
static void f2py_setup_dkblck(char *ivls) {
  int i_f2py=0;
  f2py_dkblck_def[i_f2py++].data = ivls;
}
extern void F_FUNC(f2pyinitdkblck,F2PYINITDKBLCK)(void(*)(char*));
static void f2py_init_dkblck(void) {
  F_FUNC(f2pyinitdkblck,F2PYINITDKBLCK)(f2py_setup_dkblck);
}

/*need_commonhooks*/

/**************************** See f2py2e/rules.py ****************************/

static FortranDataDef f2py_routine_defs[] = {
    {"mvnun",-1,{{-1}},0,0,(char *)  F_FUNC(mvnun,MVNUN),  (f2py_init_func)f2py_rout_mvn_mvnun,doc_f2py_rout_mvn_mvnun},
    {"mvnun_weighted",-1,{{-1}},0,0,(char *)  F_FUNC_US(mvnun_weighted,MVNUN_WEIGHTED),  (f2py_init_func)f2py_rout_mvn_mvnun_weighted,doc_f2py_rout_mvn_mvnun_weighted},
    {"mvndst",-1,{{-1}},0,0,(char *)  F_FUNC(mvndst,MVNDST),  (f2py_init_func)f2py_rout_mvn_mvndst,doc_f2py_rout_mvn_mvndst},

/*eof routine_defs*/
    {NULL}
};

static PyMethodDef f2py_module_methods[] = {

    {NULL,NULL}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mvn",
    NULL,
    -1,
    f2py_module_methods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_mvn(void) {
    int i;
    PyObject *m,*d, *s, *tmp;
    m = mvn_module = PyModule_Create(&moduledef);
    Py_SET_TYPE(&PyFortran_Type, &PyType_Type);
    import_array();
    if (PyErr_Occurred())
        {PyErr_SetString(PyExc_ImportError, "can't initialize module mvn (failed to import numpy)"); return m;}
    d = PyModule_GetDict(m);
    s = PyUnicode_FromString("1.26.4");
    PyDict_SetItemString(d, "__version__", s);
    Py_DECREF(s);
    s = PyUnicode_FromString(
        "This module 'mvn' is auto-generated with f2py (version:1.26.4).\nFunctions:\n"
"    value,inform = mvnun(lower,upper,means,covar,maxpts=d*1000,abseps=1e-06,releps=1e-06)\n"
"    value,inform = mvnun_weighted(lower,upper,means,weights,covar,maxpts=d*1000,abseps=1e-06,releps=1e-06)\n"
"    error,value,inform = mvndst(lower,upper,infin,correl,maxpts=2000,abseps=1e-06,releps=1e-06)\n"
"COMMON blocks:\n""  /dkblck/ ivls\n"".");
    PyDict_SetItemString(d, "__doc__", s);
    Py_DECREF(s);
    s = PyUnicode_FromString("1.26.4");
    PyDict_SetItemString(d, "__f2py_numpy_version__", s);
    Py_DECREF(s);
    mvn_error = PyErr_NewException ("mvn.error", NULL, NULL);
    /*
     * Store the error object inside the dict, so that it could get deallocated.
     * (in practice, this is a module, so it likely will not and cannot.)
     */
    PyDict_SetItemString(d, "_mvn_error", mvn_error);
    Py_DECREF(mvn_error);
    for(i=0;f2py_routine_defs[i].name!=NULL;i++) {
        tmp = PyFortranObject_NewAsAttr(&f2py_routine_defs[i]);
        PyDict_SetItemString(d, f2py_routine_defs[i].name, tmp);
        Py_DECREF(tmp);
    }



/*eof initf2pywraphooks*/
/*eof initf90modhooks*/

  tmp = PyFortranObject_New(f2py_dkblck_def,f2py_init_dkblck);
  if (tmp == NULL) return NULL;
  if (F2PyDict_SetItemString(d, "dkblck", tmp) == -1) return NULL;
  Py_DECREF(tmp);
/*eof initcommonhooks*/


#ifdef F2PY_REPORT_ATEXIT
    if (! PyErr_Occurred())
        on_exit(f2py_report_on_exit,(void*)"mvn");
#endif
    return m;
}
#ifdef __cplusplus
}
#endif
