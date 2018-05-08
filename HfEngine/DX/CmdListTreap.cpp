#include "../Include/CmdListTreap.h"
#include "../Include/RenderPipeline.h"
#include "../Include/D3DDevice.h"

void CmdListTreap::Insert(RenderPipeline *rp, int priority) {
    std::lock_guard<std::mutex> g(lock);
    int id;
    NewNode(priority, &id);
    rp->tid = id; //bind the id
    auto p = DoInsert2(NodeRoot, NodeRank(NodeRoot, priority));
    NodeRoot = DoInsert(DoInsert(p.first, id), p.second); 
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

int CmdListTreap::NodeRank(int n, int value) {
    if(n == -1)return 0;
    if(value < NodePool[n].priority)return NodeRank(NodePool[n].left, value);
    else return NodeRank(NodePool[n].right, value) + 1 + lsize(NodePool+n);
}

int CmdListTreap::DoInsert(int x, int y) {
    if (x == -1) {
        Pushup(NodePool + y);
        return x;
    }
    if (y == -1) {
        Pushup(NodePool + x);
        return x;
    }
    if (NodePool[x].weight < NodePool[y].weight) {
        NodePool[x].right = DoInsert(NodePool[x].right, y);
        Pushup(NodePool+x);
        return x;
    }
    else {
        NodePool[y].left = DoInsert(x, NodePool[y].left);
        Pushup(NodePool+y);
        return y;
    }
}

std::pair<int, int> CmdListTreap::DoInsert2(int n, int k)
{
    auto r = std::pair<int, int>(-1, -1);
    if(n == -1)return r;
    if (lsize(NodePool + n) >= k) {
        r = DoInsert2(NodePool[n].left, k);
        NodePool[n].left = r.second;
        Pushup(NodePool+n);
        r.second = n;
    }
    else {
        r = DoInsert2(NodePool[n].right, k-lsize(NodePool+n)-1);
        NodePool[n].right = r.first;
        Pushup(NodePool+n);
        r.first = n;
    }
    return r;
}

int CmdListTreap::DoErase() {
    return 0;
}

void CmdListTreap::DoClear() {

}

void CmdListTreap::DoRender(int u) {
    if(u == -1)return;
    if(NodePool[u].right != -1)DoRender(NodePool[u].right);
    device->native_immcontext->ExecuteCommandList(NodePool[u].list1.Get(), false);
    if(NodePool[u].left != -1)DoRender(NodePool[u].left);
}