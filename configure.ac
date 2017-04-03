#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Implementation of the --with-pam-module-directory switch.
DEFAULT_PAM_MODULE_DIRECTORY="${libdir}/security"
AC_ARG_WITH(pam-module-directory,
    [  --with-pam-module-directory=NAME
        Use NAME as installation directory for PAM modules ],
    PAM_MODULE_DIRECTORY="$withval",
    PAM_MODULE_DIRECTORY="$DEFAULT_PAM_MODULE_DIRECTORY")
AC_SUBST(PAM_MODULE_DIRECTORY)


AC_CONFIG_FILES([Makefile])
AC_OUTPUT

if test "$PAM_MODULE_DIRECTORY" = "$DEFAULT_PAM_MODULE_DIRECTORY"; then
    AC_MSG_WARN([
        **
        ** Warning: using default value ($DEFAULT_PAM_MODULE_DIRECTORY) as
        **          installation directory for PAM module; this is probably not
        **          what you want. Either use the configure flag
        **
        **                      --with-pam-module-directory=DIRECTORY
        **
        **          or copy the newly built pam_dual_control.so into the correct directory
        **          manually. Usually PAM modules are located in /lib/security or /lib64/security.
        **])
else
    AC_MSG_NOTICE([PAM module installation directory: $PAM_MODULE_DIRECTORY])
fi


