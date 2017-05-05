[![Build Status](https://travis-ci.org/cjdev/dual-control.svg?branch=master)](https://travis-ci.org/cjdev/dual-control)

# Dual Control
Dual Control is a PAM module that requires a user to input a generated token from another user before being granted resource access. The module also requires that the user input the reason for his or her access request and, via `syslog`, captures all this information for future reference.

At **CJ Engineering**, we will be implementing Dual Control on our production boxes to ensure that a single engineer cannot use sudo to gain application-role access without meeting the above-stated requirements.

Dual Control is an open source project licensed under the [GNU General Public License](https://github.com/cjdev/dual-control/blob/master/LICENSE). As it stands, Dual Control is written only for machines running Linux. However, we graciously welcome contributions, particularly those related to portability to other operating systems.

## Status
This is in active development. The current version uses a permanent token and so is not meant for production use. The final version will
use a time-based OTP.

## Install
- Obtain and install the RPM
  - build yourself using the code in /cjdev/dual-control-rpm
  - get it from a developer
- Edit the `/etc/pam.d/sudo`  (this is for CentOS 7, others may be different)
  - replace the existing auth lines with
```
#%PAM-1.0
# auth       include      system-auth
auth        required      pam_env.so
auth        required      pam_unix.so
auth        sufficient    pam_dual_control.so
auth        requisite     pam_succeed_if.so uid >= 1000 quiet_success
auth        required      pam_deny.so
account    include      system-auth
password   include      system-auth
session    optional     pam_keyinit.so revoke
session    required     pam_limits.so
```

## Add a dual control token
- in the authorizer's account home, put the token characters in `.dual_control`

## Use
- log in with a test user that has `sudo` ability (not the vagrant account it is too
powerful)
- type `sudo bash`
- enter your password
- enter dual control token, authorizer's username + ':' + authorizer's token

