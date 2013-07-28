typedef struct _object {
    PyObject_HEAD
} PyObject;

#ifdef Py_TRACE_REFS
/*Define pointers to support a doubly-linked list of all live heap objects.*/
    #define _PyObject_HEAD_EXTRA   \
        struct _object *_ob_next;  \
        struct _object *_ob_prev;
    #define _PyObject_EXTRA_INIT 0, 0,
#else
    #define _PyObject_HEAD_EXTRA
    #define _PyObject_EXTRA_INIT
#endif

#define PyObject_HEAD           \
    _PyObject_HEAD_EXTRA            \
    int ob_refcnt;              \
    struct _typeobject *ob_type;

//The final thing should look like:
typedef struct _object {
    int ob_refcnt;
    struct _typeobject *ob_type;//指定对象类型的类型对象
} PyObject;
//ob_refcnt和python内存管理有关
typedef struct {
    PyObject_HEAD
    long ob_ival;
} PyIntObject;
#define PyObject_VAR_HEAD    \
    PyObject_HEAD             \
    int ob_size;

typedef struct {
    PyObject_VAR_HEAD
} PyVarObject;

typedef PyObject* (*binaryfunc)(PyObject*, PyObject*);
typedef struct {
    binaryfunc nb_add;
    binaryfunc nb_substract;
} PyNumberMethods;
/*
>>> class MyInt(int):
        def __getitem__(self, key):
            return key + str(self)
>>> a = MyInt(1)
>>> b = MyInt(2)
>>> print a+b
>>> print a['key']
我们对int子类实例进行a['key']操作，重写__getitem__ special method
我们指定了MyInt在python内部对应的PyTypeObject的tp_as_mapping.mp_subscript
*/

