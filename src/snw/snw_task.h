#ifndef SNW_TASK_H
#define SNW_TASK_H

namespace Snowda {

    enum class TaskState : uint8_t {
        Idle,
        Running,
        Waiting,
        Finished,
    };

    enum class TaskPriority : uint8_t {
        Low,
        Normal,
        High,
    };

    class Task {
    public:
        Task();
        virtual ~Task();

        TaskPriority priority() const;
        void setPriority(TaskPriority priority);

        TaskState state() const;
        TaskState schedule();

    protected:
        virtual TaskState run() = 0;

    private:
        TaskState state_;
        TaskPriority priority_;
    };

}

#endif // SNW_TASK_H
