PyTypeObject PyInt_Type = {
    PyObject_HEAD_INIT(&PyType_Type)
    0,
    "int",
    sizeof(PyIntObject),
    0,
    (destructor)int_dealloc,    /* tp_dealloc */
    (printfunc)int_print,       /* tp_print */
    0,                 /* tp_getattr */
    0,                 /* tp_setattr */
    (cmpfunc)int_compare,        /* tp_compare */
    (reprfunc)int_repr,          /* tp_repr */
    &int_as_number,             /* tp_as_number */
    0,        /* tp_as_sequence */
    0,        /* tp_as_mapping */
    (hashfunc)int_hash,   /* tp_hash */
    0,             /* tp_call */
    (reprfunc)int_repr,   /* tp_str */
    PyObject_GenericGetAttr,  /* tp_getattro */
    0,     /* tp_setattro */
    0     /* tp_as_buffer */
    /*  .........  */
}
/*
 int_dealloc   PyIntObject 对象析构操作
 int_free      PyIntObject 对象释放操作
 int_repr      转为PyStringObject对象
 int_hash      获得HASH值
 int_print     打印PyIntObject对象
 int_compare   比较PyIntObject对象
 int_as_number 数值操作集合
 int_methods   成员函数集合
*/
static int int_compare(PyIntObject *v, PyIntObject *w)
{
    register long i = v->ob_ival;
    register long j = w->ob_ival;
    return (i < j) ? -1 : (i > j) ? 1 : 0;
}
static PyNumerMethods int_as_number = {
    
};
#define PyInt_AS_LONG(op)  (((PyIntObject *)(op))->ob_ival)
#define CONVERT_TO_LONG(obj, lng) \
    if (PyInt_Check(obj)) {       \
        lng = PyInt_AS_LONG(obj); \
    }                 \
    else { \
        Py_INCREF(Py_NotImplemented);\
        return Py_NotImplemented;;   \
    }
static PyObject* int_add(PyIntObject *v, PyIntObject *w)
{
    register long a, b, x;
    CONVERT_TO_LONG(v, a);
    CONVERT_TO_LONG(w, b);
    x = a + b;
    if ((x^a) >= 0 || (x^b) >= 0)
        return PyInt_FromLong(x);
    return PyLong_Type.tp_as_number->nb_add((PyObject *)v, (PyObject *)w);
}
PyObject* PyInt_FromString(char *s, char **pend, int base)
{
    char *end;
    long x;

    if (base == 0 && s[0] == '0')
    {
        x = (long) PyOS_strtoul(s, &end, base);
    }
    else
        x = PyOS_strtol(s, &end, base);
    return PyInt_FromLong(x);
}
#ifndef NSMALLPOSINTS
    #define NSMALLPOSINTS   257
#endif
#ifndef NSMALLNEGINTS
    #define NSMALLNEGINTS   5
#endif
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
    static PyIntObject *small_ints[NSMALLNEGINTS + NSMALLPOSINTS];
#endif
#define BLOCK_SIZE 1000
#define BHEAD_SIZE 8
#define N_INTOBJECTS   ((BLOCK_SIZE - BHEAD_SIZE) /sizeof(PyIntObject))
struct _intblock {
    struct _intblock *next;
    PyIntObject objects[N_INTOBJECTS];
};
typedef struct _intblock PyIntBlock;

static PyIntBlock *block_list = NULL;
static PyIntObject *free_list = NULL;

PyObject* PyInt_FromLong(long ival)
{
    register PyIntObject *v;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
    if (-NSMALLNEGINTS <= ival && ival < NSMALLPOSINTS) {
        v = small_ints[ival + NSMALLNEGINTS];
        Py_INCREF(v);
        return (PyObject *) v;
    }
#endif
    if (free_list == NULL) {
        if ((free_list == fill_free_list()) == NULL)
            return NULL;
    }
    v = free_list;
    free_list = (PyIntObject *)v->ob_type;
    PyObject_INIT(v, &PyInt_Type);
    v->ob_ival = ival;
    return (PyObject *) v;
}

static PyIntObject* fill_free_list(void)
{
    PyIntObject *p, *q;
    p = (PyIntObject *) PyMem_MALLOC(sizeof(PyIntBlock));
    ((PyIntBlock *)p)->next = block_list;
    block_list = (PyIntBlock *)p;
    p = &((PyIntBlock *)p)->objects[0];
    q = p + N_INTOBJECTS;
    while (--q > p)
        q->ob_type = (struct _typeobject *)(q-1);
    q->ob_type = NULL;
    return p + N_INTOBJECTS - 1;
}
static void int_dealloc(PyIntObject *v)
{
    if (PyInt_CheckExact(v)) {
        v->ob_type = (struct _typeobject *)free_list;
        free_list = v;
    }
    else
        v->ob_type->tp_free((PyObject *)v);
}
int _PyInt_Init(void)
{
    PyIntObject *v;
    int ival;
#if NSMALLNEGINTS + NSMALLPOSINTS > 0
    for (ival = -NSMALLNEGINTS; ival < NSMALLPOSINTS; ival++)
    {
        if (!free_list && (free_list = fill_free_list()) == NULL)
            return 0;
        v = free_list;
        free_list = (PyIntObject *)v->ob_type;
        PyObject_INIT(v, &PyInt_Type);
        v->ob_ival = ival;
        small_ints[ival + NSMALLNEGINTS] = v;
    }
#endif
    return 1;
}

    }
}
}
}
