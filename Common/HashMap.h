#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdio.h>
#include <vector>

#define kHASHSIZE 997001

// Default hash function class
template <typename K>
struct KeyHash {
    unsigned long long operator()(const K& key) const
    {
        return reinterpret_cast<unsigned long long>(key) % kHASHSIZE;
    }
};

template <typename KEY, typename VALUE, typename F = KeyHash<K> > class HashMap;

template <typename KEY, typename VALUE> 
class HMHMNode 
{
public:
    KEY getKey();
    VALUE getValue();
private:
    friend class HashMap<KEY, VALUE>;
    HMNode():next(NULL), key(), value() {}
    HMNode(const KEY &k, const VALUE &v): key(k), value(v), next(NULL) {}
    ~HMNode() 
	{
        if (next) delete next;
    }
    KEY key;
    VALUE value;
    HMNode *next;
};

template <typename KEY, typename VALUE, typename F = KeyHash<K> > //实现代码中要加默认值KeyHash<K>？
class HashMap 
{
public:
    HashMap();
    int size();
    bool empty();
    int insert(KEY, VALUE);
    VALUE &find(KEY);
    int erase(KEY);
    void clear();
    
    VALUE &operator [] (const KEY&);

private:
    std::vector<HMNode<KEY, VALUE>*> head;
    int n;
	F hashFunc;
};


template <typename KEY, typename VALUE>
KEY HMNode<KEY, VALUE>::getKey() {
    return key;
}

template <typename KEY, typename VALUE>
VALUE HMNode<KEY, VALUE>::getValue() {
  return value;
}

template <typename KEY, typename VALUE, typename F> 
HashMap<KEY, VALUE>::HashMap()
:n(0), head(kHASHSIZE) 
{
    for (int i = 0; i < kHASHSIZE; i++) {
        HMNode<KEY, VALUE> *temp = new HMNode<KEY, VALUE>();
        head[i] = temp;
    }
}

template <typename KEY, typename VALUE, typename F> 
int HashMap<KEY, VALUE>::size() 
{
    return n;
}

template <typename KEY, typename VALUE, typename F> 
bool HashMap<KEY, VALUE>::empty() 
{
    return n == 0;
}

template <typename KEY, typename VALUE, typename F> 
int HashMap<KEY, VALUE>::insert(KEY key, VALUE value) 
{
    unsigned long long hv = hashFunc(key);
    int p = hv % kHASHSIZE;
    for (HMNode<KEY, VALUE> *ptr = head[p]; ptr->next; ptr = ptr->next)
	{
        if (ptr->next->key == key) 
			return 1;
	}
    HMNode<KEY, VALUE> *HMNode = new HMNode<KEY, VALUE>(key, value);
    HMNode->next = head[p]->next;
    head[p]->next = HMNode;
    ++n;
    return 0;
}

template <typename KEY, typename VALUE, typename F > 
VALUE &HashMap<KEY, VALUE>::find(KEY key) 
{
    unsigned long long hv = hashFunc(key);
    int p = hv % kHASHSIZE;
    for (HMNode<KEY, VALUE> *ptr = head[p]; ptr->next; ptr = ptr->next)
	{
        if (ptr->next->key == key) 
			return ptr->next->value;
	}
    return NULL;
}

template <typename KEY, typename VALUE, typename F> 
void HashMap<KEY, VALUE>::clear()
{
    n = 0;
    for (int i = 0; i < kHASHSIZE; i++) 
	{
        if (head[i]->next) delete head[i]->next;
    }
}

template <typename KEY, typename VALUE, typename F> 
int HashMap<KEY, VALUE>::erase(KEY key) 
{
    unsigned long long hv = hashFunc(key);
    int p = hv % kHASHSIZE;
    for (HMNode<KEY, VALUE> *ptr = head[p]; ptr->next; ptr = ptr->next)
	{
        if (ptr->next->key == key) 
		{
            HMNode<KEY, VALUE> *temp = ptr->next;
            ptr->next = temp->next;
            temp->next = NULL;
            delete temp;
            --n;
            return 0;
        }
	}
    return 1;
}

template<typename KEY, typename VALUE, typename F> 
VALUE &HashMap<KEY, VALUE>::operator [] (const KEY &index) 
{
    HMNode<KEY, VALUE> *ptr = find(index);
    if (ptr != NULL) 
		return ptr->value;
    VALUE *value = new VALUE();
    insert(index, *value);
    ptr = find(index);
    return ptr->value;
}

#endif
