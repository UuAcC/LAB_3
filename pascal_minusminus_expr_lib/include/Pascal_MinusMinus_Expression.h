#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdint.h>

#include "TStack.h"
#include "TQueue.h"

#define PMM_EXPR Pascal_MinusMinus_Expression

#define ERROR PMM_EXPR::error

#define STATE PMM_EXPR::state
#define PARCER PMM_EXPR::Parcer

#define CHAR_IN_LC c >= 97 && c <= 122
#define CHAR_IN_UC c >= 65 && c <= 90
#define CHAR_IN_LETTERS CHAR_IN_LC || CHAR_IN_UC

// from dot to num
#define CHAR_IN_INTS tp >= 0 && tp <= 2
// from zero to num
#define CHAR_IN_INTS_ND tp >= 1 && tp <= 2
// from dot to var 
#define CHAR_IS_OPERAND tp >= 0 && tp <= 3
// from l_round_br to semicolon
#define CHAR_IS_OPERATION tp >= 4 && tp <= 12
// from equal to c_less_equal
#define CHAR_IS_COMP_OPER tp >= 13 && tp <= 19

#define LEXEM PMM_EXPR::lexem
#define LEX_TYPE PMM_EXPR::LexemType

using namespace std;
using std::string;
// Function pointer type for operations
using pmme_fp = double(*)(double, double);

class Visitor;

class Pascal_MinusMinus_Expression {
public:
    // Enumeration of possible lexeme types in arithmetic expression
    enum class LexemType {              
        dot, 
        zero, num,
        variable,
        l_round_br, r_round_br, l_curly_br, r_curly_br,
        add, sub, mul, div, semicolon,
        equal, c_equal, c_not_equal, c_more, c_more_equal, c_less, c_less_equal,
        keyword_while,
        keyword_if,
        keyword_else,
        keyword_print,                          // not yet
        not_a_lexem 
    };
    // Function that determines the type of a char
    static LexemType decode(char c);
    // Structure representing a lexeme with its value and type (for example: { "0", ZERO } )
    struct lexem {
        string value; LexemType type;
        lexem(string s = "0", LexemType t = LexemType::zero);
    };
    friend ostream& operator<<(ostream& ostr, const LEXEM& lex);
    friend ostream& operator<<(ostream& ostr, const LEX_TYPE& tp);


    // Structure representing an error with its position and value
    struct error {
        size_t index; string value;
        error(size_t i = 0, string v = "(A)");
        error(size_t i, char c);
    };
    friend ostream& operator<<(ostream& ostr, const ERROR& er);


    struct NodeRetVal {
        string var_name;
        double doub_val;
        bool bool_value;

        bool hv_name = false;
        bool hv_dval = false;
        bool hv_bval = false;

        NodeRetVal() {}
        NodeRetVal(string _vn) : var_name(_vn), hv_name(true) {}
        NodeRetVal(double _dv) : doub_val(_dv), hv_dval(true) {}
        NodeRetVal(bool _bv) : bool_value(_bv), hv_bval(true) {}

        inline explicit operator string() const { return var_name; }
        inline explicit operator double() const { return doub_val; }
        inline explicit operator bool() const { return bool_value; }

        inline void set_var_name(string _vn) { var_name = _vn; hv_name = true; }
        inline void set_doub_val(double _dv) { doub_val = _dv; hv_dval = true; }
        inline void set_bool_val(bool _bv) { bool_value = _bv; hv_bval = true; }
    };

    class Node; class BiNode; class Terminal;

    class Variable; class FPNumber;
    class LRBr; class RRBr; class LCBr; class RCBr;
    class AdSb; class MuDv; class Cop; class Eq; class SmCln;
    class WhileOp; class IfOp; class ElseOp;

    class Mon;
    class Pol;
    class Comp;
    class EqOper;
    class IfOper;
    class IfElse;
    class WhileOper;
    class Operator;
    class Expr;
    class ExprTree;

private:
    // States for finite state machine, STX - error state for throw
    enum state;
    // Class containing static methods for parsing string into lexeme queue
    class Parcer;

    string s_infix; // String representation of infix arithmetic expression
    TQueue<lexem> q_infix; // Infix notation represented as lexeme queue
    ExprTree* expression_tree;
public:
    Pascal_MinusMinus_Expression(string _infix);
    inline string get_s_infix() const { return s_infix; }
    inline TQueue<lexem> get_q_infix() const { return q_infix; }
    inline ExprTree* get_tree();
    // Executes the code
    void execute();
};

#include "ExprTreeNodes.h"
#include "Parcer.h"