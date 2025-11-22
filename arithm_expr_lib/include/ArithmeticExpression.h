#pragma once
#include <vector>
#include <string>
#include <map>

#include "TStack.h"
#include "TQueue.h"

#define PARCER ArithmeticExpression::Parcer
#define ANALYZER ArithmeticExpression::Analyzer

#define TYPE ArithmeticExpression::Type
#define LEXEM ArithmeticExpression::lexem

#define STATE ArithmeticExpression::state
#define ERROR ArithmeticExpression::error

using namespace std;
// Function pointer type for arithmetic operations
using arithm_fp = double(*)(double, double);

class ArithmeticExpression {
public:
    // Enumeration of possible lexeme types in arithmetic expression
    enum class Type { dot, zero, num, l_br, r_br, bop, mul, div };
    // Structure representing a lexeme with its value and type (for example: { "0", ZERO } )
    struct lexem {
        string value; Type type;
        lexem(string s = "0", Type t = Type::zero);
    };
    friend ostream& operator<<(ostream& ostr, const LEXEM& lex);
    friend ostream& operator<<(ostream& ostr, const TYPE& tp);
private:
    // States for finite state machine, STX - error state for throw
    enum state { ST0, ST1, ST2, ST3, STX };
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
        // Function that determines the type of a character
        static TYPE decode(char c);
        // Function that determines if a string represents a number type
        static TYPE type_num(string str);

        // --------------------------------------------------------------------------------------------
        //                Finite state machine for parce_infix function:
        // 
        //    |------|---------------|--------|      |------|---------------|--------|
        //    | next | [+,-,*,/,(,)] | [0..9] |      | call | [+,-,*,/,(,)] | [0..9] |
        //    |------|---------------|--------|      |------|---------------|--------|
        //    | ST0  |      ST0      |   ST1  |      | ST0  |      f0       |   f1   |
        //    |------|---------------|--------|      |------|---------------|--------|
        //    | ST1  |      ST0      |   ST1  |      | ST1  |      f2       |   f3   |
        //    |------|---------------|--------|      |------|---------------|--------|

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
        static map<TYPE, map<state, state>> td_next;

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
        // Converts string representation of number to double value
        static double to_double(const string& str);
    };
    // Class containing static methods for analyzing arithmetic expression correctness
    class Analyzer {
        // Name of the last executed function
        static string last_func;
        // Queue of errors that occurred during the last function execution
        static TQueue<ERROR> last_errors;

        // --------------------------------------------------------------------------------------------
        //                           Finite state machine for syntax_check function:
        // 
        //    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
        //    | next | [+, -] | * | / | [1..9] | 0 | ( | ) |    | call | [+, -] | * | / | [1..9] | 0 | ( | ) |   
        //    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
        //    | ST0  |   ST2  |STx|STx|  ST1   |ST1|ST0|STx|    | ST0  |   f0   |f1 |f1 |  f0    |f0 |f0 |f1 |
        //    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
        //    | ST1  |   ST2  |ST2|ST3|  STx   |STx|STx|ST1|    | ST1  |   f0   |f0 |f0 |  f1    |f1 |f1 |f0 |
        //    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
        //    | ST2  |   STx  |STx|STx|  ST1   |ST1|ST0|STx|    | ST2  |   f1   |f1 |f1 |  f0    |f0 |f0 |f1 |
        //    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|
        //    | ST3  |   STx  |STx|STx|  ST1   |STx|ST0|STx|    | ST3  |   f1   |f1 |f1 |  f0    |f1 |f0 |f1 |
        //    |------|--------|---|---|--------|---|---|---|    |------|--------|---|---|--------|---|---|---|

        // Returns function pointer from sc_funcs based on current state and lexeme type
        static void (*sc_call(STATE st, TYPE tp))(LEXEM);
        // Array containing function pointers for syntax_check finite state machine
        static void (*sc_funcs[])(LEXEM);

        // Returns next state from sc_states based on current state and lexeme type
        static STATE sc_next(STATE st, TYPE tp);
        // Array containing state transitions for syntax_check finite state machine
        static STATE sc_states[];

        // Helper functions for syntax_check finite state machine

        static inline void sc_f0(LEXEM lex);
        static inline void sc_f1(LEXEM lex);
    public:
        // Checks if errors occurred during the last function execution
        static inline bool errors_occured() { return !last_errors.isEmpty(); };
        // Prints error message to stderr with last function name and errors
        static void print_error_message();
        // Checks parentheses balance in the arithmetic expression
        static bool skobochniy_check(const string& str);
        // Performs lexical analysis of arithmetic expression (uses PARCER::to_double)
        static bool lexic_check(TQueue<LEXEM> que);
        // Performs syntactic analysis of arithmetic expression
        static bool syntax_check(TQueue<LEXEM> que);
    };
private:
    string s_infix; // String representation of infix arithmetic expression
    TQueue<lexem> q_infix; // Infix notation represented as lexeme queue
    TQueue<lexem> q_postfix; // Postfix notation represented as lexeme queue

    // Arithmetic operations used in calculate method via ae_call

    static inline double add(double arg1, double arg2) { return arg1 + arg2; }
    static inline double sub(double arg1, double arg2) { return arg1 - arg2; }
    static inline double mul(double arg1, double arg2) { return arg1 * arg2; }
    static inline double div(double arg1, double arg2) { return arg1 / arg2; }

    // Returns function pointer for arithmetic operation based on operator string
    arithm_fp ae_call(string str);
public:
    ArithmeticExpression(string _infix);
    inline string get_s_infix() const { return s_infix; }
    inline TQueue<lexem> get_q_infix() const { return q_infix; }
    inline const TQueue<lexem>& get_q_postfix() {
        if (q_postfix.isEmpty())
            q_postfix = Parcer::parce_postfix(q_infix);
        return q_postfix;
    }
    // Function that runs all analyzes, returns true if all checks passed
    bool run_analyzer();
    // Calculates the value of the arithmetic expression
    double calculate();
};