#include <iostream>
#include <stack>

int main() {
    std::stack<int> myStack;

    // 压入元素
    myStack.push(1);
    myStack.push(2);
    myStack.push(3);

    // 访问栈顶元素
    std::cout << "栈顶元素: " << myStack.top() << std::endl;

    // 弹出元素
    myStack.pop();
    std::cout << "弹出后的栈顶元素: " << myStack.top() << std::endl;

    // 检查栈是否为空
    if (!myStack.empty()) {
        std::cout << "栈不为空" << std::endl;
    }

    return 0;
}
