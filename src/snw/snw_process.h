#ifndef SNW_PROCESS_H
#define SNW_PROCESS_H

namespace Snowda {
    struct ProcessParam {
        // Thread &thread
        // Runtime &runtime
        // Program &program
        // StackFrame frame
        // ProcessId pid
    };

    class Process : public Task {
    public:
        Process(const ProcessParam &param);

    private:
        virtual TaskState run() override;

    private:
        // uint32_t typeId_;
        // uint16_t method_;
        // uint16_t instructionPointer_;

        // Heap heap
        // Stack stack
    };
}

#endif // SNW_PROCESS_H
