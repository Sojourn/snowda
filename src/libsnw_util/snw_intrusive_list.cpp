#include "snw_util.h"

using namespace Snowda;

IntrusiveListHook::IntrusiveListHook()
    : next_(nullptr)
    , prev_(nullptr)
{
}

IntrusiveListHook::~IntrusiveListHook() {
    if (isLinked()) {
        unlink();
    }
}

bool IntrusiveListHook::isLinked() const {
    return next_ != nullptr;
}

void IntrusiveListHook::unlink() {
    assert(isLinked());

    next_->prev_ = prev_;
    prev_->next_ = next_;

    next_ = nullptr;
    prev_ = nullptr;
}

