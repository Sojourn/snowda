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

    virtual void visit(const NumberExpression &node)
    {
        pad();
        std::cout << "NumberExpression value:" << node.value() << std::endl;
    }

	virtual void visit(const CharacterExpression &node)
	{
		pad();
		std::cout << "CharacterExpression value:" << node.value() << std::endl;
	}

	virtual void visit(const StringExpression &node)
	{
		pad();
		std::cout << "StringExpression value:" << node.value() << std::endl;
	}

    virtual void visit(const UnaryExpression &node)
    {
        pad();
        std::cout << "UnaryExpression op:" << str(node.op()) << std::endl;
        depth_ += 1;
        node.expr()->accept(*this);
        depth_ -= 1;
    }

    virtual void visit(const BinaryExpression &node)
    {
        pad();
        std::cout << "BinaryExpression op:" << str(node.op()) << std::endl;
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

void testParser()
{
    Lexer lexer("a / (b + c)");
    Parser parser(lexer);

	// Fixme: The parse tree is screwy
    ParserResult result = parser.parseExpression(0);
    if (result.hasValue()) {
        std::cout << "Parse succceeded" << std::endl;
        Printer printer;
        result.value()->accept(printer);
    }
    else {
        ParserError error = result.error();
        std::cout << "Parse failed: " << error.msg() << std::endl;
        std::cout << "  row: " << error.row() << std::endl;
        std::cout << "  col: " << error.col() << std::endl;
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
