//=======================================================================
// Copyright 2015 by Ireneusz Szcześniak
// Authors: Ireneusz Szcześniak <www.irkos.org>
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef BOOST_GRAPH_EXCLUDE_FILTER
#define BOOST_GRAPH_EXCLUDE_FILTER

#include <set>

namespace boost {

  // This filter excludes the given descriptors (edges or vertexes).
  template <typename Descriptor>
  struct exclude_filter
  {
    // The data set type.
    typedef typename std::set<Descriptor> ds_type;

    // The filter must be default-constructible, so it is.
    exclude_filter(): m_excluded() {};

    exclude_filter(const ds_type *excluded): m_excluded(excluded) {};

    inline bool operator()(const Descriptor &e) const
    {
      return m_excluded->count(e) == 0;
    }

    const ds_type *m_excluded;
  };

} // boost

#endif /* BOOST_GRAPH_EXCLUDE_FILTER */
