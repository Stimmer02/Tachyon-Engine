#ifndef IRUNNABLE_H
#define IRUNNABLE_H

class IRunnable {
public:

    /// @brief Method called at the beginning of runnable thread life
    virtual void Run() = 0;

    /// @brief Method sends signal to stop runnable thread life
    virtual void Stop() = 0;
};

#endif
