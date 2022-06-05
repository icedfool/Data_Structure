#pragma once
#include <algorithm>
#include <list>
#include <iostream>
#include <stack>
using namespace std;

template<typename T>
class binary_tree
{
    class tree_node;

public:
    binary_tree(void) :root_(nullptr) {}//默认构造函数
    binary_tree(const T*, const int);//从数组构造一颗二叉树
    binary_tree(const binary_tree&);//拷贝构造函数
    binary_tree& operator = (const binary_tree&);
    ~binary_tree(void) { clear(); }//析构函数
public:
    bool insert(const T& data);//插入一个元素
    bool empty(void) const { return (NULL == root_); }//二叉树是否为空
    bool exists(const T& data) const;//判断元素是否存在
    void clear(void);//非递归清空二叉树
    void erase(const T& data);
    int size(void) const;//元素数量
    T minmum(void) const;//最小值
    T maxmum(void) const;//最大值
    template<typename T>
    friend ostream& operator<<(ostream& out, const binary_tree<T>& tree);//输出二叉树
    void print(ostream& out) const;//OK
    void print_in_order_nonrecursive(void) const;//非递归中序遍历输出二叉树
    void print_in_order_recursive(std::ostream& os) const;//递归中序遍历输出二叉树
    void print_successor_nonrecursive(void) const;//非递归按元素顺序输出二叉树
    void print_post_order_nonrecursive(void) const;//非递归后续遍历输出二叉树
    void print_pre_order(void) const;//非递归先序遍历输出二叉树
    int max_length_between_node(void) const;//最大节点距离
    int hight(void) const;//树高度
    bool operator==(const binary_tree& other) const;//两个树相等：结构相同，对应元素相同
    bool operator!=(const binary_tree& other) const { return !equal(other); }//两个树不相等
    bool equal(const binary_tree& other) const;//两个树相等：结构相同，对应元素相同

private:
    void print_binary_tree(ostream&, const tree_node* bt, int depth) const;//二叉树形式打印二叉树
    int count_node(const tree_node* ptree) const;//元素个数
    tree_node* find(const T& data);//查找
    tree_node* maxmum(tree_node*) const;//最大节点
    tree_node* minmum(tree_node*) const;//最小节点
    tree_node* successor(tree_node* t) const;//后继节点
    //节点的深度与高度：对于树中相同深度的每个结点来说，它们的高度不一定相同，这取决于每个结点下面的叶结点的深度
    int hight(const tree_node* _t) const;
    bool equal(const tree_node* lhs, const tree_node* rhs) const;//两个树相等：结构相同，对应元素相同
    bool is_node_leaf(const tree_node* node) const;
    bool is_leaf_node_equal(const tree_node* lhs, const tree_node* rhs) const;
    void copy(const binary_tree& other);
    void copy_node(tree_node*& dest, tree_node* dest_parent, const tree_node* from);
    void print_in_order_recursive(std::ostream& os, const tree_node* node) const;//递归中序遍历输出二叉树

private:
    struct tree_node//OK
    {
        tree_node() :data(T()), parent(NULL), left(NULL), right(NULL) {}
        tree_node(const T& t) :data(t), parent(NULL), left(NULL), right(NULL) {}
        T data;
        tree_node *parent, *left, *right;
    };

private:
    tree_node* root_;//OK
    int size_ = 0;
};

template<typename T>
binary_tree<T>::binary_tree(const T* arr, const int length) : binary_tree()
{
    for (int i = 0; i < length; i++)
    {
        insert(arr[i]);
    }
}

template<typename T>
inline binary_tree<T>::binary_tree(const binary_tree & from) :root_(nullptr)
{
    copy(from);
}

template<typename T>
binary_tree<T>& binary_tree<T>::operator=(const binary_tree & from)
{
    copy(from);
    return *this;
}

template<typename T>
bool binary_tree<T>::insert(const T& data)
{
    if (0 != root_)
    {
        tree_node *fast, *slow, *ptemp;
        fast = slow = ptemp = root_;

        while (fast != 0)
        {
            slow = fast;
            if (data < slow->data)
            {
                fast = slow->left;
            }
            else if (data > slow->data)
            {
                fast = slow->right;
            }
            else
            {
                fast = 0;
                return false;
            }
            //esle equal do nothing
        }
        if (data < slow->data)
        {
            slow->left = new tree_node(data);
            slow->left->parent = slow;
        }
        else if (data > slow->data)
        {
            slow->right = new tree_node(data);
            slow->right->parent = slow;
        }
        else
        {
            return false;
        }
        //esle equal do nothing
    }
    else
    {
        root_ = new tree_node(data);
    }
    ++size_;
    return true;
}

template<typename T>
int binary_tree<T>::hight(void) const
{
    return hight(root_);
}

template<typename T>
bool binary_tree<T>::operator==(const binary_tree & other) const
{
    return this->equal(other);
}

