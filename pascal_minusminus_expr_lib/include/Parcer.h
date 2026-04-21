#pragma once
#include "Pascal_MinusMinus_Expression.h"

// States for finite state machine, STX - error state for throw
enum PMM_EXPR::state { ST0, ST1, ST2, ST3, ST4, ST5, STX };
// Class containing static methods for parsing string into lexeme queue
class PMM_EXPR::Parcer {
    // Name of the last executed function
    static string last_func;
    // Queue of errors that occurred during the last function execution
    static TQueue<ERROR> last_errors;

    static LEX_TYPE determine_lex_type(string str);

    // --------------------------------------------------------------------------------------------
    //                Finite state machine for parce_infix function:
    // 
    //    |------|-----------------------------|-------------|-----------|      
    //    | next | [+, -, *, /, (, ), ;, {, }] | ints & vars | [=, <, >] |  
    //    |------|-----------------------------|-------------|-----------|      
    //    | ST0  |             ST0             |     ST1     |    ST2    |
    //    |------|-----------------------------|-------------|-----------|      
    //    | ST1  |             ST0             |     ST1     |    ST2    |
    //    |------|-----------------------------|-------------|-----------|
    //    | ST2  |             ST0             |     ST1     |    ST2    |
    //    |------|-----------------------------|-------------|-----------|   
    // 
    //    |------|-----------------------------|-------------|-----------|      
    //    | call | [+, -, *, /, (, ), ;, {, }] | ints & vars | [=, <, >] |
    //    |------|-----------------------------|-------------|-----------|      
    //    | ST0  |             f0              |     f1      |    f1     |
    //    |------|-----------------------------|-------------|-----------|      
    //    | ST1  |             f2              |     f3      |    f7     |
    //    |------|-----------------------------|-------------|-----------|
    //    | ST2  |             f4              |     f5      |    f6     |
    //    |------|-----------------------------|-------------|-----------|      

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
    static inline void pi_f4(char c);
    static inline void pi_f5(char c);
    static inline void pi_f6(char c);
    static inline void pi_f7(char c);
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

    // converts lexem into Terminal*
    static Terminal* init_terminal_node(lexem lex);
    // is used to dynamic cast 
    template<class Type> static Type* cast(Node* nod);

    static bool reduce(TStack<Node*>* stack, TQueue<Terminal*>* queue);
    static bool shift(TStack<Node*>* stack, TQueue<Terminal*>* queue);
    // --------------------------------------------------------------------------------------------

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

template<class Type>
inline Type* PARCER::cast(Node* nod) {
    Type* res = dynamic_cast<Type*>(nod);
    if (res == nullptr) throw "BOTTOM-UP TREE PARSING ERROR";
    // здесь можно сделать виртуальный метод, возвращающий название или че-то такое
    else return res;
}