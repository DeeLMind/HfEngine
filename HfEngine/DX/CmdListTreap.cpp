#include "../Include/CmdListTreap.h"
#include "../Include/RenderPipeline.h"
#include "../Include/D3DDevice.h"

void CmdListTreap::Insert(RenderPipeline *rp, int priority) {
    std::lock_guard<std::mutex> g(lock);
    int id;
    auto node = NewNode(priority, &id);
    rp->pnode = node;
    auto p = DoInsert2(NodeRoot, NodeRank(NodeRoot, priority));
    NodeRoot = DoInsert(DoInsert(p.first, node), p.second); 
}
void CmdListTreap::Swap(RenderPipeline *rp) {
    rp->native_context->FinishCommandList(true, rp->pnode->p1->ReleaseAndGetAddressOf());
    rp->pnode->Swap();
}
void CmdListTreap::Clear() {

}
void CmdListTreap::Render() {
    std::lock_guard<std::mutex> g(lock);
    DoRender(NodeRoot);
}
void CmdListTreap::Erase(RenderPipeline *rp) {

}
void CmdListTreap::SetPriority(RenderPipeline *rp, int priority) {

}

int CmdListTreap::NodeRank(Node *n, int value) {
    if(!n)return 0;
    if(value < n->priority)return NodeRank(n->left, value);
    else return NodeRank(n->right, value) + 1 + lsize(n);
}

CmdListTreap::Node *CmdListTreap::DoInsert(Node *x, Node *y) {
    if (!x) {
        return y;
    }
    if (!y) {
        return x;
    }
    if (x->weight < y->weight) {
        x->right = DoInsert(x->right, y);
        Pushup(x);
        return x;
    }
    else {
        y->left = DoInsert(x, x->left);
        Pushup(y);
        return y;
    }
}

std::pair<CmdListTreap::Node *, CmdListTreap::Node *> CmdListTreap::DoInsert2(Node *n, int k)
{
    auto r = std::pair<Node *, Node *>(nullptr, nullptr);
    if(!n)return r;
    if (lsize(n) >= k) {
        r = DoInsert2(n->left, k);
        n->left = r.second;
        Pushup(n);
        r.second = n;
    }
    else {
        r = DoInsert2(n->right, k-lsize(n)-1);
        n->right = r.first;
        Pushup(n);
        r.first = n;
    }
    return r;
}

int CmdListTreap::DoErase() {
    return 0;
}

void CmdListTreap::DoClear() {

}

void CmdListTreap::DoRender(Node *u) {
    if(!u)return;
    if(u->right)DoRender(u->right);
    auto l = u->p1->Get();
    if(l)device->native_immcontext->ExecuteCommandList(l, false);
    if(u->left)DoRender(u->left);
}