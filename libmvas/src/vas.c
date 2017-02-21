#include <vas.h>

#include <linux/vas.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>


vasid_t vas_create(const char * const name, mode_t mode)
{
    vasid_t id;

    if (!name) {
        errno = EINVAL;
        return -1;
    }

    id = syscall(SYS_vas_create, name, mode);
    if (id < 0) {
        errno = -id;
        return -1;
    } else {
        return id;
    }
}

int vas_delete(vasid_t vid)
{
    long ret;

    if (vid <= 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_vas_delete, vid);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

vasid_t vas_find(const char * const name)
{
    vasid_t id;

    if (!name) {
        errno = EINVAL;
        return -1;
    }

    id = syscall(SYS_vas_find, name);
    if (id < 0) {
        errno = -id;
        return -1;
    } else {
        return id;
    }
}

int vas_attach(pid_t pid, vasid_t vid, int type)
{
    long ret;

    if (pid < 0 || vid <= 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_vas_attach, pid, vid, type);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int vas_detach(pid_t pid, vasid_t vid)
{
    long ret;

    if (pid < 0 || vid <= 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_vas_detach, pid, vid);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int vas_switch(vasid_t vid)
{
    long ret;

    if (vid < 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_vas_switch, vid);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int vas_getattr(vasid_t vid, struct vas_attr * const attr)
{
    long ret;

    if (vid <= 0 || !attr) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_vas_getattr, vid, attr);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int vas_setattr(vasid_t vid, const struct vas_attr * const attr)
{
    long ret;

    if (vid <= 0 || !attr) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_vas_setattr, vid, attr);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}
