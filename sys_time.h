#ifndef SYS_TIME_H_
#define SYS_TIME_H_

#include <ctime>
#include <memory>

class sys_time_ifc {
    public:
        virtual time_t time(time_t *timer) const {
            return 0;
        }
};

class sys_time {
    public:
        using delegate = std::shared_ptr<sys_time_ifc>;
    private:
        delegate delegate_;
    public:
        sys_time(const delegate &delegate) : delegate_(delegate) {}
        time_t time(time_t *timer) const {
            return delegate_->time(timer);
        }
   static const sys_time &get();

};

#endif

