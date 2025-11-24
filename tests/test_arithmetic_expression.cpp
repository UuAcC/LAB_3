#include "ArithmeticExpression.h"
#include <gtest/gtest.h>

// Специализация не создавалась, т.к. шаблон в глобальном пространстве имен, 
// а специализация в ArithmeticExpression.
// Она выглядела так:
// 
// template<>
// ostream& operator<<(std::ostream& ostr, TQueue<LEXEM>& q) { тут какой-то код }

// Далее пытался сделать перегрузку, но уперся в ошибку:
// error C2084: функция "std::ostream &TQueue<ArithmeticExpression::lexem>::operator <<(std::ostream &,TQueue<ArithmeticExpression::lexem> &)" уже имеет текст реализации

// В итоге оказалось, что я час крутил функцию, которая мне в дальнейшем будет не нужна.
// Вряд ли я это даже покажу.

void print_lexem_queue(ostream& ostr, TQueue<LEXEM>& q) {
	TQueue<LEXEM> tmp(q);
	size_t sz = q.get_size();
	TQueue<string> vals(sz);
	TQueue<TYPE> tps(sz);
	while (!tmp.isEmpty()) {
		LEXEM curr = tmp.pop();
		vals.push(curr.value);
		tps.push(curr.type);
	}
	ostr << "values: " << vals;
	ostr << endl;
	ostr << "types: " << tps;
}

// ((325 - 45.231) * 78 - 23434 / 1.00)

TEST(ArithmeticExpression, Previous_main) {
	string str;
	/*getline(cin, str);*/
	str = "-)-3242  (+ 3*(43  - 2723).4353)";

	ArithmeticExpression _(str);
	bool run_res = _.run_analyzer(1);
	if (run_res) {
		cout << endl;
		TQueue<LEXEM> inf = _.get_q_infix();
		print_lexem_queue(cout, inf);
		cout << endl << endl;
		TQueue<LEXEM> postf = _.get_q_postfix();
		print_lexem_queue(cout, postf);
		cout << endl << endl;
		cout << "Result: " << _.calculate() << endl;
	}
}

#include <cmath>

TEST(ArithmeticExpression, constructor_creates_object) {
    EXPECT_NO_THROW(ArithmeticExpression expr("1+2"));
}

TEST(ArithmeticExpression, get_s_infix_returns_correct_string) {
    ArithmeticExpression expr("1 + 2 * 3");
    EXPECT_EQ(expr.get_s_infix(), "1+2*3");
}

TEST(ArithmeticExpression, get_q_infix_returns_queue) {
    ArithmeticExpression expr("1+2");
    TQueue<LEXEM> infix = expr.get_q_infix();
    EXPECT_FALSE(infix.isEmpty());
    EXPECT_GE(infix.get_size(), 1);
}

TEST(ArithmeticExpression, get_q_postfix_works) {
    ArithmeticExpression expr("1+2");
    TQueue<LEXEM> postfix1 = expr.get_q_postfix();
    TQueue<LEXEM> postfix2 = expr.get_q_postfix();
    EXPECT_FALSE(postfix1.isEmpty());
    EXPECT_FALSE(postfix2.isEmpty());
    EXPECT_EQ(postfix1.get_size(), postfix2.get_size());
    while (!postfix1.isEmpty()) {
        LEXEM l1 = postfix1.pop(), l2 = postfix2.pop();
        EXPECT_EQ(l1.type, l2.type); EXPECT_EQ(l1.value, l2.value);
    }
}

TEST(ArithmeticExpression, run_analyzer_returns_true_for_valid_expression) {
    ArithmeticExpression expr("1+2");
    EXPECT_TRUE(expr.run_analyzer());
}

TEST(ArithmeticExpression, run_analyzer_returns_true_for_complex_valid_expression) {
    ArithmeticExpression expr("(3+4)*2-5/1");
    EXPECT_TRUE(expr.run_analyzer());
}

TEST(ArithmeticExpression, run_analyzer_returns_true_with_skobki) {
    ArithmeticExpression expr("((1+2)*3)");
    EXPECT_TRUE(expr.run_analyzer());
}

