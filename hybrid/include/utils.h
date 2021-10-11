/***
 * Inline Assembly utility functions to read/write to/from the Default Data
 * Capability Register (DDC)
 ***/

#if !defined(__aarch64__)
#error "These utility functions are Morello only"
#endif

void write_ddc(void *__capability cap);
void *__capability read_ddc();

// Write a capability to the DDC
inline void write_ddc(void *__capability cap)
{
	asm("MSR DDC, %[cap]\n\t" : : [cap] "C"(cap) : "memory");
}

// Read a capability from the DDC
inline void *__capability read_ddc()
{
	void *__capability ddc_cap;
	asm("MRS %[ddc_cap], DDC\n\t" : [ddc_cap] "=C"(ddc_cap) : :);
	return ddc_cap;
}