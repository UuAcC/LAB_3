#include "Parcer.h"
#include "LexTypeDefines.h"

using parcer_fp = void(*)(char);

// ----------------------------------- Error handling ---------------------------------------

string PARCER::last_func;
TQueue<ERROR> PARCER::last_errors;

void PARCER::print_error_message() {
    cerr << last_func << " failure:" << endl;
    cerr << "Errors: " << last_errors << endl;
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------

string PARCER::delete_spaces(const string& str) {
    string res;
    for (char c : str) {
        if (c == ' ') continue;
        res += c;
    }
    return res;
}
// --------------------------------------------------------------------------------------------

// -------------------------- Finite state machine for PARCE_INFIX ----------------------------

string PARCER::pi_s_buffer;
vector<LEXEM> PARCER::pi_v_buffer;

inline void PARCER::pi_f0(char c) { pi_v_buffer.push_back(lexem(string(1, c), decode(c))); }
inline void PARCER::pi_f1(char c) { pi_s_buffer = string(1, c); }
inline void PARCER::pi_f2(char c) {
    pi_v_buffer.push_back(lexem(pi_s_buffer, determine_lex_type(pi_s_buffer)));
    pi_v_buffer.push_back(lexem(string(1, c), decode(c)));
    pi_s_buffer.clear();
}
inline void PARCER::pi_f3(char c) { pi_s_buffer += c; }
inline void PARCER::pi_f4(char c) {
    pi_v_buffer.push_back(lexem(pi_s_buffer, decode(pi_s_buffer[0])));
    pi_v_buffer.push_back(lexem(string(1, c), decode(c)));
    pi_s_buffer.clear();
}
inline void PARCER::pi_f5(char c) {
    pi_v_buffer.push_back(lexem(pi_s_buffer, decode(pi_s_buffer[0])));
    pi_s_buffer = string(1, c);
}
inline void PARCER::pi_f6(char c) {
    pi_s_buffer += c;
    pi_v_buffer.push_back(lexem(pi_s_buffer, determine_lex_type(pi_s_buffer)));
    pi_s_buffer.clear();
}
inline void PARCER::pi_f7(char c) {
    pi_v_buffer.push_back(lexem(pi_s_buffer, determine_lex_type(pi_s_buffer)));
    pi_s_buffer = string(1, c);
}

LEX_TYPE PARCER::determine_lex_type(string str) {
    if (str == "0") return ZERO;
    else if (str == "while") return WHILE;
    else if (str == "if") return IF;
    else if (str == "else") return ELSE;
    else if (str == "==") return C_EQ;
    else {
        if (str[0] == '<') {
            switch (str[1]) {
            case '=': return C_LE;
            case '>': return C_NEQ;
            default: throw str[1];
            }
        }
        else if (str[0] == '>') {
            if (str[0] == '=') return C_ME;
            else throw str[1];
        }
        else {
            for (char c : str) {
                if (CHAR_IN_LETTERS) return VAR;
            } return NUM;
        }
    }
}

STATE PARCER::pi_next(char c) {
    int tp = (int)decode(c);
    if (CHAR_IS_OPERATION) { return ST0; }
    if (CHAR_IS_OPERAND) { return ST1; }
    if (CHAR_IS_COMP_OPER) { return ST2; }
    else throw c;
}

parcer_fp PARCER::pi_call(state st, char c) {
    int tp = (int)decode(c);
    if (st == ST0) {
        if (CHAR_IS_OPERATION) { return pi_f0; }
        if (CHAR_IS_OPERAND) { return pi_f1; }
        if (CHAR_IS_COMP_OPER) { return pi_f1; }
        else throw c;
    }
    else if (st == ST1) {
        if (CHAR_IS_OPERATION) { return pi_f2; }
        if (CHAR_IS_OPERAND) { return pi_f3; }
        if (CHAR_IS_COMP_OPER) { return pi_f7; }
        else throw c;
    }
    else {
        if (CHAR_IS_OPERATION) { return pi_f4; }
        if (CHAR_IS_OPERAND) { return pi_f5; }
        if (CHAR_IS_COMP_OPER) { return pi_f6; }
        else throw c;
    }
}

TQueue<LEXEM> PARCER::parce_infix(const string& str) {
    last_func = __func__;
    last_errors.clear();
    TQueue<ERROR> curr_errors(str.size());

    pi_v_buffer.clear();
    pi_s_buffer.clear();
    // Process each character in the string
    state st = ST0;
    for (size_t i = 0; i < str.size(); ++i) {
        try {
            pi_call(st, str[i])(str[i]);
            st = pi_next(str[i]);
        }
        catch (char c) { curr_errors.push(error(i, c)); }
    }
    if (!pi_s_buffer.empty())
        pi_v_buffer.push_back(lexem(pi_s_buffer, determine_lex_type(pi_s_buffer)));
    pi_s_buffer.clear();
    // Convert vector to queue for result
    TQueue<lexem> res(pi_v_buffer);
    pi_v_buffer.clear();
    return res;
}
// --------------------------------------------------------------------------------------------

// -------------------------- Finite state machine for TO_DOUBLE ------------------------------

double PARCER::buffer_value = 0.0;
double PARCER::buffer_dot = 1.0;
// std::function<void(state, char)> 
map<LEX_TYPE, map<STATE, STATE>> PARCER::td_next{
    //          ST0         ST1         ST2         ST3
    {ZERO, { {ST0, ST2}, {ST1, ST1}, {ST2, STX}, {ST3, ST3} } },
    {NUM, { {ST0, ST1}, {ST1, ST1}, {ST2, STX}, {ST3, ST3} } },
    {DOT, { {ST0, STX}, {ST1, ST3}, {ST2, ST3}, {ST3, STX} } }
};

parcer_fp PARCER::td_call(state st, char c) {
    int tp = (int)decode(c);
    switch (st) {
    case ST0:
        if (CHAR_IN_INTS_ND) { return td_f0; }
        else { return td_f3; }
    case ST1:
        if (CHAR_IN_INTS_ND) { return td_f0; }
        else { return td_f2; }
    case ST2:
        if (CHAR_IN_INTS_ND) { return td_f3; }
        else { return td_f2; }
    case ST3:
        if (CHAR_IN_INTS_ND) { return td_f1; }
        else { return td_f3; }
    default: return td_f3;
    }
}

inline void PARCER::td_f0(char c) { buffer_value *= 10; buffer_value += switch_char(c); }
inline void PARCER::td_f1(char c) { buffer_dot *= 10; td_f0(c); }
inline void PARCER::td_f2(char c) { /* Do nothing */ }
inline void PARCER::td_f3(char c) { throw c; }

inline double PARCER::switch_char(char c) {
    switch (c) {
    case '1': return 1.0;
    case '2': return 2.0;
    case '3': return 3.0;
    case '4': return 4.0;
    case '5': return 5.0;
    case '6': return 6.0;
    case '7': return 7.0;
    case '8': return 8.0;
    case '9': return 9.0;
    case '0': return 0.0;
    default: throw c;
    }
}

double PARCER::to_double(const string& str) {
    last_func = __func__;
    last_errors.clear();
    TQueue<ERROR> curr_errors(str.size());

    buffer_dot = 1.0;
    buffer_value = 0.0;

    state st = ST0;
    for (size_t i = 0; i < str.size(); ++i) {
        try {
            td_call(st, str[i])(str[i]);
            st = td_next[decode(str[i])][st];
        }
        catch (char c) { curr_errors.push(error(i, c)); }
    }

    double res = buffer_value / buffer_dot;
    last_errors = curr_errors;
    return res;
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------

void PARCER::unary_handle(TQueue<LEXEM>& _infix) noexcept {
    vector<lexem> buffer(_infix.to_vector());
    auto iterator = buffer.begin();
    LEX_TYPE pvi = buffer[0].type;
    if (pvi == ADD || pvi == SUB) {
        buffer.insert(iterator, lexem("0", ZERO));
    }
    iterator = buffer.begin();
    for (int i = 1; i < buffer.size(); ++i) {
        pvi = buffer[i].type;
        if (buffer[i - 1].type == L_R_BR && (pvi == ADD || pvi == SUB)) {
            buffer.insert(iterator + i, lexem("0", ZERO));
        }
    }
    _infix = TQueue<lexem>(buffer);
}
// --------------------------------------------------------------------------------------------

PMM_EXPR::Terminal* PARCER::init_terminal_node(lexem lex) {
    switch (lex.type) {
    case ZERO:
    case NUM: return new FPNumber(lex);
        case VAR: return new Variable(lex);
    case ADD:
    case SUB: return new AdSb(lex);
    case MUL:
    case DIV: return new MuDv(lex);
    case EQ: return new Eq();
        case C_EQ:
        case C_NEQ:
        case C_M:
        case C_ME:
        case C_L:
        case C_LE: return new Cop(lex);
    case SMCLN: return new SmCln();
        case L_R_BR: return new LRBr();
        case R_R_BR: return new RRBr();
        case L_C_BR: return new LCBr();
        case R_C_BR: return new RCBr();
    case WHILE: return new WhileOp();
    case IF: return new IfOp();
    case ELSE: return new ElseOp();
    }
}

bool PARCER::reduce(TStack<Node*>* stack, TQueue<Terminal*>* queue) {
    int res = 1;
    TStack<Node*> stackOfStashed(7);
    while (!stack->isEmpty()) {
        Node* stackItem = stack->pop();
        res *= stackItem->getCode();
        switch (res) {
        case 2: { 
            if (!stack->isEmpty() && stack->top()->getCode() == 19
                || (!queue->isEmpty() && queue->top()->getCode() == 29)) {
                stackOfStashed.push(stackItem); break;
            }
            Variable* term = static_cast<Variable*>(stackItem);
            stack->push(new Mon(term));
            return true;
        } 
        case 3: { 
            if (!stack->isEmpty() && stack->top()->getCode() == 19) { 
                stackOfStashed.push(stackItem); break;
            }
            FPNumber* term = static_cast<FPNumber*>(stackItem);
            stack->push(new Mon(term));
            return true;
        }
        case 2014: { 
            Mon* left = cast<Mon>(stackItem, "Incorrect lexemes order: tried to cast Mon");
            MuDv* op = cast<MuDv>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Mul/Div");
            Variable* right = cast<Variable>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Variable");
            stack->push(new Mon(left, op->getType(), right));
            return true;
        } 
        case 3021: {
            Mon* left = cast<Mon>(stackItem, "Incorrect lexemes order: tried to cast Mon");
            MuDv* op = cast<MuDv>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Mul/Div");
            FPNumber* right = cast<FPNumber>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast FPNumber");
            stack->push(new Mon(left, op->getType(), right));
            return true;
        } 
        case 1855: {
            Mon* mon = cast<Mon>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Mon");
            Node* right = mon->getRight();
            if (right != nullptr) {
                Variable* rght = dynamic_cast<Variable*>(right);
                if (rght == nullptr)
                    stack->push(new Mon(static_cast<Mon*>(mon->getLeft()), mon->getOp(), static_cast<FPNumber*>(right)));
                else stack->push(new Mon(static_cast<Mon*>(mon->getLeft()), mon->getOp(), rght));
            }
            else {
                Node* left = mon->getLeft();
                Variable* lft = dynamic_cast<Variable*>(left);
                if (lft == nullptr)
                    stack->push(new Mon(static_cast<FPNumber*>(left)));
                else stack->push(new Mon(lft));
            }
            return true;
        }
        case 53: {
            if ((!stack->isEmpty() && stack->top()->getCode() == 17) 
                || (!queue->isEmpty() && queue->top()->getCode() == 19))
            {
                stackOfStashed.push(stackItem); break;
            } 
            Mon* mon = static_cast<Mon*>(stackItem);
            stack->push(new Pol(mon));
            return true;
        }
        case 53159: {
            if (!queue->isEmpty() && queue->top()->getCode() == 19) { 
                stackOfStashed.push(stackItem); break; 
            }
            Pol* left = cast<Pol>(stackItem, "Incorrect lexemes order: tried to cast Pol");
            AdSb* op = cast<AdSb>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Add/Sub");
            Mon* right = cast<Mon>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Mon");
            stack->push(new Pol(left, op->getType(), right));
            return true;
        }
        case 2065: {
            Pol* pol = cast<Pol>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Pol");
            Mon* right = static_cast<Mon*>(pol->getRight());
            if (right != nullptr)
                stack->push(new Pol(static_cast<Pol*>(pol->getLeft()), pol->getOp(), right));
            else stack->push(new Pol(static_cast<Mon*>(pol->getLeft())));
            return true;
        }
        case 80063: {
            if (!queue->isEmpty() && queue->top()->getCode() == 17) {
                stackOfStashed.push(stackItem); break;
            }
            Pol* left = cast<Pol>(stackItem, "Incorrect lexemes order: tried to cast Pol");
            Cop* op = cast<Cop>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast compare operation");
            Pol* right = cast<Pol>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Pol");
            stack->push(new Comp(left, op->getType(), right));
            return true;
        }
        case 2135: {
            if (!queue->isEmpty() && queue->top()->getCode() == 11) {
                stackOfStashed.push(stackItem); break;
            }
            Comp* cmp = cast<Comp>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Comp");
            stack->push(new Comp(static_cast<Pol*>(cmp->getLeft()), cmp->getOp(), static_cast<Pol*>(cmp->getRight())));
            return true;
        }
        case 3422: {
            if (!queue->isEmpty() && queue->top()->getCode() == 17) {
                stackOfStashed.push(stackItem); break;
            }
            Variable* left = cast<Variable>(stackItem, "Incorrect lexemes order: tried to cast Variable");
            stackOfStashed.pop();
            Pol* right = cast<Pol>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Pol)");
            stack->push(new EqOper(left, right));
            return true;
        }
        case 932096165: {
            stackOfStashed.pop(); 
            Comp* cond = cast<Comp>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Comp");
            stackOfStashed.pop();
            stackOfStashed.pop();
            Expr* expr = cast<Expr>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Expr");
            stack->push(new IfOper(cond, expr));
            return true;
        }
        case 73: {
            if (!queue->isEmpty() && queue->top()->getCode() == 47) {
                stackOfStashed.push(stackItem); break;
            }
            IfOper* iop = static_cast<IfOper*>(stackItem);
            stack->push(new IfElse(iop));
            return true;
        }
        case 34834943: {
            IfOper* iop = cast<IfOper>(stackItem, "Incorrect lexemes order: tried to cast IfOper");
            stackOfStashed.pop();
            stackOfStashed.pop();
            Expr* expr = cast<Expr>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Expr");
            stack->push(new IfElse(iop, expr));
            return true;
        }
        case 888742855: {
            stackOfStashed.pop();
            Comp* cond = cast<Comp>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Comp");
            stackOfStashed.pop();
            stackOfStashed.pop();
            Expr* expr = cast<Expr>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Expr");
            stack->push(new WhileOper(cond, expr));
            return true;
        }
        case 67: {
            EqOper* eop = static_cast<EqOper*>(stackItem);
            stack->push(new Operator(eop));
            return true;
        }
        case 79: {
            IfElse* ieop = static_cast<IfElse*>(stackItem);
            stack->push(new Operator(ieop));
            return true;
        }
        case 89: {
            WhileOper* wop = static_cast<WhileOper*>(stackItem);
            stack->push(new Operator(wop));
            return true;
        }
        case 83: {
            if (!stack->isEmpty() && stack->top()->getCode() == 31) {
                stackOfStashed.push(stackItem); break;
            }
            Operator* op = static_cast<Operator*>(stackItem);
            stack->push(new Expr(op));
            return true;
        }
        case 182683: {
            Expr* expr = cast<Expr>(stackItem, "Incorrect lexemes order: tried to cast Expr");
            stackOfStashed.pop();
            Operator* op = cast<Operator>(stackOfStashed.pop(), "Incorrect lexemes order: tried to cast Operator");
            stack->push(new Expr(expr, op));
            return true;
        }
        case 71: {
            if (!queue->isEmpty()) { 
                stackOfStashed.push(stackItem); break; 
            } 
            Expr* expr = static_cast<Expr*>(stackItem);
            stack->push(new ExprTree(expr));
            return true;
        }
        default: 
            stackOfStashed.push(stackItem);
        }
    }
    while (!stackOfStashed.isEmpty())
        stack->push(stackOfStashed.pop());
    return false;
}

bool PARCER::shift(TStack<Node*>* stack, TQueue<Terminal*>* queue) {
    if (!queue->isEmpty()) { stack->push(queue->pop()); return true; }
    else { return false; }
}

PMM_EXPR::ExprTree* PARCER::bottomup_parce_tree(const TQueue<LEXEM>& in_que) {
    TQueue<lexem> inf = in_que;
    TQueue<Terminal*> queue(in_que.get_size());
    while (!inf.isEmpty()) {
        queue.push(init_terminal_node(inf.pop()));
    }
    TStack<Node*> stack(inf.get_size());
    bool shift_result = true;
    while (!queue.isEmpty() || shift_result) {
        if (!reduce(&stack, &queue)) {
            shift_result = shift(&stack, &queue);
            if (!shift_result) {
                ExprTree* res = dynamic_cast<ExprTree*>(stack.pop());
                if (!res) {
                    TStack<Node*> debugs = stack;
                    cout << "stack: ";
                    while (!debugs.isEmpty())
                        cout << debugs.pop()->getCode() << " ";
                    TQueue<Terminal*> debugq = queue;
                    cout << "\nqueue: ";
                    while (!debugq.isEmpty())
                        cout << debugq.pop()->getCode() << " ";
                    cout << endl;
                    throw runtime_error("BOTTOM-UP TREE PARSING ERROR");
                }
                else return res;
            }
        }
    }
}

// мне хочется плакать


