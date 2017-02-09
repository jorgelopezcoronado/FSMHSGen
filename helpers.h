#include <sys/time.h> 

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL
//code seems cleaner with this boolean representations
#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
#endif

#ifndef loglevel_decl
#define loglevel_decl
typedef enum 
{
	emergency=0,
	alert,
	critical,
	error,
	warning,
	notice,
	info,
	debug
}loglevel;	
#endif

#ifndef logtype_decl
#define logtype_decl
typedef enum
{
	none = 0,
	local,
	syslog_log
}logtype;
#endif

#ifndef logtype
//define log level and log type, and log name
extern loglevel log_level;
extern logtype log_type; 
extern const char *log_name;
#endif

/*
 *  * timeval_substract takes two values to substract and put the result in the result parameter.
 */
int timeval_substract(struct timeval*, struct timeval*, struct timeval*);

/*
 * timeval_isgreaterthan takes t1 and t2 pointers and return if t1 > t2
 */
BOOL timeval_isgreaterthan(struct timeval*, struct timeval*);

/*
 * bitstream_equal: takes two u_cahr* a lenth and compare them
 */
BOOL bitstream_equal(unsigned char*, unsigned char*, unsigned long long); 

/*
 * strtrim trims the desired string, beware it modifies the original string.
 */

void strtrim(char **string);

/*
 * print_timeval: easy func to display timeval
 */
void print_timeval(struct timeval*, BOOL, BOOL);

//**
//log in accordance to paramters
//**/
void fsm_log(loglevel ll, char *msg);

#ifdef __cplusplus
}
#endif
