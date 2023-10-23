#include <unordered_map>
#include <list>
#include <mutex>

namespace
{

template <typename Data>
class Element {
public:
    Element(Data d) : data(d) {}
    Element() = default;
    ~Element() = default;

    Data data;
    Element *pre, *next;
};


template <typename Data>
class List {
public:
    List()
    {
        _size = 0;
        _head = new Element<Data>();
        _tail = new Element<Data>();
        
        _head->pre = nullptr;
        _head->next = _tail;
        _tail->pre = _head;
        _tail->next = nullptr;
    }

    Element<Data> *remove(Element<Data> *e)
    {
        e->pre->next = e->next;
        e->next->pre = e->pre;
        _size--;
        return e;
    }

    Element<Data> *pop_back()
    {
        if (_head->next == _tail) {
            return nullptr;
        }

        auto e = _tail->pre;
        e->pre->next = _tail;
        _tail->pre = e->pre;
        _size--;
        return e;
    }

    Element<Data> *push_front(Data data)
    {
        auto *e = new Element<Data>(data);
        e->next = _head->next;
        e->pre = _head;
        _head->next->pre = e;
        _head->next = e;
        _size++;
        return e;
    }

    Element<Data> *push_front(Element<Data> *e)
    {
        e->next = _head->next;
        e->pre = _head;
        _head->next->pre = e;
        _head->next = e;
        _size++;
        return e;
    }    

    Element<Data> *push_back(Data data)
    {
        auto *e = new Element<Data>(data);
        e->pre = _tail->pre;
        e->next = _tail;
        _tail->pre->next = e;
        _tail->pre = e;
        _size++;
        return e;
    }

    Element<Data> *push_back(Element<Data> *e)
    {
        e->pre = _tail->pre;
        e->next = _tail;
        _tail->pre->next = e;
        _tail->pre = e;
        _size++;
        return e;
    }    

    void print_eles()
    {
        auto e = _head->next;
        while (e != _tail) {
            printf("%d ", e->data);
            e = e->next;
        }
        printf("\n");
    }

    int size()
    {
        return _size;
    }

private:
    int _size;
    Element<Data> *_head, *_tail;
};

} // namespace

template <typename Data>
class LRU {
public:
    typedef void(*remove_data_func)(Data data);

    LRU(int capacity = 60, remove_data_func rmfunc = nullptr) : _capacity(capacity), _rmfunc(rmfunc) {}
    ~LRU() = default;

    LRU(LRU&) = delete;
    LRU(LRU&&) = delete;
    LRU& operator=(LRU&) = delete;
    LRU& operator=(LRU&&) = delete;

    void add(Data data)
    {
        std::unique_lock lock(mtx);

        auto found = _map.find(data);
        if (found != _map.end()) {
            auto e = _datas.remove(found->second);
            _datas.push_front(e);
            return;
        }

        if (_datas.size() == _capacity) {
            auto e = _datas.pop_back();
            _map.erase(e->data);
            if (_rmfunc) {
                _rmfunc(e->data);
            }

            e->data = data;
            _datas.push_front(e);
            _map.insert({data, e});
            return;
        }

        auto e = _datas.push_front(data);
        _map.insert({data, e});
    }

    // for dbug
    void print_datas()
    {
        _datas.print_eles();
    }

private:
    remove_data_func _rmfunc;
    std::unordered_map<Data, Element<Data>* > _map;
    List<Data> _datas;
    std::mutex mtx;
    int _capacity;
};


#include <stdio.h>

void remove_data(int data)
{
    printf("remove data %d\n", data);
}

int main(int argc, char **argv)
{
    LRU<int> lru(5, remove_data);
    
    for (int i = 0; i < 5; i++) {
        lru.add(i);
    }
    lru.print_datas();

    lru.add(6);
    lru.add(7);
    lru.add(8);
    lru.print_datas();
    
    lru.add(3);
    lru.print_datas();

    lru.add(7);
    lru.print_datas();   
}
