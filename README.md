[![Build Status](https://travis-ci.org/cjdev/dual-control.svg?branch=master)](https://travis-ci.org/cjdev/dual-control)

# Dual Control
Dual Control is a PAM module that requires a user to input a generated token from another user before being granted sudo access. The module also requires that the user input the reason for his or her access request and, via `syslog`, captures all this information for future reference.

At **CJ Engineering**, we will be implementing Dual Control on our production boxes to ensure that a single engineer cannot gain write access without meeting the above-stated requirements.

Dual Control is an open source project licensed under the [GNU General Public License](https://github.com/cjdev/dual-control/blob/master/COPYING). As it stands, Dual Control is written only for machines running Linux. However, we graciously welcome contributions, particularly those related to portability to other operating systems.

## For ZFR
* setup a native project
  * for Linux (docker, virtualbox)
* installer
* CI + CD
* Logging
