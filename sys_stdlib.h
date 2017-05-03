#ifndef SYS_CSTDLIB_H_
#define SYS_CSTDLIB_H_

#include <memory>

class stdlib_ifc {
    public:
        virtual int rand() const {return 0;}
};

class stdlib {
    public:
        using delegate = std::shared_ptr<stdlib_ifc>;
    private:
       delegate delegate_;
    public:
       stdlib(delegate delegate = std::make_shared<stdlib_ifc>()) : delegate_(delegate) {}
      int rand() const { return delegate_->rand(); }
      static const stdlib& get();
};


#endif
