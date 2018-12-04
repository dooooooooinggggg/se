#define C(val)                                                                                                              \
    do                                                                                                                      \
    {                                                                                                                       \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %c\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                     \
    } while (0)
#define D(val)                                                                                                              \
    do                                                                                                                      \
    {                                                                                                                       \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %d\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                     \
    } while (0)
#define P(val)                                                                                                              \
    do                                                                                                                      \
    {                                                                                                                       \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %p\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                     \
    } while (0)
#define L(val)                                                                                                               \
    do                                                                                                                       \
    {                                                                                                                        \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %ld\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                      \
    } while (0)
#define LL(val)                                                                                                               \
    do                                                                                                                        \
    {                                                                                                                         \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %lld\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                       \
    } while (0)
#define S(val)                                                                                                              \
    do                                                                                                                      \
    {                                                                                                                       \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %s\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                     \
    } while (0)
#define F(val)                                                                                                              \
    do                                                                                                                      \
    {                                                                                                                       \
        fprintf(stderr, "%ld %16s %4d %16s %16s: %f\n", (long int)pthread_self(), __FILE__, __LINE__, __func__, #val, val); \
        fflush(stderr);                                                                                                     \
    } while (0)
#define N                                                                                               \
    do                                                                                                  \
    {                                                                                                   \
        fprintf(stderr, "%ld %16s %4d %16s\n", (long int)pthread_self(), __FILE__, __LINE__, __func__); \
        fflush(stderr);                                                                                 \
    } while (0)
#define ERR              \
    do                   \
    {                    \
        perror("ERROR"); \
        N;               \
        exit(1);         \
    } while (0)
#define ERR2                \
    do                      \
    {                       \
        perror("ERROR");    \
        N;                  \
        pthread_exit(NULL); \
    } while (0)
