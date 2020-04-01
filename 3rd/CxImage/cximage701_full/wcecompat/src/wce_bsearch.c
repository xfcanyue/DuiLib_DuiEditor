
#include <stdlib.h>
#include <assert.h>

void* bsearch(const void *key, const void *base, size_t num, size_t width,
                    int (*compare)(const void *, const void *))
{
	while (num-- > 0)
	{
		if (compare(key, base) == 0)
			return (void*)base;

		base = (char*)base + width;
	}
    return NULL;
}
