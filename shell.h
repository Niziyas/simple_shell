#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/** for read/write buffers */
#define read_buffer_data_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/** for command chaining */
#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/** for int_to_base() */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/** 1 if using system getline() */
#define USEget_next_input_line 0
#define USE_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguments to pass into a function,
 * allowing uniform prototype for the function pointer struct
 * @arg: a string generated from getline containing arguments
 * @argv: an array of strings generated from arguments
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on, count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;
	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{ \
	NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0 \
}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* Function Prototypes */
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);
int isExecutableCommand(info_t *, char *);
char *duplicateChars(char *, int, int);
char *findPath(info_t *, char *, char *);
int loophsh(char **);
void printString(char *);
int printCharToStderr(char);
int writeCharToFD(char c, int fd);
int writeStringToFD(char *str, int fd);
int string_length(char *);
int string_compare(char *, char *);
char *find_start(const char *, const char *);
char *string_concat(char *, char *);
char *string_copy(char *, char *);
char *string_duplicate(const char *);
void print_string(char *);
int write_char_to_stdout(char);
char *copy_n_characters(char *, char *, int);
char *concatenate_n_characters(char *, char *, int);
char *find_character(char *, char);
char **split_string(char *, char *);
char **split_string2(char *, char);
char *fillMemory(char *, char, unsigned int);
void freeStrings(char **);
void *myRealloc(void *, unsigned int, unsigned int);
int free_and_null(void **);
int is_interactive(info_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int string_to_int(char *);
int str_to_int(char *);
void print_error_message(info_t *, char *);
int print_decimalecimal(int, int);
char *int_to_base(long int, int, int);
void remove_first_comment(char *);
int my_exit(info_t *);
int my_cd(info_t *);
int my_help(info_t *);
int my_history(info_t *);
int my_alias(info_t *);
ssize_t read_and_parse_input(info_t *);
int get_next_input_line(info_t *, char **, size_t *);
void block_ctrl_c(int);
void initialize_info_struct(info_t *);
void initialize_info(info_t *, char **);
void free_info_struct_fields(info_t *, int);
int printEnvironment(info_t *);
char *getEnvValue(info_t *, const char *);
int setEnvironmentVariable(info_t *);
int unsetEnvironmentVariable(info_t *);
int populateEnvironmentList(info_t *);
char **copy_environment(info_t *);
int remove_environment_variable(info_t *, char *);
int initialize_or_modify_environment(info_t *, char *, char *);
char *allocate_history_file(info_t *info);
int save_history(info_t *info);
int load_history(info_t *info);
int add_to_history(info_t *info, char *buf, int linecount);
int update_history_numbers(info_t *info);
list_t *prepend_node(list_t **, const char *, int);
list_t *prepend_node_end(list_t **, const char *, int);
size_t print_environment_list(const list_t *);
int remove_node_at_index(list_t **, unsigned int);
void clear_list(list_t **);
size_t count_list(const list_t *);
char **convert_list_to_strings(list_t *);
size_t print_history(const list_t *);
list_t *find_node_with_prefix(list_t *, char *, char);
ssize_t find_node_index(list_t *, list_t *);
int findCharacter(info_t *, char *, size_t *);
void checkChain(info_t *, char *, size_t *, size_t, size_t);
int replaceAlias(info_t *);
int replaceAlias(info_t *);
int replace_string(char **, char *);

#endif
