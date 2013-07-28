typedef struct {
    PyObject_HEAD;
    long ob_ival;
} PyIntObject;
PyObject *PyInt_FromLong(long ival)
PyObject* PyInt_FromString(char *s, char **pend, int base)
#ifdef Py_USING_UNICODE
    PyObject*PyInt_FromUnicode(Py_UNICODE *s, int length, int base)
#endif


