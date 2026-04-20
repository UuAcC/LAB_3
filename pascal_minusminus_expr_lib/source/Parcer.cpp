#include "Pascal_MinusMinus_Expression.h"

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

LEX_TYPE PARCER::determine_lex_type(string str) {
    if (str == "0") return ZERO;
    else if (str == "while") return WHILE;
    for (char c : str) {
        if (CHAR_IN_LETTERS) return VAR;
    } return NUM;
}

STATE PARCER::pi_next(char c) {
    int tp = (int)decode(c);
    if (CHAR_IS_OPERATION) { return ST0; }
    if (CHAR_IS_OPERAND) { return ST1; }
    else throw c;
}

parcer_fp PARCER::pi_call(state st, char c) {
    int tp = (int)decode(c);
    if (st == ST0) {
        if (CHAR_IS_OPERATION) { return pi_f0; }
        if (CHAR_IS_OPERAND) { return pi_f1; }
        else throw c;
    }
    else if (st == ST1) {
        if (CHAR_IS_OPERATION) { return pi_f2; }
        if (CHAR_IS_OPERAND) { return pi_f3; }
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
    int64_t res = 1;
    TStack<Node*> currSt(7);
    while (!stack->isEmpty()) {
        Node* stackItem = stack->top();
        res *= stackItem->getCode();
        switch (res) {
        case 2: {
            if (queue->top()->getCode() == 19) {
                currSt.push(stackItem);
                stack->pop();
                break;
            }
            Variable* term = static_cast<Variable*>(stackItem);
            stack->push(new Mon(term));
            return true;
        } // я запутался
        case 3: {
            if (queue->top()->getCode() == 19) {
                currSt.push(stackItem);
                stack->pop();
                break;
            }
            FPNumber* term = static_cast<FPNumber*>(stackItem);
            stack->push(new Mon(term));
            return true;
        }
        case 2014: {
            Mon* left = static_cast<Mon*>(stackItem);
            MuDv* op = static_cast<MuDv*>(currSt.pop());
            Variable* right = static_cast<Variable*>(currSt.pop());
            stack->push(new Mon(left, op->getType(), right));
            return true;
        } 
        case 3021: {
            Mon* left = static_cast<Mon*>(stackItem);
            MuDv* op = static_cast<MuDv*>(currSt.pop());
            FPNumber* right = static_cast<FPNumber*>(currSt.pop());
            stack->push(new Mon(left, op->getType(), right));
            return true;
        } 
        case 53: {
            Mon* mon = static_cast<Mon*>(stackItem);
            stack->push(new Pol(mon));
            return true;
        }
        case 53159: {
            Pol* left = static_cast<Pol*>(stackItem);
            AdSb* op = static_cast<AdSb*>(currSt.pop());
            Mon* right = static_cast<Mon*>(currSt.pop());
            stack->push(new Pol(left, op->getType(), right));
            return true;
        }
        case 0b000000000000000000000: {

        }
        case 0b000000000000000000000: {

        }
        default: 
            currSt.push(stackItem);
            stack->pop();
        }
    }
    while (!currSt.isEmpty())
        stack->push(currSt.pop());
    return false;
}

bool PARCER::shift(TStack<Node*>* stack, TQueue<Terminal*>* queue) {

}

PMM_EXPR::ExprTree* PARCER::bottomup_parce_tree(const TQueue<LEXEM>& in_que) {
    TQueue<lexem> inf = in_que;
    TQueue<Terminal*> queue;
    while (!inf.isEmpty()) {
        queue.push(init_terminal_node(inf.pop()));
    }
    TStack<Node*> stack(inf.get_size());
    while (!queue.isEmpty()) {
        if (!reduce(&stack, &queue)) {
            if (!shift(&stack, &queue)) {
                ExprTree* res = dynamic_cast<ExprTree*>(stack.pop());
                if (!res) throw "PARSING ERROR";
                else return res;
            }
        }
    }
}


