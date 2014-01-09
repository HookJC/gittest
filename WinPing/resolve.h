#ifndef _RESOLVE_H_
#define _RESOLVE_H_
// ��C++����������C���Եķ�ʽ����
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