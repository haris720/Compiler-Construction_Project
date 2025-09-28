#include "AST.h"
#include <iostream>
#include <iomanip>

static void pad(int n){ for(int i=0;i<n;++i) std::cout << ' '; }

static const char* tokName(TokenType t) {
    switch(t){
        case TokenType::INT: return "int";
        case TokenType::FLOAT: return "float";
        case TokenType::STRING: return "string";
        default: return "unknown";
    }
}

void printAST(const StmtPtr& n, int indent){
    if(!n){ pad(indent); std::cout << "(null)\n"; return; }

    switch(n->kind){
        case NodeKind::Program: {
            auto* p = static_cast<Program*>(n.get());
            pad(indent); std::cout << "Program\n";
            for (auto& it : p->items) printAST(it, indent+2);
            break;
        }
        case NodeKind::FnDecl: {
            auto* f = static_cast<FnDeclStmt*>(n.get());
            pad(indent); std::cout << "FnDecl name=" << f->name;
            if (f->returnType != TokenType::ERROR) std::cout << " return=" << tokName(f->returnType);
            std::cout << "\n";
            pad(indent+2); std::cout << "Params:\n";
            for (auto& pr : f->params){
                pad(indent+4); std::cout << tokName(pr.typeTok) << " " << pr.name << "\n";
            }
            pad(indent+2); std::cout << "Body:\n";
            printAST(f->body, indent+4);
            break;
        }
        case NodeKind::Block: {
            auto* b = static_cast<BlockStmt*>(n.get());
            pad(indent); std::cout << "Block\n";
            for (auto& s : b->statements) printAST(s, indent+2);
            break;
        }
        case NodeKind::VarDecl: {
            auto* v = static_cast<VarDeclStmt*>(n.get());
            pad(indent); std::cout << "VarDecl " << tokName(v->typeTok) << " " << v->name << " =\n";
            printAST(std::make_shared<ExprStmt>(v->init), indent+2);
            break;
        }
        case NodeKind::ReturnStmt: {
            auto* r = static_cast<ReturnStmt*>(n.get());
            pad(indent); std::cout << "Return\n";
            printAST(std::make_shared<ExprStmt>(r->expr), indent+2);
            break;
        }
        case NodeKind::ExprStmt: {
            auto* e = static_cast<ExprStmt*>(n.get());
            // inline dispatch for expressions
            auto& x = e->expr;
            switch(x->kind){
                case NodeKind::Identifier: {
                    auto* i = static_cast<IdentExpr*>(x.get());
                    pad(indent); std::cout << "Ident \"" << i->name << "\"\n"; break;
                }
                case NodeKind::IntLit: {
                    auto* i = static_cast<IntLitExpr*>(x.get());
                    pad(indent); std::cout << "Int " << i->value << "\n"; break;
                }
                case NodeKind::FloatLit: {
                    auto* i = static_cast<FloatLitExpr*>(x.get());
                    pad(indent); std::cout << "Float " << i->value << "\n"; break;
                }
                case NodeKind::StringLit: {
                    auto* s = static_cast<StringLitExpr*>(x.get());
                    pad(indent); std::cout << "String \"" << s->value << "\"\n"; break;
                }
                case NodeKind::Unary: {
                    auto* u = static_cast<UnaryExpr*>(x.get());
                    pad(indent); std::cout << "Unary(" << (int)u->op << ")\n";
                    printAST(std::make_shared<ExprStmt>(u->expr), indent+2);
                    break;
                }
                case NodeKind::Binary: {
                    auto* b = static_cast<BinaryExpr*>(x.get());
                    pad(indent); std::cout << "Binary(" << (int)b->op << ")\n";
                    printAST(std::make_shared<ExprStmt>(b->left), indent+2);
                    printAST(std::make_shared<ExprStmt>(b->right), indent+2);
                    break;
                }
                case NodeKind::Call: {
                    auto* c = static_cast<CallExpr*>(x.get());
                    pad(indent); std::cout << "Call \"" << c->callee << "\"\n";
                    pad(indent+2); std::cout << "Args:\n";
                    for (auto& a : c->args)
                        printAST(std::make_shared<ExprStmt>(a), indent+4);
                    break;
                }
                default:
                    pad(indent); std::cout << "(unknown expr kind)\n"; break;
            }
            break;
        }
        default:
            pad(indent); std::cout << "(unknown stmt kind)\n"; break;
    }
}

void printAST(const std::vector<StmtPtr>& nodes, int indent){
    for (auto& n : nodes) printAST(n, indent);
}
