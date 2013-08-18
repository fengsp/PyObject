typedef struct {
    PyObject_VAR_HEAD
    //ob_item为指向元素列表的指针，实际上，Python中的list[0]就是ob_item[0]
    PyObject **ob_item;
    int allocated;
} PyListObject;
