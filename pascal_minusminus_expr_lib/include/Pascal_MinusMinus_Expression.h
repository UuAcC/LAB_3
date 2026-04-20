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
#define CHAR_IS_OPERATION tp >= 4 && tp <= 11


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
        l_round_br, r_round_br,
        add, sub, mul, div, equal, semicolon,
        l_curly_br, r_curly_br,
        c_equal, c_not_equal, c_more, c_more_equal, c_less, c_less_equal,
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


    class Node {
    public:
        virtual NodeRetVal accept(Visitor* v) = 0;
        virtual inline int getCode() const = 0;
    };

    class BiNode : public Node {
    protected:
        Node* left = nullptr;
        Node* right = nullptr;
    public:
        BiNode(Node* l, Node* r = nullptr);
        Node* getLeft() const;
        Node* getRight() const;
        virtual NodeRetVal accept(Visitor* v) = 0;
    };

    class Terminal : public Node {
    protected:
        lexem il;
    public:
        Terminal() : il(lexem()) {}
        Terminal(lexem _l) : il(_l) {}
        virtual NodeRetVal accept(Visitor* v) = 0;
        virtual inline lexem getIL() const { return il; }
    };


    class Variable : public Terminal {
    public:
        Variable(lexem _n) : Terminal(_n) {}
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 2; }
        string inline getName() const { return il.value; }
    };
    class FPNumber : public Terminal {
    public:
        FPNumber(lexem _val) : Terminal(_val) {}
        double inline getVal() const { return std::stod(il.value); }
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 3; }
    };
    class LRBr : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 5; }
    };
    class RRBr : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 7; }
    };
    class LCBr : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 11; }
    };
    class RCBr : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 13; }
    };
    class AdSb : public Terminal {
    public:
        AdSb(lexem _tp) : Terminal(_tp) {}
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 17; }
        inline LexemType getType() const { return il.type; }
    };
    class MuDv : public Terminal {
    public:
        MuDv(lexem _tp) : Terminal(_tp) {}
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 19; }
        inline LexemType getType() const { return il.type; }
    };
    class Cop : public Terminal {
    public:
        Cop(lexem _tp) : Terminal(_tp) {}
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 23; }
        inline LexemType getType() const { return il.type; }
    };
    class Eq : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 29; }
    };
    class SmCln : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 31; }
    };
    class WhileOp : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 37; }
    };
    class IfOp : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 43; }
    };
    class ElseOp : public Terminal {
    public:
        virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
        virtual inline int getCode() const override { return 47; }
    };

    class Mon : public BiNode {
        LexemType op;
    public:
        Mon(Terminal* l);
        Mon(Mon* l, LexemType _op, Terminal* r);
        LexemType getOp() const;
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 53; }
    };

    class Pol : public BiNode {
        LexemType op;
    public:
        Pol(Mon* l);
        Pol(Pol* l, LexemType _op, Mon* r);
        LexemType getOp() const;
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 59; }
    };

    class Comp : public BiNode {
        LexemType op;
    public:
        Comp(Pol* l, LexemType _op, Pol* r);
        LexemType getOp() const;
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 61; }
    };

    class EqOper : public BiNode {
    public:
        EqOper(Variable* l, Pol* r);
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 67; }
    };

    class Operator;
    class Expr : public BiNode {
    public:
        Expr(Operator* l);
        Expr(Expr* l, Operator* r);
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 71; }
    };

    class IfOper : public BiNode {
    public:
        IfOper(Comp* l, Expr* r);
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 73; }
    };

    class IfElse : public BiNode {
    public:
        IfElse(IfOper* l, Expr* r = nullptr);
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 79; }
    };

    class Operator : public BiNode {
    public:
        Operator(EqOper* l);
        Operator(Comp* l, Expr* r);
        Operator(IfElse* l);
        virtual NodeRetVal accept(Visitor* v) override;
        virtual inline int getCode() const override { return 83; }
    };

    class ExprTree : public Node {
        Node* child;
    public:
        ExprTree(Expr* c);
        virtual NodeRetVal accept(Visitor* v) override;
    };



    private:
    // States for finite state machine, STX - error state for throw
    enum state { ST0, ST1, ST2, ST3, ST4, ST5, STX };
    // Class containing static methods for parsing string into lexeme queue
    class Parcer {
        // Name of the last executed function
        static string last_func;
        // Queue of errors that occurred during the last function execution
        static TQueue<ERROR> last_errors;

        static LEX_TYPE determine_lex_type(string str);

        // --------------------------------------------------------------------------------------------
        //                Finite state machine for parce_infix function:
        // 
        //    |------|-------------------|-------------|      |------|-------------------|-------------|
        //    | next | [+,-,*,/,(,),;,=] | ints & vars |      | call | [+,-,*,/,(,),;,=] | ints & vars |
        //    |------|-------------------|-------------|      |------|-------------------|-------------|
        //    | ST0  |        ST0        |     ST1     |      | ST0  |        f0         |     f1      |
        //    |------|-------------------|-------------|      |------|-------------------|-------------|
        //    | ST1  |        ST0        |     ST1     |      | ST1  |        f2         |     f3      |
        //    |------|-------------------|-------------|      |------|-------------------|-------------|

        // Buffer for accumulating digits during parce_infix execution
        static string pi_s_buffer;
        // Buffer for storing lexemes during parce_infix execution
        static vector<LEXEM> pi_v_buffer;

        // Determines the next state for parce_infix finite state machine
        static state pi_next(char input);

        // Returns the function to call based on current state and input character
        static void (*pi_call(state st, char c))(char);

        // Helper functions for parce_infix finite state machine

        static inline void pi_f0(char c);
        static inline void pi_f1(char c);
        static inline void pi_f2(char c);
        static inline void pi_f3(char c);
        // --------------------------------------------------------------------------------------------

        // --------------------------------------------------------------------------------------------
        //        Finite state machine for to_double function:
        // 
        //    |------|---|--------|---|   |------|---|--------|
        //    | next | 0 | [1..9] | . |   | call | . | [0..9] |
        //    |------|---|--------|---|   |------|---|--------|
        //    | ST0  |ST2|  ST1   |STx|   | ST0  | f3|   f0   |
        //    |------|---|--------|---|   |------|---|--------|
        //    | ST1  |ST1|  ST1   |ST3|   | ST1  | f2|   f0   |
        //    |------|---|--------|---|   |------|---|--------|
        //    | ST2  |STx|  STx   |ST3|   | ST2  | f2|   f3   |
        //    |------|---|--------|---|   |------|---|--------|
        //    | ST3  |ST3|  ST3   |STx|   | ST3  | f3|   f1   |
        //    |------|---|--------|---|   |------|---|--------|

        // Buffer for accumulating numeric value during to_double execution
        static double buffer_value;
        // Power of 10 divisor for decimal part in to_double
        static double buffer_dot;

        // State transition table for to_double finite state machine
        static map<LEX_TYPE, map<state, state>> td_next;

        // Returns the function to call based on current state and input character for to_double
        static void (*td_call(state st, char c))(char);

        // Helper functions for to_double finite state machine

        static inline void td_f0(char c);
        static inline void td_f1(char c);
        static inline void td_f2(char c);
        static inline void td_f3(char c);
        static inline double switch_char(char c);
        // --------------------------------------------------------------------------------------------

        // Function that handles unary operators by adding zero before them
        static void unary_handle(TQueue<LEXEM>& _infix) noexcept;

        // --------------------------------------------------------------------------------------------
        //        Functions needed to bottom-up parsing
       
        // 
        static Terminal* init_terminal_node(lexem lex);
        static bool reduce(TStack<Node*>* stack, TQueue<Terminal*>* queue);
        static bool shift(TStack<Node*>* stack, TQueue<Terminal*>* queue);
    public:
        // Checks if errors occurred during the last function execution
        static inline bool errors_occured() { return !last_errors.isEmpty(); };
        // Prints error message to stderr with last function name and errors
        static void print_error_message();
        // Removes all spaces from the input string
        static string delete_spaces(const string& str);
        // Parses input: string -> lexem queue
        static TQueue<LEXEM> parce_infix(const string& str);
        // Parses lexem queue -> ExprTree
        static ExprTree* bottomup_parce_tree(const TQueue<LEXEM>& que);
        // Converts string representation of number to double value
        static double to_double(const string& str);
    };
private:
    string s_infix; // String representation of infix arithmetic expression
    TQueue<lexem> q_infix; // Infix notation represented as lexeme queue
    ExprTree* expression_tree;
public:
    Pascal_MinusMinus_Expression(string _infix);
    inline string get_s_infix() const { return s_infix; }
    inline TQueue<lexem> get_q_infix() const { return q_infix; }
    inline const ExprTree*& get_tree() const;
    // Executes the code
    void execute();
};