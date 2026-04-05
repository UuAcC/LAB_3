#include "Pascal_MinusMinus_Expression.h"

#define ZERO LexemType::zero
#define NUM LexemType::num
#define DOT LexemType::dot

#define VAR LexemType::variable

#define L_BR LexemType::l_br
#define R_BR LexemType::r_br
#define SMCLN LexemType::semicolon

#define BOP LexemType::bop
#define MUL LexemType::mul
#define DIV LexemType::div
#define EQ LexemType::equal

#define NAL LexemType::not_a_lexem

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
    pi_v_buffer.push_back(lexem(pi_s_buffer, type_num(pi_s_buffer)));
    pi_v_buffer.push_back(lexem(string(1, c), decode(c)));
    pi_s_buffer.clear();
}
inline void PARCER::pi_f3(char c) { pi_s_buffer += c; }

LEX_TYPE PARCER::type_num(string str) {
    if (str == "0") return ZERO;
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
        pi_v_buffer.push_back(lexem(pi_s_buffer, type_num(pi_s_buffer)));
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
    if (pvi == BOP) {
        buffer.insert(iterator, lexem("0", ZERO));
    }
    iterator = buffer.begin();
    for (int i = 1; i < buffer.size(); ++i) {
        pvi = buffer[i].type;
        if (buffer[i - 1].type == L_BR && (pvi == BOP)) {
            buffer.insert(iterator + i, lexem("0", ZERO));
        }
    }
    _infix = TQueue<lexem>(buffer);
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------

map<string, int> PARCER::priority{
    {";", 0}, {"=", 1}, {"(", 2}, {"+", 3}, {"-", 3}, {"*", 4}, {"/", 4}
};

TQueue<LEXEM> PARCER::parce_postfix(const TQueue<LEXEM>& in) {
    TQueue<lexem> inf(in); unary_handle(inf);

    size_t sz = inf.get_size();
    TQueue<lexem> postf(sz);
    TStack<lexem> st(sz);
    lexem stackItem;
    while (!inf.isEmpty()) {
        lexem item = inf.pop();
        switch (item.type) {
        case L_BR:
            st.push(item);
            break;
        case R_BR:
            stackItem = st.pop();
            while (stackItem.type != L_BR) {
                postf.push(stackItem);
                stackItem = st.pop();
            }
            break;
        case VAR:
        case ZERO:
        case NUM:
            postf.push(item); break;
        default:
            while (!st.isEmpty()) {
                stackItem = st.top();
                if (priority[item.value] <= priority[stackItem.value])
                    postf.push(st.pop());
                else { break; }
            } st.push(item);
        }
    }
    while (!st.isEmpty()) {
        stackItem = st.pop();
        postf.push(stackItem);
    }
    return postf;
}
// --------------------------------------------------------------------------------------------

//Expr* PARCER::parce_tree(const TQueue<LEXEM>& in) {
//    TQueue<lexem> inf(in); unary_handle(inf);
//
//    size_t sz = inf.get_size();
//
//    TStack<lexem> tempStack(sz);
//    TStack<Expr*> treeStack(sz);
//
//    lexem stackItem;
//    while (!inf.isEmpty()) {
//        lexem item = inf.pop();
//        switch (item.type) {
//        case L_BR:
//            tempStack.push(item);
//            break;
//        case R_BR:
//            stackItem = tempStack.pop();
//            while (stackItem.type != L_BR) {
//                Expr* r = treeStack.pop();
//                Expr* l = treeStack.pop();
//                Expr* newNode = init_bioperation(stackItem.value, l, r);
//                treeStack.push(newNode);
//                stackItem = tempStack.pop();
//            }
//            break;
//        case MUL:
//        case DIV:
//        case BOP:
//            while (!tempStack.isEmpty()) {
//                stackItem = tempStack.top();
//                if (priority[item.value] <= priority[stackItem.value]) {
//                    Expr* r = treeStack.pop();
//                    Expr* l = treeStack.pop();
//                    Expr* newNode = init_bioperation(tempStack.pop().value, l, r);
//                    treeStack.push(newNode);
//                }
//                else { break; }
//            } tempStack.push(item);
//            break;
//        default:
//            treeStack.push(init_fpnumber(item.value));
//        }
//    }
//    while (!tempStack.isEmpty()) {
//        stackItem = tempStack.pop();
//        Expr* r = treeStack.pop();
//        Expr* l = treeStack.pop();
//        Expr* newNode = init_bioperation(stackItem.value, l, r);
//        treeStack.push(newNode);
//    }
//    Expr* res = treeStack.pop();
//    return res;
//}

Expr* PARCER::parce_tree(const TQueue<LEXEM>& que) {
    TQueue<lexem> postf(que);
    size_t sz = postf.get_size();
    
    TStack<Expr*> treeStack(sz);
    while (!postf.isEmpty()) {
        lexem item = postf.pop();
        if (item.type == VAR || item.type == NUM || item.type == ZERO) {
            treeStack.push(init_fpnumber(item.value));
        }
        else {
            Expr* r = treeStack.pop();
            Expr* l = treeStack.pop();
            Expr* newNode = init_bioperation(item.value, l, r);
            treeStack.push(newNode);
        }
    }
    return treeStack.pop();
}
