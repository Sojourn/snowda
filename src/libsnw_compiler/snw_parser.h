#ifndef SNW_PARSER_H
#define SNW_PARSER_H

namespace Snowda {

    class ParserFrame {
    public:
        ParserFrame(Parser &parser);
        ~ParserFrame();

        Ast::NodeContent nodeContent();

    private:
        Parser &parser_;
        ParserFrame *prev_;
        const size_t pos_;
        const size_t row_;
        const size_t col_;
    };

    class Parser {
        friend class ParserFrame;
    public:
        explicit Parser(Lexer &lexer);
        ~Parser();

        bool finished();
        ExprResult parseExpression(int bp);
        StmtResult parseStatement();
        RootResult parseRootStatement();

        size_t row();
        size_t col();

        Token currentToken();
		Token nextToken();
        Token consumeToken();
        bool advanceToken(TokenType type);

        const Grammar &grammar() const;

        template<typename T, typename... Args>
        const T *create(Args&&... args)
        {
            const T *node = new T(frame_->nodeContent(), std::forward<Args>(args)...);
            nodes_.push_back(node);
            return node;
        }

    private:
        TokenStream stream_;
        const Grammar grammar_;
        size_t depth_;
		Ast::NodeVec nodes_;
        ParserFrame *frame_;
    };

}

#endif // SNW_PARSER_H
