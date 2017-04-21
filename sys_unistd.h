#ifndef _SYS_UNISTD_H_
#define _SYS_UNISTD_H_

#include <memory>

class unistd_ifc {
    public:
        virtual long int sysconf(int name) { return -1; }
};

class unistd : public unistd_ifc {
    public:
        typedef std::shared_ptr<unistd_ifc> delegate;

    private:
        delegate delegate_;

    public:
        unistd(delegate delegate): delegate_(delegate) {}
        unistd() : delegate_(delegate(new unistd_ifc)) {}
        long int sysconf(int name) {
            return delegate_->sysconf(name);
        }
        static unistd system();
};



#endif
