#include <bci/bci_package.h>
extern int builtin_library_init();
extern void builtin_library_finalize();
int bci_package_init()
{
	return builtin_library_init();
}
void bci_package_finalize()
{
	builtin_library_finalize();
}
