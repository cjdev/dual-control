#include "become.h"
#include "sys_unistd.h"
#include "sys_pwd.h"
#include "user.h"
#include "test_util.h"

template<class T>
std::shared_ptr<T> share (T *t)
{
    return std::shared_ptr<T> (t);
}

class mock_unistd : public unistd_ifc
{
private:
    uid_t expected_euid_;
public:
    mutable uid_t actual_uid;
    mock_unistd (uid_t expected_euid)
        : expected_euid_ (expected_euid)
    {}

    int seteuid (uid_t euid) const override
    {
        actual_uid = euid;
        return euid == expected_euid_ ? 0 : -1;
    }
};

class fake_user : public user_ifc
{
private:
    uid_t uid_;
public:
    fake_user (uid_t uid) : uid_ (uid)
    {}
    uid_t uid() const override
    {
        return uid_;
    }
};

int become_calls_seteuid_with_right_arguments ()
{
    uid_t expected_uid = 1000;
    std::shared_ptr<mock_unistd> mock_unistd  = share (new class mock_unistd(expected_uid));
    unistd fake_unistd (mock_unistd);
    user fake_user (share (new class fake_user(expected_uid)));

    become become_ (fake_user, fake_unistd);

    check (mock_unistd->actual_uid == expected_uid, "become does not seteuid");

    succeed();
}

bool run_tests()
{
    test (become_calls_seteuid_with_right_arguments);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
