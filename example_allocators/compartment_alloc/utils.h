#include <stdlib.h>
#include <sys/sysctl.h>

// A shortcut to turn a pointer into a sealed capability
#define sealed_reference(ptr) cheri_seal((void *__capability) ptr, cheri_get_sealcap())

void *__capability cheri_get_sealcap()
{
    void *__capability sealcap;
    size_t sealcap_size = sizeof(sealcap);
    if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
    {
        printf("Fatal error. Cannot get `security.cheri.sealcap`.");
        exit(1);
    }
    return sealcap;
}