template<typename T>
bool binary_tree<T>::equal(const binary_tree & other) const
{
    return equal(root_, other.root_);
}
template<typename T>
bool binary_tree<T>::equal(const tree_node* lhs, const tree_node* rhs) const
{
    if (lhs == nullptr && rhs == nullptr)
    {
        return true;
    }
    if (is_leaf_node_equal(lhs, rhs))
    {
        return true;
    }
    if (lhs && rhs && lhs->data == rhs->data)
    {
        return equal(lhs->left, rhs->left) &&  equal(lhs->right, rhs->right);
    }
    return false;
}
template<typename T>
inline bool binary_tree<T>::is_node_leaf(const tree_node * node) const
{
    return node != nullptr && node->left == nullptr && node->right == nullptr;
}
template<typename T>
inline bool binary_tree<T>::is_leaf_node_equal(const tree_node * lhs
    , const tree_node * rhs) const
{
    if (is_node_leaf(lhs) && is_node_leaf(rhs))
    {
        return lhs->data == rhs->data;
    }
    return false;
}
template<typename T>
void binary_tree<T>::copy(const binary_tree & other)
{
    if (this == &other)
    {
        return;
    }
    clear();
    size_ = other.size_;
    if (other.root_)
    {
        copy_node(root_, nullptr, other.root_);
    }
}
template<typename T>
void binary_tree<T>::copy_node(tree_node *& dest, tree_node* dest_parent, const tree_node * from)
{
    if (from == nullptr)
    {
        return;
    }
    dest = new tree_node(from->data);
    dest->parent = dest_parent;

    copy_node(dest->left, dest, from->left);
    copy_node(dest->right, dest, from->right);
}

template<typename T>
int binary_tree<T>::hight(const tree_node* _t) const
{
    if (_t == nullptr)
    {
        return 0;
    }
    if (_t->left == nullptr && _t->right == nullptr)
    {
        return 1;
    }
    return 1 + std::max<int>(hight(_t->left), hight(_t->right));
}

template<typename T>
int binary_tree<T>::max_length_between_node(void) const
{
    int max_length = 0;

    const tree_node* ptree = root_;

    list<tree_node*> listNode;

    listNode.push_back(root_);
    while (!listNode.empty())
    {
        auto pnode = listNode.front();
        listNode.pop_front();
        if (pnode->left != nullptr)
        {
            listNode.push_back(pnode->left);
        }
        if (pnode->right != nullptr)
        {
            listNode.push_back(pnode->right);
        }
        int tempBetween = hight(pnode->left) + hight(pnode->right);

        max_length = std::max<int>(tempBetween, max_length);
    }


    return max_length;
}


template<typename T>
void binary_tree<T>::print_binary_tree(ostream& out, const tree_node* bt, int depth) const
{
    //用右左孩子的方式输出一颗树,先输出右孩子后输出左孩子
    if (bt)
    {
        print_binary_tree(out, bt->right, depth + 1);

        if (depth == 0)
        {
            out << bt->data << endl;
        }
        else if (depth == 1)
        {
            out << " --" << bt->data << endl;
        }
        else
        {
            int n = depth;
            while (--n)
            {
                cout << "    ";
            }
            out << " --" << bt->data << endl;
        }

        print_binary_tree(out, bt->left, depth + 1);
    }
}

template<typename T>
void binary_tree<T>::print(ostream& out) const
{
    print_binary_tree(out, root_, 0);
}

template<typename T>
void binary_tree<T>::print_in_order_nonrecursive(void) const
{
    cout << "print_in_order_nonrecursive : ";
    stack<tree_node*> tempstack;
    tree_node* t = root_;
    if (t != NULL)
    {
        do
        {
            tempstack.push(t);
            t = t->left;
        } while (t != NULL);
    }
    while (!tempstack.empty())
    {
        tree_node* p = tempstack.top();
        cout << p->data << " ";
        tempstack.pop();
        if (p->right != NULL)
        {
            p = p->right;
            do
            {
                tempstack.push(p);
                p = p->left;
            } while (p != NULL);
        }
    }
    cout << endl;
}

template<typename T>
inline void binary_tree<T>::print_in_order_recursive(std::ostream & os) const
{
    print_in_order_recursive(os, root_);
}
template<typename T>
void binary_tree<T>::print_in_order_recursive(std::ostream & os, const tree_node * node) const
{
    if (node == nullptr)
    {
        return;
    }
    print_in_order_recursive(os, node->left);
    os << node->data << " ";
    print_in_order_recursive(os, node->right);
}
template<typename T>
ostream& operator<<(ostream& out, const binary_tree<T>& tree)
{
    tree.print(out);
    return out;
}

