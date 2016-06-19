#ifndef SNW_PROGRAM_H
#define SNW_PROGRAM_H

namespace Snowda {

    class Program {
    public:
        Program(MemoryManager &manager);
        Program(const Program &) = delete;
        Program(Program &&) = delete;

        Program &operator=(const Program &) = delete;
        Program &operator=(Program &&) = delete;

    private:
        ArenaAllocator arena_;
        View<Type> types_;
    };
}

#endif // SNW_PROGRAM_H
