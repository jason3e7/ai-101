// ZeroJudge a017 - 五則運算 (+ - * / % 與括號, 整數運算, 先乘除後加減)
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
using namespace std;

int prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}
long long apply(long long a, long long b, char op) {
    switch (op) {
        case '+': return a + b; case '-': return a - b; case '*': return a * b;
        case '/': return a / b; case '%': return a % b;
    }
    return 0;
}
int main() {
    string line;
    while (getline(cin, line)) {
        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;
        stringstream ss(line);
        string tok;
        stack<long long> vals;
        stack<char> ops;
        auto doOp = [&]() {
            long long b = vals.top(); vals.pop();
            long long a = vals.top(); vals.pop();
            char o = ops.top(); ops.pop();
            vals.push(apply(a, b, o));
        };
        while (ss >> tok) {
            if (tok == "(") ops.push('(');
            else if (tok == ")") { while (!ops.empty() && ops.top() != '(') doOp(); if (!ops.empty()) ops.pop(); }
            else if (tok == "+" || tok == "-" || tok == "*" || tok == "/" || tok == "%") {
                char o = tok[0];
                while (!ops.empty() && ops.top() != '(' && prec(ops.top()) >= prec(o)) doOp();
                ops.push(o);
            } else vals.push(stoll(tok));
        }
        while (!ops.empty()) doOp();
        cout << vals.top() << "\n";
    }
    return 0;
}
