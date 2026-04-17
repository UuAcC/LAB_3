#pragma once
#include <vector>
#include <string>
#include <map>

#include "TStack.h"
#include "TQueue.h"

#include "ExprIterativeExecutor.h"

#define PMM_EXPR Pascal_MinusMinus_Expression

#define PARCER PMM_EXPR::Parcer

#define LEX_TYPE PMM_EXPR::LexemType
#define LEXEM PMM_EXPR::lexem

#define STATE PMM_EXPR::state
#define ERROR PMM_EXPR::error

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
#define CHAR_IS_OPERATION tp >= 4 && tp <= 10

using namespace std;
// Function pointer type for operations
using pmme_fp = double(*)(double, double);

class Pascal_MinusMinus_Expression {
public:
    // Enumeration of possible lexeme types in arithmetic expression
    enum class LexemType {              // in tree:
        dot, 
        zero, num,                              // fpnumber
        variable,                               // variable
        l_round_br, r_round_br,
        add_sub, mul, div, equal, semicolon,    // bioperation
        l_curly_br, r_curly_br,
        keyword_while,                          // operator_while
        keyword_if,                             // operator_if
        keyword_else,                           // operator_else
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
private:
    // States for finite state machine, STX - error state for throw
    enum state { ST0, ST1, ST2, ST3, ST4, ST5, STX };
    // Structure representing an error with its position and value
    struct error {
        size_t index; string value;
        error(size_t i = 0, string v = "(A)");
        error(size_t i, char c);
    };
    friend ostream& operator<<(ostream& ostr, const ERROR& er);
    // Class containing static methods for parsing string into lexeme queue
    class Parcer {
        // Name of the last executed function
        static string last_func;
        // Queue of errors that occurred during the last function execution
        static TQueue<ERROR> last_errors;

        // Operator priority mapping
        static map<string, int> priority;
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
    public:
        // Checks if errors occurred during the last function execution
        static inline bool errors_occured() { return !last_errors.isEmpty(); };
        // Prints error message to stderr with last function name and errors
        static void print_error_message();
        // Removes all spaces from the input string
        static string delete_spaces(const string& str);
        // Parses input: string -> lexeme queue
        static TQueue<LEXEM> parce_infix(const string& str);
        // Converts infix notation to postfix notation using shunting yard algorithm
        static TQueue<LEXEM> parce_postfix(const TQueue<LEXEM>& que);

        static Expr* parce_tree(const TQueue<LEXEM>& que);
        // Converts string representation of number to double value
        static double to_double(const string& str);
    };
private:
    string s_infix; // String representation of infix arithmetic expression
    TQueue<lexem> q_infix; // Infix notation represented as lexeme queue
    TQueue<lexem> q_postfix; // Postfix notation represented as lexeme queue
    Expr* expression_tree;
public:
    Pascal_MinusMinus_Expression(string _infix);
    inline string get_s_infix() const { return s_infix; }
    inline TQueue<lexem> get_q_infix() const { return q_infix; }
    inline const TQueue<lexem>& get_q_postfix() {
        if (q_postfix.isEmpty())
            q_postfix = Parcer::parce_postfix(q_infix);
        return q_postfix;
    }
    inline Expr* get_tree() {
        if (expression_tree == nullptr)
            expression_tree = Parcer::parce_tree(get_q_postfix());
        return expression_tree;
    }
    // Executes the code
    void execute();

    void print();
};