#include "pool.h"
#include "svn_pools.h"

/**
 * SvnCpp namespace
 */
namespace svn
{
    Pool::Pool()
    {
	m_pool = svn_pool_create(NULL);
    }

    Pool::Pool(apr_pool_t* parent)
    {
	m_pool = svn_pool_create(parent);
    }

    Pool::Pool(Pool* pool)
    {
	if( pool != this )
	{
	    m_pool = pool->pool();
	}
    }

    Pool::~Pool()
    {
	if( m_pool )
	{
	    svn_pool_destroy(m_pool);
	}
    }

    apr_pool_t *
    Pool::pool()
    {
	return m_pool;
    }
}
