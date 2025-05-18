#include <iostream>
#include <vector>
#include <string>

using namespace std;

// گره‌های درخت جست‌وجو
class Node
{
public:
    int id;
    string name;
    Node *left = nullptr;
    Node *right = nullptr;

    // سازنده
    Node(int inputId, string inputName)
    {
        id = inputId;
        name = inputName;
    }
};

class Tree
{
public:
    Node *root;

    Tree()
    {
        root = nullptr; // شروع درخت با هیچ
    }

    // اضافه کردن یه گره به درخت
    void add(int id, string name)
    {
        Node *newNode = new Node(id, name);

        if (root == nullptr)
        {
            root = newNode; // اولین گره ریشه میشه
            return;
        }

        Node *current = root;

        while (true)
        {
            if (id < current->id)
            {
                // بریم سمت چپ
                if (current->left == nullptr)
                {
                    current->left = newNode;
                    break;
                }
                current = current->left;
            }
            else if (id > current->id)
            {
                // بریم سمت راست
                if (current->right == nullptr)
                {
                    current->right = newNode;
                    break;
                }
                current = current->right;
            }
            else
            {
                // آی‌دی تکراریه، کاری نکن
                break;
            }
        }
    }

    // جست‌وجوی آی‌دی توی درخت
    bool find(int id)
    {
        Node *ptr = root;

        while (ptr)
        {
            if (ptr->id == id)
                return true;

            ptr = (id < ptr->id) ? ptr->left : ptr->right;
        }

        return false; // هیچی پیدا نشد
    }

    // حذف گره - بازگشتی
    Node *remove(Node *node, int id)
    {
        if (!node)
            return nullptr;

        if (id < node->id)
        {
            node->left = remove(node->left, id);
        }
        else if (id > node->id)
        {
            node->right = remove(node->right, id);
        }
        else
        {
            // گره مورد نظر پیدا شده
            if (!node->left)
                return node->right;
            else if (!node->right)
                return node->left;

            // گره با دو فرزند
            Node *succ = node->right;
            while (succ && succ->left)
                succ = succ->left;

            node->id = succ->id;
            node->name = succ->name;

            node->right = remove(node->right, succ->id);
        }

        return node;
    }

    void deleteById(int id)
    {
        root = remove(root, id);
    }

    void display(Node *node)
    {
        if (node)
        {
            cout << "(id = " << node->id << ", name = " << node->name << ") ";
            display(node->left);
            display(node->right);
        }
    }

    bool isEmpty()
    {
        return root == nullptr;
    }
};

// یک درخواست شامل آی‌دی و اولویت
class Request
{
public:
    int id;
    int priority;

    Request(int idVal, int pVal)
    {
        id = idVal;
        priority = pVal;
    }
};

class Heap
{
public:
    vector<Request> data;

    void insert(int id, int priority)
    {
        Request newReq(id, priority);
        data.push_back(newReq);

        int index = data.size() - 1;

        while (index > 0 && data[parent(index)].priority < data[index].priority)
        {
            swap(data[index], data[parent(index)]);
            index = parent(index);
        }
    }

    void removeTop()
    {
        if (data.size() == 0)
            return;

        data[0] = data.back(); // جایگزین کردن با آخرین عضو
        data.pop_back();

        int index = 0;
        while (true)
        {
            int maxIndex = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < data.size() && data[left].priority > data[maxIndex].priority)
                maxIndex = left;
            if (right < data.size() && data[right].priority > data[maxIndex].priority)
                maxIndex = right;

            if (maxIndex == index)
                break;

            swap(data[index], data[maxIndex]);
            index = maxIndex;
        }
    }

    void handleTop(Tree *tree)
    {
        if (!data.empty())
        {
            int idToRemove = data[0].id;
            removeTop();
            tree->deleteById(idToRemove);
        }
    }

    void show()
    {
        for (auto &req : data)
        {
            cout << "(id = " << req.id << ", priority = " << req.priority << ") ";
        }
        cout << endl;
    }

    void bumpPriority(int id, int newPriority)
    {
        for (int i = 0; i < data.size(); i++)
        {
            if (data[i].id == id && newPriority > data[i].priority)
            {
                data[i].priority = newPriority;

                while (i > 0 && data[parent(i)].priority < data[i].priority)
                {
                    swap(data[i], data[parent(i)]);
                    i = parent(i);
                }
                break;
            }
        }
    }

    int parent(int i)
    {
        return (i - 1) / 2;
    }

    int getSize()
    {
        return data.size();
    }

    bool isEmpty()
    {
        return data.empty();
    }
};

int main()
{
    Tree bst;
    Heap heap;

    // تست دستی
    /*
    bst.add(10, "VS Code");
    heap.insert(10, 4);

    bst.add(20, "Chrome");
    heap.insert(20, 9);

    bst.add(15, "Media Player");
    heap.insert(15, 7);

    cout << "Binary Tree:\n";
    bst.display(bst.root);
    cout << "\n\n";

    cout << "Max Heap:\n";
    heap.show();

    cout << "\nProcessing top priority...\n";
    heap.handleTop(&bst);

    cout << "After processing:\n";
    bst.display(bst.root);
    cout << "\n";
    heap.show();
    */

    // منوی ساده با سوییچ
    /*
    int choice = -1;
    while (choice != 0) {
        cout << "\nChoose an operation:\n";
        cout << "1. Add to BST\n";
        cout << "2. Add to Heap\n";
        cout << "3. Search in BST\n";
        cout << "4. Show BST\n";
        cout << "5. Show Heap\n";
        cout << "6. Delete from BST\n";
        cout << "7. Remove from Heap (top)\n";
        cout << "8. Process top (Heap -> BST delete)\n";
        cout << "9. Increase priority\n";
        cout << "0. Exit\n";

        cin >> choice;

        if (choice == 1) {
            int id;
            string name;
            cout << "Enter ID and Name: ";
            cin >> id >> name;
            bst.add(id, name);
        } else if (choice == 2) {
            int id, pr;
            cout << "Enter ID and Priority: ";
            cin >> id >> pr;
            heap.insert(id, pr);
        } else if (choice == 3) {
            int id;
            cout << "Enter ID to search: ";
            cin >> id;
            cout << "Found? " << bst.find(id) << "\n";
        } else if (choice == 4) {
            bst.display(bst.root);
        } else if (choice == 5) {
            heap.show();
        } else if (choice == 6) {
            int id;
            cout << "Enter ID to delete from BST: ";
            cin >> id;
            bst.deleteById(id);
        } else if (choice == 7) {
            heap.removeTop();
        } else if (choice == 8) {
            heap.handleTop(&bst);
        } else if (choice == 9) {
            int id, newPr;
            cout << "Enter ID and new priority: ";
            cin >> id >> newPr;
            heap.bumpPriority(id, newPr);
        } else if (choice == 0) {
            cout << "Bye!\n";
        } else {
            cout << "Invalid option!\n";
        }
    }
    */

    return 0;
}
