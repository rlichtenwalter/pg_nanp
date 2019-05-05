#include <postgres.h>
#include <fmgr.h>
#include <stdint.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define NUM_CAPTURES 16
#define MESSAGE_BUFFER_SIZE 256

PG_FUNCTION_INFO_V1( nanp_in );
Datum nanp_in( PG_FUNCTION_ARGS ) {
	char const * str;
	char const * regex_str;
	regmatch_t captures[NUM_CAPTURES];
	regex_t regex;
	int compile_code;
	char message_buffer[MESSAGE_BUFFER_SIZE];
	int buffer_required;
	int exec_code;
	char const * result;
	long result_long;
	str = PG_GETARG_CSTRING( 0 );
	regex_str = "^\\(1[-. ]\\?\\)\\?\\((\\([2-9][0-9]\\{2\\}\\))\\|\\([2-9][0-9]\\{2\\}\\)\\)[-. ]\\?\\([2-9]\\(1[023456789]\\|[023456789][0-9]\\)\\)[-. ]\\?\\([0-9]\\{4\\}\\)$";
	compile_code = regcomp( &regex, regex_str, 0 );
	if( compile_code ) {
		buffer_required = regerror( compile_code, &regex, message_buffer, MESSAGE_BUFFER_SIZE );
		(void)buffer_required; // do not use the return value for now
		ereport( ERROR, (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),errmsg( "regular expression compilation failed: %s in '%s'\n", message_buffer, regex_str )) );
	}
	exec_code = regexec( &regex, str, NUM_CAPTURES, captures, 0 );
	if( !exec_code ) {
		result = psprintf( "%c%c%c%c%c%c%c%c%c%c",
				str[ captures[2].rm_so + 0 ] != '(' ? str[ captures[2].rm_so + 0 ] : str[ captures[2].rm_so + 1 ],
				str[ captures[2].rm_so + 0 ] != '(' ? str[ captures[2].rm_so + 1 ] : str[ captures[2].rm_so + 2 ],
				str[ captures[2].rm_so + 0 ] != '(' ? str[ captures[2].rm_so + 2 ] : str[ captures[2].rm_so + 3 ],
				str[ captures[5].rm_so + 0 ],
				str[ captures[5].rm_so + 1 ],
				str[ captures[5].rm_so + 2 ],
				str[ captures[7].rm_so + 0 ],
				str[ captures[7].rm_so + 1 ],
				str[ captures[7].rm_so + 2 ],
				str[ captures[7].rm_so + 3 ]
				 );
		result_long = atol( result );
		PG_RETURN_INT64( result_long );
	} else if( exec_code == REG_NOMATCH ) {
		ereport( ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),errmsg("invalid input format for nanp: '%s'", str)) );
	} else {
		buffer_required = regerror( compile_code, &regex, message_buffer, MESSAGE_BUFFER_SIZE );
		(void)buffer_required; // do not use the return value for now
		ereport( ERROR, (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),errmsg( "unexpected error: %s executing '%s' on '%s'", message_buffer, regex_str, str )) );
	}
	regfree( &regex );
}

PG_FUNCTION_INFO_V1( nanp_out );
Datum nanp_out( PG_FUNCTION_ARGS ) {
	long nanp;
	char const * result;
	char const * formatted_result;
	nanp = PG_GETARG_INT64( 0 );
	result = psprintf( "%lu", nanp );
	formatted_result = psprintf( "%c%c%c-%c%c%c-%c%c%c%c",
			result[0],
			result[1],
			result[2],
			result[3],
			result[4],
			result[5],
			result[6],
			result[7],
			result[8],
			result[9]
			);
	PG_RETURN_CSTRING( formatted_result );
}
