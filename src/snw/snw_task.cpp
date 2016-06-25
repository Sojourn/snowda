#include "snw.h"

using namespace Snowda;

Task::Task()
    : state_(TaskState::Idle)
{
}

Task::~Task()
{
}

TaskPriority Task::priority() const
{
    return priority_;
}

void Task::setPriority(TaskPriority priority)
{
    priority_ = priority;
}

TaskState Task::state() const
{
    return state_;
}

TaskState Task::schedule()
{
    assert((state_ == TaskState::Idle) || (state_ == TaskState::Waiting));

    switch (run()) {
    case TaskState::Idle:
        state_ = TaskState::Idle;
        break;
    }

    return state_;
}
