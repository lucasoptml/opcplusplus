
#define BOOST_SYSTEM_HAS_SYSTEM_ERROR

#include "boost/system/error_category.hpp"
#include "boost/system/error_code.hpp"

struct boostreference
{
    boost::system::error_category 
    const& reference_generic_category___() { return boost::system::generic_category(); }
    
    boost::system::error_category 
    const& reference_system_category___() { return boost::system::system_category(); }
};