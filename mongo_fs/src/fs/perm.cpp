#include "perm.h"

using namespace fs;

Mode::Mode(mode_t mode) {
    user_perm =
    Perm{(bool)(mode & S_IRUSR), (bool)(mode & S_IWUSR), bool(mode & S_IXUSR)};
    group_perm =
    Perm{(bool)(mode & S_IRUSR), (bool)(mode & S_IWUSR), bool(mode & S_IXUSR)};
    univ_perm =
    Perm{(bool)(mode & S_IRUSR), (bool)(mode & S_IWUSR), bool(mode & S_IXUSR)};
}