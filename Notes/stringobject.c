PyTypeObject PyString_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,
    "str",
    sizeof(PyStringObject),
    sizeof(char),
    ......
    (reprfunc)string_repr,
    &string_as_number,
    &string_as_sequence,
    &string_as_mapping,
    (hashfunc)string_hash,
    0,
    ......
    string_new,
    PyObject_Del,
};
PyObject* PyString_FromString(const char *str)
{
    register size_t size;
    register PyStringObject *op;

    size = strlen(str);
    if (size > PY_SSIZE_T_MAX) {
        return NULL;
    }

    if (size == 0 && (op = nullstring) != NULL) {
        return (PyObject *)op;
    }

    if (size == 1 && (op = characters[*str & UCHAR_MAX]) != NULL) {
        return (PyObject *)op;
    }

    op = (PyStringObject *)PyObject_MALLOC(sizeof(PyStringObject) + size);
    PyObject_INIT_VAR(op, &PyString_Type, size);
    op->ob_shash = -1;
    op->ob_sstate = SSTATE_NOT_INTERNED;
    memcpy(op->ob_sval, str, size+1);
    ......
    return (PyObject *) op;
}
/*
___________________________________________________________________

ref | type | 6(ob_size) | -1(ob_hash) | 0(ob_sstate) | p(ob_sval) | ython\0
___________________________________________________________________

*/
PyObject* PyString_FromStringAndSize(const char *str, int size)
{
    register PyStringObject *op;
    if (size == 0 && (op = nullstring) != NULL) {
        return (PyObject *)op;
    }
    if (size == 1 && str != NULL && (op = characters[*str & UCHAR_MAX]) != NULL)
    {
        return (PyObject *)op;
    }
    op = (PyStringObject *)PyObject_MALLOC(sizeof(PyStringObject) + size);
    PyObject_INIT_VAR(op, &PyString_Type, size);
    op->ob_shash = -1
    op->ob_ssate = SSTATE_NOT_INTERNED;
    if (str != NULL)
        memcpy(op->ob_sval, str, size);
    op->ob_sval[size] = '\0';
    
    if (size == 0) {
        PyObject *t = (PyObject *)op;
        PyString_InternInPlace(&t);
        op = (PyStringObject *)t;
        nullstring = op;
    } else if (size == 1) {
        PyObject *t = (PyObject *)op;
        PyString_InternInPlace(&t);
        op = (PyStringObject *)t;
        characters[*str & UCHAR_MAX] = op;
    }
    return (PyObject *) op;
}
void PyString_InternInPlace(PyObject **p)
{
    register PyStringObject *s = (PyStringObject *)(*p);
    PyObject *t;
    if (!PyString_CheckExact(s))
        return;
    if (PyString_CHECK_INTERNED(s))
        return;
    if (interned == NULL) {
        interned = PyDict_New();
    }
    t = PyDict_GetItem(interned, (PyObject *)s);
    if (t) {
        Py_INCREF(t);
        Py_DECREF(*p);
        *p = t;
        return;
    }
    PyDict_SetItem(interned, (PyObject *)s, (PyObject *)s);
    s->ob_refcnt -= 2;
    PyString_CHECK_INTERNED(s) = SSTATE_INTERNED_MORTAL;
}
static void string_dealloc(PyObject *op)
{
    switch (PyString_CHECK_INTERNED(op)) {
        case SSTATE_NOT_INTERNED:
            break;

        case SSTATE_INTERNED_MORTAL:
            op->ob_refcnt = 3
            if (PyDict_DelItem(interned, op) != 0)
                Py_FatalError(
                    "deletion of interned string failed");
            break;

        case SSTATE_INTERNED_IMMORTAL:
            Py_FatalError("Immortal interned string died.");

        default:
            Py_FatalError("Inconsistent interned string state.");
    }
    op->ob_type->tp_free(op);
}
static PyObject* string_concat(register PyStringObject *a, register PyObject *bb)
{
    register unsigned int size;
    register PyStringObject *op;
    #define b ((PyStringObject *)bb)
    size = a->ob_size + b->ob_size;
    op = (PyStringObject *)PyObject_MALLOC(sizeof(PyStringObject) + size);
    PyObject_INIT_VAR(op, &PyString_Type, size);
    op->ob_shash = -1;
    op->ob_sstate = SSTATE_NOT_INTERNED;

    memcpy(op->ob_sval, a->ob_sval, (int) a->ob_size);
    memcpy(op->ob_sval + a->ob_size, b->ob_sval, (int) b->ob_size);
    op->ob_sval[size] = '\0';
    return (PyObject *) op;
    #undef b
}
static PyObject* string_join(PyStringObject *self, PyObject *orig)
{
    char *seq = PyString_AS_STRING(self);
    const int seplen = PyString_GET_SIZE(self);
    PyObject *res = NULL;
    char *p;
    int seqlen = 0;
    size_t sz = 0;
    int i;
    PyObject *seq, *item;
    for (i = 0; i < seqlen; i++)
    {
        item = PySequence_Fast_GET_ITEM(seq, i);
        sz += PyString_GET_SIZE(item);
        if (i != 0)
            sz += seqlen;
    }
    res = PyString_FromStringAndSize((char*)NULL, (int)sz);
    p = PyString_AS_STRING(res);
    for (i = 0; i < seqlen; ++i)
    {
        size_t n;
        item = PySequence_Fast_GET_ITEM(seq, i);
        n = PyString_GET_SIZE(item);
        memcpy(p, PyString_AS_STRING(item), n);
        p += n;
        if (i < seqlen-1)
        {
            memcpy(p, seq, seqlen);
            p += seqlen;
        }
     }
     return res;
}
