#include "solver.h"
using namespace std;

map<string, double> vars;
const string filename = "vars.txt";

void saveVar(string& var, double& val);
void loadVars();

deque<string> parse(const std::string& str);
Queue<string> poliz(deque<string>& tokens);
double solve(Queue<string>& polized);

template<typename T>
std::string toString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void saveVar(string& var, double& val) {
    cout << "var " << var << "=" << val << endl;
    vars[var] = val;
    ofstream out(filename);
    out << vars.size() << endl;
    for(auto it: vars) {
        out << it.first << " " << it.second << endl;
    }
    out.close();
}
double calculate(const std::string& str) {
    //    cout << "calc " << str << endl;
    auto tokens = parse(str);
    //    cout << "Tokens: [";
    //    for(auto it: tokens) {
    //        cout << '\"' << it << "\",";
    //    }
    //    cout << ']' << endl;
    auto polized = poliz(tokens);
    /*while(!polized.isEmpty()){
        cout<<"\""<<polized.pop()<<"\",";
    }
    cout<<endl;*/
    double answer = solve(polized);
    //    cout << "Result: " << answer << endl;
    return answer;
}

void loadVars() {
    ifstream in(filename);
    if(!in.is_open()) {
        ofstream out(filename);
        out << 0;
        out.close();
    }
    size_t keys_count;
    in >> keys_count;
    string var;
    double val;
    for(size_t i = 0; i < keys_count; i++) {
        in >> var >> val;
        vars[var] = val;
    }
    in.close();
}

deque<string> parse(const std::string& str) {
    deque<string> tokens;
    bool isNumOrVar = false;
    string NumOrVar;
    for(size_t i = 0; i < str.length(); i++) {
        if((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') ||
           (str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
            NumOrVar += str[i];
            isNumOrVar = true;
        }
        if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '(' ||
           str[i] == ')') {
            if(isNumOrVar) {
                if(vars.find(NumOrVar) != vars.end())
                    tokens.push_back(toString(vars[NumOrVar]));
                else
                    tokens.push_back(NumOrVar);
                NumOrVar = "";
                isNumOrVar = false;
            }
            if(str[i] == '-' && (i > 0 ? str[i - 1] == '(' : true)) {
                tokens.push_back("0");
            }
            tokens.push_back(toString(str[i]));
        }
    }
    if(isNumOrVar) {
        if(vars.find(NumOrVar) != vars.end())
            tokens.push_back(toString(vars[NumOrVar]));
        else
            tokens.push_back(NumOrVar);
        NumOrVar = "";
        isNumOrVar = false;
    }
    return tokens;
}

Queue<string> poliz(deque<string>& tokens) {
    Queue<string> polized;
    Stack<string> stack;
    stack.push("0");
    for(size_t i = 0; i < tokens.size(); i++) {
        if(tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/") {
            if(stack.get() == "0" ||
               ((tokens[i] == "*" || tokens[i] == "/") &&
                (stack.get() == "+" || stack.get() == "-" || stack.get() == "(")) ||
               ((tokens[i] == "-" || tokens[i] == "+") && (stack.get() == "("))) {
                stack.push(tokens[i]);
            } else {
                while((tokens[i] == "+" || tokens[i] == "-") &&
                      (stack.get() == "*" || stack.get() == "/" || stack.get() == "+" ||
                       stack.get() == "-")) {
                    polized.push(stack.pop());
                }
                stack.push(tokens[i]);
            }
            continue;
        }
        if(tokens[i] == "(") {
            stack.push(tokens[i]);
            continue;
        }
        if(tokens[i] == ")") {
            while(stack.get() != "(") {
                polized.push(stack.pop());
            }
            stack.pop();
            continue;
        }
        polized.push(tokens[i]);
    }
    while(stack.get() != "0") {
        polized.push(stack.pop());
    }
    return polized;
}

double solve(Queue<string>& polized) {
    double res;
    Stack<string> stack;
    size_t size = polized.size();
    for(size_t i = 0; i < size; i++) {
        if(polized.get() == "+" || polized.get() == "-" || polized.get() == "*" ||
           polized.get() == "/") {
            double second = atof(stack.pop().c_str());
            double first = atof(stack.pop().c_str());
            if(polized.get() == "+") {
                stack.push(toString(first + second));
            }
            if(polized.get() == "-") {
                stack.push(toString(first - second));
            }
            if(polized.get() == "*") {
                stack.push(toString(first * second));
            }
            if(polized.get() == "/") {
                stack.push(toString(first / second));
            }
            polized.pop();
        } else {
            stack.push(polized.pop());
        }
    }
    res = atof(stack.pop().c_str());
    return res;
}
