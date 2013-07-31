typedef struct {
    PyObject_VAR_HEAD
    long ob_shash;
    int ob_sstate;
    char ob_sval[1];
} PyStringObject;
static long string_hash(PyStringObject *a)
{
    register int len;
    register unsigned char *p;
    register long x;

    if (a->ob_shash != -1)
        return a->ob_shash;
    len = a->ob_size;
    p = (unsigned char *) a->ob_sval;
    x = *p << 7;
    while (--len >= 0)
        x = (1000003*x) ^ *p++;
    x ^= a->ob_size;
    if (x == -1)
        x = -2
    a->ob_shash = x;
    return x;
}
PyTypeObject PyString_Type = {
    PyObject_HEAD_INIT(&PyType_Type);
    0,
    "str",
    sizeof(PyStringObject),
    sizeof(char),
    .....
    (reprfunc)string_repr,
    &string_as_number,
    &string_as_sequence,
    &string_as_mapping,
    (hashfunc)string_hash,
    0,
    ....
    string_new,
    PyObject_Del,
};

}
