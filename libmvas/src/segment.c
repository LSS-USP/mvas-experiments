#include <segment.h>
#include <vas.h>

#include <linux/vas.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

segid_t segment_create(const char * const name, unsigned long start,
                       unsigned long end, mode_t mode)
{
    segid_t id;

    if (!name) {
        errno = EINVAL;
        return -1;
    }

    id = syscall(SYS_segment_create, name, start, end, mode);
    if (id < 0) {
        errno = -id;
        return -1;
    } else {
        return id;
    }
}

segid_t segment_create_sz(const char * const name, unsigned long start,
                          unsigned long size, mode_t mode)
{
    return segment_create(name, start, start + size, mode);
}

int segment_delete(segid_t sid)
{
    long ret;

    if (sid <= 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_segment_delete, sid);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

segid_t segment_find(const char * const name)
{
    segid_t id;

    if (!name) {
        errno = EINVAL;
        return -1;
    }

    id = syscall(SYS_segment_find, name);
    if (id < 0) {
        errno = -id;
        return -1;
    } else {
        return id;
    }
}

int segment_attach(vasid_t vid, segid_t sid, int type)
{
    long ret;

    if (vid <= 0 || sid <= 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_segment_attach, vid, sid, type);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int segment_detach(vasid_t vid, segid_t sid)
{
    long ret;

    if (vid <= 0 || sid <= 0) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_segment_detach, vid, sid);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int segment_getattr(segid_t sid, struct segment_attr * const attr)
{
    long ret;

    if (sid <= 0 || !attr) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_segment_getattr, sid, attr);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}

int segment_setattr(segid_t sid, const struct segment_attr * const attr)
{
    long ret;

    if (sid <= 0 || !attr) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(SYS_segment_setattr, sid, attr);
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return 0;
    }
}
