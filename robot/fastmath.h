/* Definitions of fast math library replacements that might not be correct under
 * as many circumstances but might shave off cycle times when sued correctly
 */
#ifndef __FASTMATH_H__
#define __FASTMATH_H__

/* fast ceil using float/integer to double truncation
 * This works well due to hardware bodges in x86 but might not work so well on
 * arm.
 * Only defined correctly for number >=0
 */
#define PCEIL(x) (long)(x+1.0)

#endif
