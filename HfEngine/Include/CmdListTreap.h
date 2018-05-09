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
#include <unordered_map>
#endif

#include "Utility/referptr.h"

class D3DDevice;
class RenderPipeline;


    class CmdListTreap : public Utility::ReferredObject {
        friend class RenderPipeline;
        struct Node {
            
            ComPtr<ID3D11CommandList> list1;
            ComPtr<ID3D11CommandList> list2;
            
            ComPtr<ID3D11CommandList> * volatile p1;
            ComPtr<ID3D11CommandList> * volatile p2;
            inline Node() {
                p1 = std::addressof(list1);
                p2 = std::addressof(list2);
                size = 1, left = right = nullptr, weight = priority = 0;
              
            }

            inline void Swap() {
                p2 = (ComPtr<ID3D11CommandList>*)InterlockedExchangePointer((volatile PVOID*)&p1, (PVOID)p2);
            }

            inline void Destroy() {
                list1.ReleaseAndGetAddressOf();
                list2.ReleaseAndGetAddressOf();
            }

            int size;
            int priority;  //rendering priority

            int weight; //only for treap itself
            Node *left, *right;
        }*NodeRoot = nullptr;
        
        inline int lsize(Node *n) {
            return (n->left) ? n->left->size : 0;
        }
        inline int rsize(Node *n) {
            return (n->right) ? n->right->size : 0;
        }
        inline void Pushup(Node *n) {
            n->size = 1+lsize(n) + rsize(n);
        }
        int size = 0;
        std::mutex lock;
        inline int GetRand() {
            static std::default_random_engine engine;
            static std::uniform_int_distribution<int> distribution(-32767, 32767);
            static auto mrand = std::bind(distribution, engine);
            return mrand();
        }
        inline Node *NewNode(int priority, int *id) {
            //std::lock_guard<std::mutex> g(lock);
            if(size + 1 > 4096)
                throw std::runtime_error("Treap : Too many nodes");
            size++;
            Node *node = new Node;
            node->priority = priority;
            node->weight = GetRand();
            /*
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
            */
            *id = node->weight;
            return node; 
        }

        int NodeRank(Node *x, int value);
        Node *DoInsert(Node *x, Node *y);
        std::pair<Node *, Node *> DoInsert2(Node *x, int k);
        int DoErase();
        void DoClear();
        void DoRender(Node *u);
        Utility::ReferPtr<D3DDevice> device;

    public:
        void Initialize(D3DDevice *d) {
            device = d;
            //capability = 128;
            //NodePool = (Node *)malloc(sizeof(Node)*capability); //yes, malloc...
            //NodeRoot = -1;
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

        void Swap(RenderPipeline*);
    };



