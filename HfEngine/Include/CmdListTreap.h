/*
A treap for ID3D11CommandList
*/
#pragma once

#ifdef _TEST_CMD_LIST_
typedef int ID3D11CommandList;
template<class T>
class ComPtr {
    T *ptr;
    ComPtr() {
        ptr = nullptr;
    }
    T *Get() const {
        return ptr;
    }
    T **operator&() {
        Release();
        return &ptr;
    }
    void Release() {
        if (ptr)
            delete ptr;
    }
};
#else
#include "stdafx.h"
#include "DX.h"
#include <random>
#endif

#include "Utility/referptr.h"

class D3DDevice;
class RenderPipeline;


    class CmdListTreap : public Utility::ReferredObject {
        struct Node {
            ComPtr<ID3D11CommandList> list1;
            ComPtr<ID3D11CommandList> list2;
            
            volatile ComPtr<ID3D11CommandList> *p1;
            volatile ComPtr<ID3D11CommandList> *p2;
            inline Node() {
                p1 = &list1;
                p2 = &list2;
                size = 1, left = right = -1, weight = priority = 0;
            }

            inline void Swap() {
                p2 = (volatile ComPtr<ID3D11CommandList>*)InterlockedExchangePointer((volatile PVOID*)&p1, (PVOID)p2);
            }

            inline void Destroy() {
                list1.ReleaseAndGetAddressOf();
                list2.ReleaseAndGetAddressOf();
            }

            int size;
            int left, right;
            int priority;  //rendering priority

            int weight; //only for treap itself

        }*NodePool;
        int NodeRoot;
        inline int lsize(Node *n) {
            return (~n->left) ? NodePool[n->left].size : 0;
        }
        inline int rsize(Node *n) {
            return (~n->right) ? NodePool[n->right].size : 0;
        }
        inline void Pushup(Node *n) {
            size = 1+lsize(n) + rsize(n);
        }

        int size, capability;
        std::mutex lock;
        inline int GetRand() {
            static std::default_random_engine engine;
            static std::uniform_int_distribution<int> distribution(-2147483647, 2147483647);
            static auto mrand = std::bind(distribution, engine);
            return mrand();
        }
        inline Node *NewNode(int priority, int *id) {
            //std::lock_guard<std::mutex> g(lock);
            Node node;
            node.priority = priority;
            node.weight = GetRand();
            if (size + 1 > capability) {
                capability *= 2;
                Node *NewPool = (Node *)malloc(sizeof(Node) * capability);
                if(!NewPool)
                    throw std::bad_alloc();
                for(int i = 0; i < size; i++)
                   NewPool[i] = std::move(NodePool[i]);
                free(NodePool);
                NodePool = NewPool;
            }
            *id = size;
            size++;
            NodePool[*id] = std::move(node);
            return NodePool + (*id);
        }

        int NodeRank(int n, int value);
        int DoInsert(int xnode, int ynode);
        std::pair<int, int> DoInsert2(int n, int k);
        int DoErase();
        void DoClear();
        void DoRender(int u);
        Utility::ReferPtr<D3DDevice> device;
    public:
        void Initialize(D3DDevice *d) {
            device = d;
            size = 0;
            capability = 128;
            NodePool = (Node *)malloc(sizeof(Node)*capability); //yes, malloc...
            NodeRoot = -1;
        }
        void UnInitialize() {
            //
            device.Release();
        }
        void Release() {
            UnInitialize();
        }

        void Insert(RenderPipeline *rp, int priority);
        void SetPriority(RenderPipeline *rp, int newp);
        void Erase(RenderPipeline *rp);
        void Clear();
        void Render();
    };



