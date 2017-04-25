#ifndef SYS_FSTREAM_H
#define SYS_FSTREAM_H

#include <memory>
#include <sstream>

class fstreams_ifc {
    public:
        typedef std::shared_ptr<std::istream> pstream;
        virtual pstream open_fstream(const std::string &file_path) {
            return pstream(new std::istringstream(""));
        }
};

class fstreams : public fstreams_ifc {
    public:
        typedef std::shared_ptr<fstreams_ifc> delegate;
    private:
        delegate delegate_;
    public:
        fstreams(const delegate &delegate) : delegate_(delegate){}
        fstreams() : fstreams(delegate(new fstreams_ifc)) {}
        pstream open_fstream(const std::string &file_path) {
            return delegate_->open_fstream(file_path);
        }
        static fstreams create();
};


#endif

