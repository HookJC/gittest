#ifndef _RESOLVE_H_
#define _RESOLVE_H_
// 在C++编译器中以C语言的方式编译
#ifdef _cplusplus
extern "C" {
#endif
 
int              PrintAddress(SOCKADDR *sa, int salen);
int              FormatAddress(SOCKADDR *sa, int salen, char *addrbuf, int addrbuflen);
int              ReverseLookup(SOCKADDR *sa, int salen, char *namebuf, int namebuflen);
struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto);
 
#ifdef _cplusplus
}
#endif
 
#endif