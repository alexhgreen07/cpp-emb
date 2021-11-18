#ifndef __EMB_ASSERT_HPP__
#define __EMB_ASSERT_HPP__

#ifdef EMB_CUSTOM_ASSERT

void emb_assert(bool cond, int line, const char * filename);
#define EMB_ASSERT(cond) emb_assert(cond, __LINE__, __FILE__);

#else

#include<assert.h>
#define EMB_ASSERT(cond) assert(cond);

#endif // EMB_CUSTOM_ASSERT

#endif // __EMB_ASSERT_HPP__
