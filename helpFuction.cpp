#include "widget.h"
#include <vector>
#include <QRegularExpression>
#include "./ui_widget.h"

using namespace std;

bool Widget::judgeLegal(QString &exp) {
    vector<QChar> legalopt{'+', '-', '*', '/', '(', ')', '.',' '};
    int len = exp.length();
    if (len <= 0) {
        exp = "不能输入空表达式";
        return false;
    }

    if (exp[0] != '(' && !exp[0].isDigit() && exp[0] != '-' && exp[0] != '.') {
        exp = "闹呢兄弟";
        return false;
    }

    if (exp[len - 1] != ')' && !exp[len - 1].isDigit() && exp[len - 1] != '.') {
        exp = "闹呢兄弟";
        return false;
    }

    bool flag = false;
    for (auto &i: legalopt) {
        if (exp.contains(i)) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        exp = "就写个数字,闹呢?";
        return false;
    }

    QStack<QChar> stack;
    for (int i = 0; i < len; ++i) {
        QChar c = exp[i];
        if (c == ' ') continue;

        if (!c.isDigit() && std::find(legalopt.begin(), legalopt.end(), c) == legalopt.end()) {
            exp = "闹呢兄弟";
            return false;
        }

        if (c.isDigit() || c == '.') {
            int j = i;
            int dotCount = 0;
            while (j < len && (exp[j].isDigit() || exp[j] == '.')) {
                if (exp[j] == '.') {
                    if (++dotCount > 1) {
                        exp = "数字中多个小数点";
                        return false;
                    }
                }
                j++;
            }
            i = j - 1;
        }

        if (c == '(') {
            stack.push(c);
            if (i + 1 < len) {
                QChar next = exp[i + 1];
                int nextIndex = i + 1;
                while (nextIndex < len && exp[nextIndex] == ' ') {
                    nextIndex++;
                    if (nextIndex >= len) break;
                    next = exp[nextIndex];
                }

                if (next == '+' || next == '*' || next == '/' || next == ')') {
                    exp = "闹呢兄弟";
                    return false;
                }
            }
        } else if (c == ')') {
            if (stack.isEmpty() || stack.pop() != '(') {
                exp = "闹呢兄弟";
                return false;
            }
            if (i + 1 < len) {
                QChar next = exp[i + 1];
                int nextIndex = i + 1;
                while (nextIndex < len && exp[nextIndex] == ' ') {
                    nextIndex++;
                    if (nextIndex >= len) break;
                    next = exp[nextIndex];
                }

                if (next.isDigit() || next == '(' || next == '.' ) {
                    exp = "闹呢兄弟";
                    return false;
                }
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (i + 1 >= len) {
                exp = "闹呢兄弟";
                return false;
            }
            QChar next = exp[i + 1];
            int nextIndex = i + 1;
            while (nextIndex < len && exp[nextIndex] == ' ') {
                nextIndex++;
                if (nextIndex >= len) break;
                next = exp[nextIndex];
            }

            if (next == '+' || next == '-' || next == '*' || next == '/' || next == ')') {
                exp = "不支持两个运算符";
                return false;
            }
            if (c == '/' && next == '0') {
                int j = i + 1;
                while (j < len && (exp[j] == '0' || exp[j] == '.' || exp[j] == ' ')) {
                    j++;
                }
                if (j >= len || (!exp[j].isDigit() && exp[j] != '.')) {
                    exp = "0做分母,闹麻了";
                    return false;
                }
            }
        }
    }

    if (stack.isEmpty()) {
        return true;
    } else {
        exp = "不能成对,让我很苦恼啊";
        return false;
    }
}

void Widget::calculate(QString &exp) {
    QStack<QChar> opts;
    QStack<double> vals;

    auto priority = [](QChar op)-> int {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    };

    auto calculate = [&]() {
        if (vals.length() < 2 || opts.isEmpty()) {
            return;
        }
        double right = vals.pop();
        double left = vals.pop();
        QChar opt = opts.pop();

        switch (opt.unicode()) {
            case '+': vals.push(left + right);
                break;
            case '-': vals.push(left - right);
                break;
            case '*': vals.push(left * right);
                break;
            case '/':
                if (qFuzzyIsNull(right)) {
                    exp = "除零错误";
                    return;
                }
                vals.push(left / right);
                break;
        }
    };

    for (int i = 0; i < exp.length(); ++i) {
        QChar c = exp[i];

        if (c ==' ') continue;

        if (c == '-' && (i == 0 || exp[i-1] == '(')) {
            // 将 -a 转换为 0 - a
            vals.push(0);
            opts.push('-');
            continue;
        }

        if (c.isDigit() || c == '.') {
            QString numStr;
            //多次检查,这是防止judgeLegal误判(DeepSeek强烈推荐)
            int dotCount = 0;
            while (i < exp.length() && (exp[i].isDigit() || exp[i] == '.')) {
                if (exp[i] == '.') {
                    if (++dotCount > 1) {
                        exp = "数字中多个小数点";
                        return;
                    }
                }
                numStr.append(exp[i]);
                ++i;
            }
            --i;
            //判断toDouble成功没
            bool ok;
            double num = numStr.toDouble(&ok);
            if (ok) {
                vals.push(num);
            } else {
                exp = "无效数字格式";
                return;
            }
        } else if (c == '(') {
            opts.push(c);
        } else if (c == ')') {
            while (!opts.isEmpty() && opts.top() != '(') {
                calculate();
            }
            if (!opts.isEmpty() && opts.top() == '(') {
                opts.pop();
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            //这里很关键
            while (!opts.isEmpty() && priority(opts.top()) >= priority(c)) {
                calculate();
            }
            opts.push(c);
        }
    }
    while (!opts.isEmpty()) {
        calculate();
    }
    if (vals.size() == 1 && opts.isEmpty()) {
        // 格式化结果，移除多余的零和小数点(我还不太明白,DeepSeek教的)
        exp = QString::number(vals.pop(), 'f', 10).replace(QRegularExpression("\\.?0+$"), "");
    } else {
        exp = "兄弟表达式不对,闹呢";
    }
}
