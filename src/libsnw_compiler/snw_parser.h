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
        explicit Parser(MemoryManager &memoryManager, Lexer &lexer);

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
            ArenaAllocator &arena = memoryManager_.arenaAllocator();
            uint8_t *addr = arena.allocate(sizeof(T));
            return new(addr) T(frame_->nodeContent(), std::forward<Args>(args)...);
        }

    private:
        MemoryManager &memoryManager_;
        ArenaFrame arenaFrame_;
        TokenStream stream_;
        const Grammar grammar_;
        size_t depth_;
        ParserFrame *frame_;
    };

}

#endif // SNW_PARSER_H
