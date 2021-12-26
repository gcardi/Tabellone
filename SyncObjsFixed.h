//---------------------------------------------------------------------------

#ifndef SyncObjsFixedH
#define SyncObjsFixedH

#include <stdint.h>

#include <System.SyncObjs.hpp>

/*
  http://www.delphitools.info/2011/11/30/fixing-tcriticalsection/

  TCriticalSection (along  with TMonitor*)  suffers from  a severe  design
  flaw in which entering/leaving different TCriticalSection instances  can
  end  up  serializing  your  threads,  and  the  whole  can  even  end up
  performing  worse than  if your  threads had  been serialized.  This  is
  because  it’s  a  small,   dynamically  allocated  object,  so   several
  TCriticalSection instances can  end up in  the same CPU  cache line, and
  when that happens, you’ll have cache conflicts aplenty between the cores
  running the threads.  How severe can  that be? Well,  it depends on  how
  many cores you have, but the more cores you have, the more severe it can
  get. On a  quad core, a  bad case of  contention can easily  result in a
  200%  slowdown on  top of  the serialization.  And it  won’t  always  be
  reproducible, since it’s related to dynamic memory allocation. There  is
  thankfully a simple fix for that, use TFixedCriticalSection:
*/

class TFixedCriticalSection : public TCriticalSection
{
private:
    uint8_t dummy_[96];
};

/*
  That’s it folks. This makes sure the instance size larger than 96 bytes,
  which means  that it’ll  be larger  than the  cache line  in all current
  CPUs,  so  no  serialization anymore  across  distinct  critical section
  instances. As a  bonus, it also  ends up using  one of the  larger, more
  aligned, FastMM  bucket, which  seems to  improve critical  section code
  performance by about 7%. The downside is you use more RAM… but how  many
  critical sections do you really have?

  http://www.delphitools.info/2011/11/30/fixing-tcriticalsection/
*/

//---------------------------------------------------------------------------
#endif
