/**
 * Timer infrastucture.
 *
 * Copyright (C) 2007-2008, Bernhard Kauer <bk@vmmon.org>
 *
 * This file is part of Vancouver.
 *
 * Vancouver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Vancouver is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */
#pragma once

#include <nova/cpu.h>
#include <vmm/math.h>
#include <driver/logging.h>


typedef unsigned long long timevalue;

/**
 * A clock returns the time in different time domains.
 *
 * The reference clock is the CPUs TSC.
 */
class Clock
{
 protected:
  timevalue _source_freq;
 public:
#ifdef TESTING
  virtual 
#endif
  timevalue time() { return Cpu::rdtsc(); }

  /**
   * Returns the current clock in freq-time.
   */
  timevalue clock(timevalue freq) { return Math::muldiv128(time(), freq, _source_freq); }
  
  /**
   * Returns a timeout in absolute TSC time.
   */
  timevalue abstime(timevalue delta, timevalue freq) {  return time() + Math::muldiv128(delta, _source_freq, freq); }
  
  Clock(timevalue source_freq) : _source_freq(source_freq) { Logging::printf("source freq %lld\n", source_freq); }
};




/**
 * Keeping track of the timeouts.
 */
template <unsigned ENTRIES>
class TimeoutList
{
  class TimeoutEntry
  {
    friend class TimeoutList<ENTRIES>;
    TimeoutEntry *_next;
    TimeoutEntry *_prev;
    timevalue _timeout;
  };

  TimeoutEntry  _entries[ENTRIES];
  unsigned      _count;
public:
  /**
   * Alloc a new timeout object.
   */
  unsigned alloc()
  {
    if (_count < ENTRIES-1) return ++_count;
    Logging::printf("can not alloc a timer %d vs %d!\n", _count, ENTRIES);
    return 0;
  }


  /**
   * Cancel a programmed timeout.
   */
  int cancel(unsigned nr)
  {
    if (!nr || nr >= ENTRIES)  return -1;
    TimeoutEntry *current = _entries+nr;
    //Logging::printf("cancel %x prev %p next %p\n", nr, current->_prev, current->_next);
    if (current->_next == current) return -2;
    int res = _entries[0]._next == current;
    
    current->_next->_prev =  current->_prev;
    current->_prev->_next =  current->_next;
    current->_next = current->_prev = current;
    return res;
  }
    
  /**
   * Request a new timeout.
   */
  int request(unsigned nr, timevalue to)
  {
    if (!nr || nr > ENTRIES)  return -1;
    TimeoutEntry *current = _entries + nr;
    cancel(nr);
    
    // keep a sorted list here
    TimeoutEntry *t = _entries;
    do { t = t->_next; }  while (t->_timeout < to);

    current->_timeout = to;
    current->_next = t;
    current->_prev = t->_prev;
    t->_prev->_next = current;
    t->_prev = current;
    //Logging::printf("request %x to %llx toh %llx\n", nr, to, timeout());
    return timeout() != to;
  }

  /**
   * Get the head of the queue.
   */
  unsigned  trigger(timevalue now) {  
    if (now >= timeout())  return (_entries[0]._next - _entries);
    return 0;
  }

  timevalue timeout() { assert(_entries[0]._next); return _entries[0]._next->_timeout; }
  void init()
  {
    _count = 0;
    for (unsigned i = 0; i < ENTRIES; i++) 
      {
	_entries[i]._prev = _entries + i;
	_entries[i]._next = _entries + i;
      }
    _entries[0]._timeout = ~0ULL; 
  }
};