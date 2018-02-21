#ifndef UTILS_H
#define UTILS_H

#include <QThread>

bool testBit(uint64_t value, int bit);

class StringUtils {
public:
    static QString intToHex(long long value, int len)
    {
        QByteArray ba;
        for (int i = 0; i < len; i++) {
            ba.insert(0, (value >> i * 8) & 0xFF);
        }
        return ba.toHex().toUpper();
    }

    static QString intToStr(uint64_t value)
    {
        QString text;
        return text.sprintf("%llu", value);
    }

    static QString dataToHex(QByteArray ba)
    {
        QString result;
        for (int i = 0; i < ba.length(); i++) {
            if (i != 0) {
                result.append(" ");
            }
            QByteArray b;
            b.append(ba.at(i));
            result.append(b.toHex());
        }
        return result.toUpper();
    }
};

class QSleepThread : private QThread {
public:
    static void msleep(uint32_t msec)
    {
        QThread::msleep(msec);
    }
};

class ErrorText
{
public:
    static QString getErrorText(int code){
        switch (code)
        {
            case -1              : return "Socket closed";
            case EPERM           : return "EPERM, Operation not permitted";
            case ENOENT          : return "ENOENT, No such file or directory";
            case ESRCH           : return "ESRCH, No such process";
            case EINTR           : return "EINTR, Interrupted system call";
            case EIO             : return "EIO, I/O error";
            case ENXIO           : return "ENXIO, No such device or address";
            case E2BIG           : return "E2BIG, Argument list too long";
            case ENOEXEC         : return "ENOEXEC, Exec format error";
            case EBADF           : return "EBADF, Bad file number";
            case ECHILD          : return "ECHILD, No child processes";
            case EAGAIN          : return "EAGAIN, Try again";
            case ENOMEM          : return "ENOMEM, Out of memory";
            case EACCES          : return "EACCES, Permission denied";
            case EFAULT          : return "EFAULT, Bad address";
            case ENOTBLK         : return "ENOTBLK, Block device required";
            case EBUSY           : return "EBUSY, Device or resource busy";
            case EEXIST          : return "EEXIST, File exists";
            case EXDEV           : return "EXDEV, Cross-device link";
            case ENODEV          : return "ENODEV, No such device";
            case ENOTDIR         : return "ENOTDIR, Not a directory";
            case EISDIR          : return "EISDIR, Is a directory";
            case EINVAL          : return "EINVAL, Invalid argument";
            case ENFILE          : return "ENFILE, File table overflow";
            case EMFILE          : return "EMFILE, Too many open files";
            case ENOTTY          : return "ENOTTY, Not a typewriter";
            case ETXTBSY         : return "ETXTBSY, Text file busy";
            case EFBIG           : return "EFBIG, File too large";
            case ENOSPC          : return "ENOSPC, No space left on device";
            case ESPIPE          : return "ESPIPE, Illegal seek";
            case EROFS           : return "EROFS, Read-only file system";
            case EMLINK          : return "EMLINK, Too many links";
            case EPIPE           : return "EPIPE, Broken pipe";
            case EDOM            : return "EDOM, Math argument out of domain of func";
            case ERANGE          : return "ERANGE, Math result not representable";
            case EDEADLK         : return "Resource deadlock would occur";
            case ENAMETOOLONG    : return "File name too long";
            case ENOLCK          : return "No record locks available";
            case ENOSYS          : return "Function not implemented";
            case ENOTEMPTY       : return "Directory not empty";
            case ELOOP           : return "Too many symbolic links encountered";
            //case EWOULDBLOCK     : return "Operation would block";
            case ENOMSG          : return "No message of desired type";
            case EIDRM           : return "Identifier removed";
            case ECHRNG          : return "Channel number out of range";
            case EL2NSYNC        : return "Level 2 not synchronized";
            case EL3HLT          : return "Level 3 halted";
            case EL3RST          : return "Level 3 reset";
            case ELNRNG          : return "Link number out of range";
            case EUNATCH         : return "Protocol driver not attached";
            case ENOCSI          : return "No CSI structure available";
            case EL2HLT          : return "Level 2 halted";
            case EBADE           : return "Invalid exchange";
            case EBADR           : return "Invalid request descriptor";
            case EXFULL          : return "Exchange full";
            case ENOANO          : return "No anode";
            case EBADRQC         : return "Invalid request code";
            case EBADSLT         : return "Invalid slot";
            case EBFONT          : return "Bad font file format";
            case ENOSTR          : return "Device not a stream";
            case ENODATA         : return "No data available";
            case ETIME           : return "Timer expired";
            case ENOSR           : return "Out of streams resources";
            case ENONET          : return "Machine is not on the network";
            case ENOPKG          : return "Package not installed";
            case EREMOTE         : return "Object is remote";
            case ENOLINK         : return "Link has been severed";
            case EADV            : return "Advertise error";
            case ESRMNT          : return "Srmount error";
            case ECOMM           : return "Communication error on send";
            case EPROTO          : return "Protocol error";
            case EMULTIHOP       : return "Multihop attempted";
            case EDOTDOT         : return "RFS specific error";
            case EBADMSG         : return "Not a data message";
            case EOVERFLOW       : return "Value too large for defined data type";
            case ENOTUNIQ        : return "Name not unique on network";
            case EBADFD          : return "File descriptor in bad state";
            case EREMCHG         : return "Remote address changed";
            case ELIBACC         : return "Can not access a needed shared library";
            case ELIBBAD         : return "Accessing a corrupted shared library";
            case ELIBSCN         : return ".lib section in a.out corrupted";
            case ELIBMAX         : return "Attempting to link in too many shared libraries";
            case ELIBEXEC        : return "Cannot exec a shared library directly";
            case EILSEQ          : return "Illegal byte sequence";
            case ERESTART        : return "Interrupted system call should be restarted";
            case ESTRPIPE        : return "Streams pipe error";
            case EUSERS          : return "Too many users";
            case ENOTSOCK        : return "Socket operation on non-socket";
            case EDESTADDRREQ    : return "Destination address required";
            case EMSGSIZE        : return "Message too long";
            case EPROTOTYPE      : return "Protocol wrong type for socket";
            case ENOPROTOOPT     : return "Protocol not available";
            case EPROTONOSUPPORT : return "Protocol not supported";
            case ESOCKTNOSUPPORT : return "Socket type not supported";
            case EOPNOTSUPP      : return "Operation not supported on transport endpoint";
            case EPFNOSUPPORT    : return "Protocol family not supported";
            case EAFNOSUPPORT    : return "Address family not supported by protocol";
            case EADDRINUSE      : return "Address already in use";
            case EADDRNOTAVAIL   : return "Cannot assign requested address";
            case ENETDOWN        : return "Network is down";
            case ENETUNREACH     : return "Network is unreachable";
            case ENETRESET       : return "Network dropped connection because of reset";
            case ECONNABORTED    : return "Software caused connection abort";
            case ECONNRESET      : return "Connection reset by peer";
            case ENOBUFS         : return "No buffer space available";
            case EISCONN         : return "Transport endpoint is already connected";
            case ENOTCONN        : return "Transport endpoint is not connected";
            case ESHUTDOWN       : return "Cannot send after transport endpoint shutdown";
            case ETOOMANYREFS    : return "Too many references: cannot splice";
            case ETIMEDOUT       : return "Connection timed out";
            case ECONNREFUSED    : return "Connection refused";
            case EHOSTDOWN       : return "Host is down";
            case EHOSTUNREACH    : return "No route to host";
            case EALREADY        : return "Operation already in progress";
            case EINPROGRESS     : return "Operation now in progress";
            case ESTALE          : return "Stale NFS file handle";
            case EUCLEAN         : return "Structure needs cleaning";
            case ENOTNAM         : return "Not a XENIX named type file";
            case ENAVAIL         : return "No XENIX semaphores available";
            case EISNAM          : return "Is a named type file";
            case EREMOTEIO       : return "Remote I/O error";
            case EDQUOT          : return "Quota exceeded";
            case ENOMEDIUM       : return "No medium found";
            case EMEDIUMTYPE     : return "Wrong medium type";
        }
        return "Unknown";
    }
};

#endif // UTILS_H
