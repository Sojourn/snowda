#ifndef SNW_VIRTUAL_MACHINE_H
#define SNW_VIRTUAL_MACHINE_H

namespace Snowda {
    class VirtualMachine {
    public:
        VirtualMachine(const Program &program);

        void run(View<const char *> args);

    private:
        const Program &program_;
        View<const char *> args_;
    };
}

#endif // SNW_VIRTUAL_MACHINE_H