template<typename T>
void binary_tree<T>::print_post_order_nonrecursive(void) const
{
    //后续序序遍历输出一颗树的全部结点值2,3,1
    //广度优先遍历
    cout << "print_post_order_nonrecursive : ";
    typedef pair<tree_node*, bool> multinode;
    stack<multinode> tempstack;
    if (root_)
    {
        tempstack.push(make_pair(root_, false));
    }
    while (!tempstack.empty())
    {
        multinode m = tempstack.top(); tempstack.pop();
        if (m.first->left == NULL && m.first->right == NULL)
        {//叶子节点直接输出
            cout << m.first->data << " ";
        }
        else if (m.second == true)
        {//所有孩子都遍历完了才会到这一步
            cout << m.first->data << " ";
        }
        else
        {//非终结点，并且孩子还没遍历完。
            m.second = true; tempstack.push(m);
            if (m.first->right != NULL)
            {
                tempstack.push(make_pair(m.first->right, false));
            }
            if (m.first->left != NULL)
            {
                tempstack.push(make_pair(m.first->left, false));
            }
        }
    }
    cout << endl;
}

template<typename T>
void binary_tree<T>::print_pre_order(void) const
{
    //先序遍历输出一颗树的全部结点值1,2,3,先根遍历
    cout << "print_pre_order : ";
    stack<tree_node*> node_stack;
    if (root_)
    {
        node_stack.push(root_);
        tree_node* t;
        while (!node_stack.empty())
        {
            t = node_stack.top();
            node_stack.pop();
            cout << t->data << " ";
            if (t->right != 0)
            {
                node_stack.push(t->right);
            }
            if (t->left != 0)
            {
                node_stack.push(t->left);
            }
        }
        cout << endl;
    }
}

template<typename T>
bool binary_tree<T>::exists(const T& data) const
{
    bool result = false;
    if (root_)
    {
        tree_node* pfind = root_;
        while (pfind)
        {
            if (pfind->data == data)
            {
                result = true;
                break;
            }
            else if (data < pfind->data)
            {
                pfind = pfind->left;
            }
            else
                pfind = pfind->right;
        }
    }
    return result;
}

template<typename T>
typename binary_tree<T>::tree_node* binary_tree<T>::find(const T& data)
{
    if (root_)
    {
        tree_node* pfind = root_;
        while (pfind)
        {
            if (pfind->data == data)
            {
                return pfind;
            }
            else if (data < pfind->data)
            {
                pfind = pfind->left;
            }
            else
                pfind = pfind->right;
        }
    }
    return NULL;
}

template<typename T>
void binary_tree<T>::clear(void)
{
    //先序遍历删除一颗树的全部结点值1,2,3,先根遍历
    if (root_)
    {
        stack<tree_node*> node_stack;
        node_stack.push(root_);
        tree_node* t;
        while (!node_stack.empty())
        {
            t = node_stack.top();
            node_stack.pop();
            if (t->right != 0)
            {
                node_stack.push(t->right);
            }
            if (t->left != 0)
            {
                node_stack.push(t->left);
            }
            delete t;
        }
        root_ = NULL;
    }
    size_ = 0;
}

template<typename T>
int binary_tree<T>::count_node(const tree_node* ptree) const
{
    int num1, num2;
    if (ptree == NULL)
        return 0;
    else
    {
        num1 = count_node(ptree->left);
        num2 = count_node(ptree->right);
        return (num1 + num2 + 1);
    }
}

template<typename T>
int binary_tree<T>::size(void) const
{
    return size_;
}

template<typename T>
T binary_tree<T>::minmum(void) const
{
    const tree_node* p = root_;
    while (p->left)
    {
        p = p->left;
    }
    return p->data;
}

template<typename T>
typename binary_tree<T>::tree_node* binary_tree<T>::minmum(tree_node* p) const
{
    while (p->left)
    {
        p = p->left;
    }
    return p;
}

template<typename T>
T binary_tree<T>::maxmum(void) const
{
    const tree_node* p = root_;
    while (p->right)
    {
        p = p->right;
    }
    return p->data;
}

template<typename T>
typename binary_tree<T>::tree_node* binary_tree<T>::maxmum(tree_node* t) const
{
    while (t->right)
    {
        t = t->right;
    }
    return t;
}

template<typename T>
typename binary_tree<T>::tree_node* binary_tree<T>::successor(tree_node* t) const
{
    if (t->right)
    {
        return minmum(t->right);
    }
    else
    {
        tree_node* fast = t->parent, *slow = t;
        while (fast && fast->right == slow)
        {
            slow = fast;
            fast = fast->parent;
        }
        return fast;
    }
}

template<typename T>
void binary_tree<T>::print_successor_nonrecursive(void) const
{
    cout << "print_successor_nonrecursive by order: ";
    if (!empty())
    {
        tree_node* p = minmum(root_);
        cout << p->data << " ";
        while (p = successor(p))
        {
            cout << p->data << " ";
        }
        cout << endl;
    }
}

template<typename T>
void binary_tree<T>::erase(const T& data)
{
    tree_node* iter = find(data);
    //delete iter;
}