TEST(ArithmeticExpression, run_analyzer_returns_true_for_doubles) {
    ArithmeticExpression expr("1.5+2.5");
    EXPECT_TRUE(expr.run_analyzer());
}

TEST(ArithmeticExpression, calculate_simple_addition) {
    ArithmeticExpression expr("2+3");
    expr.run_analyzer();
    double res = expr.calculate();
    EXPECT_DOUBLE_EQ(res, 5.0);
}

TEST(ArithmeticExpression, calculate_simple_subtraction) {
    ArithmeticExpression expr("5-2");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 3.0);
}

TEST(ArithmeticExpression, calculate_simple_multiplication) {
    ArithmeticExpression expr("3*4");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 12.0);
}

TEST(ArithmeticExpression, calculate_simple_division) {
    ArithmeticExpression expr("8/2");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 4.0);
}

TEST(ArithmeticExpression, calculate_division_with_drobi) {
    ArithmeticExpression expr("5/2");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 2.5);
}

TEST(ArithmeticExpression, calculate_with_skobki_1) {
    ArithmeticExpression expr("(2+3)*4");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 20.0);
}

TEST(ArithmeticExpression, calculate_with_skobki_2) {
    ArithmeticExpression expr("((1+2)*3)");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 9.0);
}

TEST(ArithmeticExpression, calculate_with_skobki_3) {
    ArithmeticExpression expr("(3+4)*(2-1)");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 7.0);
}

TEST(ArithmeticExpression, calculate_positive_unary) {
    ArithmeticExpression expr("+5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 5.0);
}

TEST(ArithmeticExpression, calculate_negative_unary) {
    ArithmeticExpression expr("-5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), -5.0);
}

TEST(ArithmeticExpression, calculate_unary_in_expression) {
    ArithmeticExpression expr("3+(-2)");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 1.0);
}

TEST(ArithmeticExpression, calculate_complex_unary_operations) {
    ArithmeticExpression expr("5-(-3)");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 8.0);
}

TEST(ArithmeticExpression, calculate_drobnoye_addition) {
    ArithmeticExpression expr("1.5+2.5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 4.0);
}

TEST(ArithmeticExpression, calculate_drobnoye_multiplication) {
    ArithmeticExpression expr("1.5*2");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 3.0);
}

TEST(ArithmeticExpression, calculate_drobnoye_division) {
    ArithmeticExpression expr("3.0/2");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 1.5);
}

TEST(ArithmeticExpression, calculate_complex_drobnoye_expression) {
    ArithmeticExpression expr("(1.2+3.8)*2.5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 12.5);
}

TEST(ArithmeticExpression, calculate_with_zero) {
    ArithmeticExpression expr("0+5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 5.0);
}

TEST(ArithmeticExpression, calculate_zero_multiplication) {
    ArithmeticExpression expr("0*5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 0.0);
}

TEST(ArithmeticExpression, calculate_zero_division) {
    ArithmeticExpression expr("0/5");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 0.0);
}

TEST(ArithmeticExpression, integration_complex_expression_1) {
    ArithmeticExpression expr("3+4*2/(1-5)*2");
    expr.run_analyzer();
    double result = expr.calculate();
    EXPECT_DOUBLE_EQ(result, -1.0);
}

TEST(ArithmeticExpression, integration_complex_expression_2) {
    ArithmeticExpression expr("10.5+2.5*(3-1.5)/2");
    expr.run_analyzer();
    double result = expr.calculate();
    EXPECT_DOUBLE_EQ(result, 12.375);
}

TEST(ArithmeticExpression, single_number_expression) {
    ArithmeticExpression expr("42");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 42.0);
}

TEST(ArithmeticExpression, single_zero_expression) {
    ArithmeticExpression expr("0");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 0.0);
}

TEST(ArithmeticExpression, expression_with_spaces) {
    ArithmeticExpression expr(" 2 + 3 * 4 ");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 14.0);
}

TEST(ArithmeticExpression, calculate_operand_order_subtraction) {
    ArithmeticExpression expr("5-3");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 2.0);
}

TEST(ArithmeticExpression, calculate_operand_order_division) {
    ArithmeticExpression expr("6/2");
    expr.run_analyzer();
    EXPECT_DOUBLE_EQ(expr.calculate(), 3.0);
}