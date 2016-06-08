#ifndef _ERROR_CODES__
#define _ERROR_CODES__

namespace errors
{
	const int ER_OK							= 0;
    /*  Common errors [1-98]    */
    // Common errors
	const int ER_PUT_INI					= 99;
	const int ER_READ_INI					= 100;
	const int ER_EXTERNAL_ERROR				= 101;
    /*  MySQL errors [102-150]  */
    const int ER_MYSQL_ERROR				= 102;
    const int ER_USER_EXISTS				= 103;
    const int ER_USER_DOESNT_EXIST  		= 104;
    // MySQL Errors

    /* User Input errors    [151-200]   */
    const int ER_PWDS_NOT_MATCH 			= 151;
    // User Input errors

    /*  Auth errors [201-250]*/
    const int ER_USER_IS_ALREADY_ONLINE     = 201;
    const int ER_WRONG_PWD                  = 202;
    const int ER_USER_IS_ALREADY_OFFLINE    = 203;
    // Auth errors


	const int ER_DLL_LOAD_ERROR				= 161;
	const int ER_DLL_UNLOAD_ERROR			= 162;
	const int ER_INTERNAL_ERROR				= 163;	
	const int ER_NETWORK_ERROR				= 104;
	const int ER_INVALID_PARAMETER			= 105;
	const int ER_EMPTY_DATA					= 107;
	const int ER_BAD_RESPONSE			    = 108;
	const int ER_ACTION_NOT_ALLOWED			= 109;
    const int ER_CONNECTION_LOST            = 112;
	const int ER_BAD_CHIP_DATA_FORMAT		= 156;
	const int ER_REGISTRY_OPERATION_ERROR	= 157;
	const int ER_UNKNOWN_FUNCTION			= 206;
	const int ER_PARAM_INVALID				= 233;
};
#endif
