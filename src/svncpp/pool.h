#ifndef _SVNCPP_POOL_H_
#define _SVNCPP_POOL_H_

/**
 * Forward declarations
 */
struct apr_pool_t;

/**
 * SvnCpp namespace
 */
namespace svn
{
    /**
     * Class for encapsulation of apr/subversion pools
     */
    class Pool
	{
	public:
	    /**
	     * creates a new pool
	     */
	    Pool();

	    /**
	     * creates a new pool to an existing pool
	     */
	    Pool(apr_pool_t * parent);

	    /**
	     * copy constructor
	     */
	    Pool(Pool* pool);

	    virtual ~Pool();

	    /**
	     * returns the handle to the pool
	     */
	    apr_pool_t * pool();
		
	private:
	    apr_pool_t * m_pool;
	};
}
	    
#endif
