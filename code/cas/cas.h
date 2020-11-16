/*
 * @Author: gongluck 
 * @Date: 2020-11-16 16:02:56 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-16 16:06:02
 */

template <typename ElemType>
class Queue
{
public:
    Queue();
    ~Queue();

public:
    void push(ElemType elem);
    bool pop();
    void show();

private:
    struct _qNode
    {
        _qNode() : _next(nullptr) {}
        _qNode(ElemType elem) : _elem(elem), _next(nullptr) {}
        ElemType _elem;
        struct _qNode *_next;
    };

private:
    struct _qNode *_head;
    struct _qNode *_tail;
};

template <typename ElemType>
Queue<ElemType>::Queue()
{
    _head = _tail = new _qNode();
}

template <typename ElemType>
Queue<ElemType>::~Queue()
{
    while (_head != nullptr)
    {
        struct _qNode *tempNode = _head;
        _head = _head->_next;
        delete tempNode;
    }
}

template <typename ElemType>
void Queue<ElemType>::push(ElemType elem)
{
    struct _qNode *newNode = new struct _qNode(elem);
    struct _qNode *oldp = _tail;
    while (!__sync_bool_compare_and_swap(&_tail->_next, nullptr, newNode))
        ;
    __sync_bool_compare_and_swap(&_tail, oldp, newNode);
}

template <typename ElemType>
bool Queue<ElemType>::pop()
{
    struct _qNode *p;
    do
    {
        p = _head;
        if (p->_next == nullptr)
            return false;
    } while (!__sync_bool_compare_and_swap(&_head, p, p->_next));
    delete p;
    return true;
}
