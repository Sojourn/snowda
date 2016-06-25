#ifndef SNW_DRIVER_H
#define SNW_DRIVER_H

#define SNW_DRIVER_BEGIN() switch(continuation_) { case 0:
#define SNW_DRIVER_WAIT(cond) \
    continuation_ = __LINE__; case __LINE__: \
    if (!(cond)) { \
        return TaskState::Waiting \
    }
#define SNW_DRIVER_AWAIT(future) abort() /* TODO */
#define SNW_DRIVER_YIELD() \
    continuation_ = __LINE__; case __LINE__: \
    return TaskState::Idle
#define SNW_DRIVER_END() } return TaskState::Finished

namespace Snowda {
    class Driver : public Task {
    public:
        Driver();

    protected:
        uint32_t continuation_;
    };
}

#endif // SNW_DRIVER_H
