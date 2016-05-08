#include "snw.h"

using namespace Snowda;
using namespace Snowda::Ast;

void testLexer()
{
    {
        Lexer lexer("(a bb ccc)\n"
                    "(\"stuff\")\n"
                    "var a.type = bb;");

        for (;;) {
            Token token = lexer.next();
            std::cout << token << std::endl;
            if (token.type == TokenType::Finished || token.type == TokenType::Error) {
                break;
            }
        }
    }
}

void testTokenStream()
{
    Lexer lexer("(a b c);");
    TokenStream stream(lexer);

    size_t index = 0;
    auto checkNext = [&](TokenType expectedType) {
        Token token = stream[index++];
        assert(token.type == expectedType);
    };

    auto checkpoint = [&]() {
        stream.advance(index);
        index = 0;
    };

    checkpoint();
    checkNext(TokenType::LParen);
    checkNext(TokenType::Identifier);
    checkpoint();
    checkNext(TokenType::Identifier);
    checkNext(TokenType::Identifier);
    checkNext(TokenType::RParen);
    checkpoint();
    checkNext(TokenType::Semi);
    checkNext(TokenType::Finished);
    checkpoint();
    checkNext(TokenType::Finished);
    checkpoint();
    checkpoint();
}

struct Printer : public Visitor {
public:
    Printer()
        : depth_(0)
    {
    }

    virtual void visit(const LiteralExpression &node)
    {
        pad();
        std::cout << "LiteralExpression value:" << node.value() << std::endl;
    }

    virtual void visit(const UnaryExpression &node)
    {
        pad();
        std::cout << "UnaryExpression op:" << (char)node.op() << std::endl;
        depth_ += 1;
        node.expr()->accept(*this);
        depth_ -= 1;
    }

    virtual void visit(const BinaryExpression &node)
    {
        pad();
        std::cout << "BinaryExpression op:" << (char)node.op() << std::endl;
        depth_ += 1;
        node.lhs()->accept(*this);
        node.rhs()->accept(*this);
        depth_ -= 1;
    }

    virtual void visit(const IdentifierExpression &node)
    {
        pad();
        std::cout << "IdentifierExpression name:" << node.name() << std::endl;
    }

    virtual void visit(const ConditionalExpression &node)
    {
        pad();
        std::cout << "ConditionalExpression" << std::endl;
        depth_ += 1;
        node.condExpr()->accept(*this);
        node.thenExpr()->accept(*this);
        if (node.elseExpr()) {
            node.elseExpr()->accept(*this);
        }
        depth_ -= 1;
    }

private:
    void pad() const
    {
        for (size_t i = 0; i < depth_; ++i) {
            std::cout << "  ";
        }
    }

    size_t depth_;
};

template<int bp, UnaryOperator op>
void prefix(Parser &parser, TokenType type) {
    parser.add(type, [](Parser &parser) {
        ParserResult result = parser.parseExpression(bp);
        if (result.hasError()) {
            return result;
        }
        else {
            return std::make_unique<UnaryExpression>(op, std::move(result.value()));
        }
    });
}

template<int bp, BinaryOperator op>
void infix(Parser &parser, TokenType type) {
    parser.add(type, bp, [](Parser &parser, ExpressionPtr left) -> ParserResult {
        ParserResult result = parser.parseExpression(bp);
        if (result.hasError()) {
            return result;
        }
        else {
            return std::make_unique<BinaryExpression>(op, std::move(left), std::move(result.value()));
        }
    });
}

void testParser()
{
    NullDelimitedFunc identifier = [](Parser &parser, Token token) -> ParserResult {
        return std::make_unique<IdentifierExpression>(token.content);
    };

	LeftDelimitedFunc addition = [](Parser &parser, ExpressionPtr left, Token token) -> ParserResult {
		ParserResult result = parser.parseExpression(BindingPower::Sum);
		if (result.hasError()) {
			return result;
		}
		else {
			return std::make_unique<BinaryExpression>(BinaryOperator::Add, std::move(left), std::move(result.value()));
		}
	};

    NullDelimitedFunc plus = [](Parser &parser, Token token) -> ParserResult {
        ParserResult result = parser.parseExpression(BindingPower::Unary);
        if (result.hasError()) {
            return result;
        }
        else {
            return std::make_unique<UnaryExpression>(UnaryOperator::Plus, std::move(result.value()));
        }
    };

    Lexer lexer("a ++ a");

    Parser parser(lexer);
    parser.add(TokenType::Identifier, identifier);
    // infix<BindingPower::Unary, UnaryOperator::Plus>(parser, TokenType::Plus);
    // infix<BindingPower::Sum, BinaryOperator::Add>(parser, TokenType::Plus);
    parser.add(TokenType::Plus, plus);
	parser.add(TokenType::Plus, BindingPower::Sum, addition);

    ParserResult result = parser.parseExpression(0);
    if (result.hasValue()) {
        std::cout << "Parse succceeded" << std::endl;
		Printer printer;
		result.value()->accept(printer);
    }
    else {
        ParserError error = result.error();
        std::cout << "Parse failed: " << error.message << std::endl;
        std::cout << "  row: " << error.row << std::endl;
        std::cout << "  col: " << error.col << std::endl;
    }
}

int main(int argc, char **argv)
{
    testLexer();
    testTokenStream();
    testParser();

    std::system("pause");

    return 0;
}
