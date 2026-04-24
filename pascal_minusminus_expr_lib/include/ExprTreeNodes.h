#pragma once
#include "Pascal_MinusMinus_Expression.h"

class PMM_EXPR::Node {
public:
    virtual NodeRetVal accept(Visitor* v) = 0;
    virtual inline int getCode() const = 0;
};

class PMM_EXPR::BiNode : public Node {
protected:
    Node* left = nullptr;
    Node* right = nullptr;
public:
    BiNode(Node* l, Node* r = nullptr);
    Node* getLeft() const;
    Node* getRight() const;
    virtual NodeRetVal accept(Visitor* v) = 0;
};

class PMM_EXPR::Terminal : public Node {
protected:
    lexem il;
public:
    Terminal() : il(lexem()) {}
    Terminal(lexem _l) : il(_l) {}
    virtual NodeRetVal accept(Visitor* v) = 0;
    virtual inline lexem getIL() const { return il; }
};


class PMM_EXPR::Variable : public Terminal {
public:
    Variable(lexem _n) : Terminal(_n) {}
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 2; }
    string inline getName() const { return il.value; }
};
class PMM_EXPR::FPNumber : public Terminal {
public:
    FPNumber(lexem _val) : Terminal(_val) {}
    double inline getVal() const { return std::stod(il.value); }
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 3; }
};
class PMM_EXPR::LRBr : public Terminal {
public:
    LRBr() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 5; }
};
class PMM_EXPR::RRBr : public Terminal {
public:
    RRBr() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 7; }
};
class PMM_EXPR::LCBr : public Terminal {
public:
    LCBr() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 11; }
};
class PMM_EXPR::RCBr : public Terminal {
public:
    RCBr() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 13; }
};
class PMM_EXPR::AdSb : public Terminal {
public:
    AdSb(lexem _tp) : Terminal(_tp) {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 17; }
    inline LexemType getType() const { return il.type; }
};
class PMM_EXPR::MuDv : public Terminal {
public:
    MuDv(lexem _tp) : Terminal(_tp) {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 19; }
    inline LexemType getType() const { return il.type; }
};
class PMM_EXPR::Cop : public Terminal {
public:
    Cop(lexem _tp) : Terminal(_tp) {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 23; }
    inline LexemType getType() const { return il.type; }
};
class PMM_EXPR::Eq : public Terminal {
public:
    Eq() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 29; }
};
class PMM_EXPR::SmCln : public Terminal {
public:
    SmCln() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 31; }
};
class PMM_EXPR::WhileOp : public Terminal {
public:
    WhileOp() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 37; }
};
class PMM_EXPR::IfOp : public Terminal {
public:
    IfOp() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 43; }
};
class PMM_EXPR::ElseOp : public Terminal {
public:
    ElseOp() : Terminal() {}
    virtual NodeRetVal accept(Visitor* v) override { return 0.0; }
    virtual inline int getCode() const override { return 47; }
};

class PMM_EXPR::Mon : public BiNode {
    LexemType op;
public:
    Mon(Terminal* l);
    Mon(Mon* l, LexemType _op, Terminal* r);
    LexemType getOp() const;
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 53; }
};

class PMM_EXPR::Pol : public BiNode {
    LexemType op;
public:
    Pol(Mon* l);
    Pol(Pol* l, LexemType _op, Mon* r);
    Pol(Pol* l, LexemType _op, Pol* r);
    LexemType getOp() const;
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 59; }
};

class PMM_EXPR::Comp : public BiNode {
    LexemType op;
public:
    Comp(Pol* l, LexemType _op, Pol* r);
    LexemType getOp() const;
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 61; }
};

class PMM_EXPR::EqOper : public BiNode {
public:
    EqOper(Variable* l, Pol* r);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 67; }
};

class PMM_EXPR::Expr : public BiNode {
public:
    Expr(Operator* l);
    Expr(Expr* l, Operator* r);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 71; }
};

class PMM_EXPR::IfOper : public BiNode {
public:
    IfOper(Comp* l, Expr* r);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 73; }
};

class PMM_EXPR::IfElse : public BiNode {
public:
    IfElse(IfOper* l, Expr* r = nullptr);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 79; }
};

class PMM_EXPR::WhileOper : public BiNode {
public:
    WhileOper(Comp* l, Expr* r);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 89; }
};

class PMM_EXPR::Operator : public Node {
    Node* child;
public:
    Operator(EqOper* c);
    Operator(WhileOper* c);
    Operator(IfElse* c);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 83; }
    inline Node* getChild() const { return child; }
};

class PMM_EXPR::ExprTree : public Node {
    Node* child;
public:
    ExprTree(Expr* c);
    virtual NodeRetVal accept(Visitor* v) override;
    virtual inline int getCode() const override { return 97; }
    inline Node* getChild() const { return child; }
};