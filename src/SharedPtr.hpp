#ifndef GRAPH_ANALYSIS_SHARED_PTR_HPP
#define GRAPH_ANALYSIS_SHARED_PTR_HPP

#if __cplusplus <= 199711L
#define USE_BOOST_SHARED_PTR
#endif

#ifdef USE_BOOST_SHARED_PTR
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#else
#include <memory>
#include <functional>
#endif

namespace graph_analysis
{
#ifdef USE_BOOST_SHARED_PTR
    using ::boost::shared_ptr;
    using ::boost::enable_shared_from_this;
    using ::boost::make_shared;
    using ::boost::dynamic_pointer_cast;
    using ::boost::static_pointer_cast;
    using ::boost::const_pointer_cast;
    using ::boost::function1;
#else
    using ::std::shared_ptr;
    using ::std::make_shared;
    using ::std::dynamic_pointer_cast;
    using ::std::static_pointer_cast;
    using ::std::const_pointer_cast;
    template <class T, class U>
    using function1 = ::std::function<T(U)>;
    using ::std::enable_shared_from_this;
#endif
}

#endif // GRAPH_ANALYSIS_SHARED_PTR_HPP
