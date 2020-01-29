#ifndef _TC_CONTEXT_FCONTEXT_H
#define _TC_CONTEXT_FCONTEXT_H

namespace tars
{


typedef void*   fcontext_t;

struct transfer_t {
    fcontext_t  fctx;
    void    *   data;
};

extern "C" transfer_t jump_fcontext( fcontext_t const to, void * vp);
extern "C" fcontext_t make_fcontext( void * sp, std::size_t size, void (* fn)( transfer_t) );

// extern "C" intptr_t jump_fcontext( fcontext_t * ofc, fcontext_t const* nfc, intptr_t vp, bool preserve_fpu = true);
// extern "C" fcontext_t * make_fcontext( void * sp, std::size_t size, void (* fn)( intptr_t) );

}


#endif
