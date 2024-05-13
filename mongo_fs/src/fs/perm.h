#ifndef __PERM_H__
#define __PERM_H__

#include <sys/stat.h>

namespace fs {

enum class PermBits {
    READ,
    WRITE,
    EXEC,
    NONE,
};

struct Perm {
    Perm(bool read, bool write, bool exec) : read{read}, write{write}, exec{exec} {
    }
    Perm() : read{false}, write{false}, exec{false} {
    }

    bool read;
    bool write;
    bool exec;
};

class Mode {
    public:
    Mode(Perm user_perm, Perm group_perm, Perm univ_perm)
    : user_perm{user_perm}, group_perm{group_perm}, univ_perm{univ_perm} {
    }
    Mode(mode_t mode);

    Mode() {
        user_perm  = Perm{false, false, false};
        group_perm = Perm{false, false, false};
        univ_perm  = Perm{false, false, false};
    }

    Perm user_perm;
    Perm group_perm;
    Perm univ_perm;
};

} // namespace fs

#endif
