#include "include/common.h"
#include <cheriintrin.h>
#include <machine/sysarch.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>

int main()
{
	void *sealcap;
	size_t sealcap_size = sizeof(sealcap);
	if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
	{
		error("Fatal error. Cannot get `security.cheri.sealcap`.");
		exit(1);
	}

	printf("---- sealcap ----\n");
	pp_cap(sealcap);

	void *buffer = malloc(64);
	printf("---- buffer (before sealing) ----\n");
	pp_cap(buffer);

	void *sealed = cheri_seal(buffer, sealcap);
	printf("---- sealed ----\n");
	pp_cap(sealed);

	free(buffer);
	return 0;
}
