/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:01:00 by mhaizan           #+#    #+#             */
/*   Updated: 2025/08/26 15:01:00 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** LIBFT - 42 STANDARD LIBRARY
** =============================================================================
**
** Libft is a foundational project at 42 School where students recreate
** essential C library functions. This library provides string manipulation,
** memory management, and utility functions that are used throughout other
** 42 projects including pipex.
**
** LIBRARY ORGANIZATION:
** ---------------------
** 1. Character Classification (ft_is*)
**    - Check character types (alpha, digit, printable, etc.)
**
** 2. Character Conversion (ft_to*)
**    - Convert between upper/lower case
**
** 3. String Operations (ft_str*)
**    - String manipulation and searching functions
**
** 4. Memory Operations (ft_mem*)
**    - Memory manipulation and allocation
**
** 5. Number Conversion (ft_atoi, ft_itoa)
**    - Convert between strings and integers
**
** 6. File Descriptor Output (ft_put*_fd)
**    - Write data to specified file descriptors
**
** 7. Linked List Operations (ft_lst*)
**    - Bonus functions for linked list manipulation
**
** FUNCTIONS USED BY PIPEX:
** ------------------------
** - ft_split():   Splits command strings into argument arrays
** - ft_strjoin(): Concatenates paths with commands
** - ft_strlen():  Gets string length for write() calls
** - ft_strchr():  Checks if command contains '/'
** - ft_strncmp(): Finds PATH in environment variables
** - ft_strdup():  Duplicates path strings
** - ft_substr():  Used internally by ft_split()
** - ft_calloc():  Used internally for allocation
**
** =============================================================================
*/

#ifndef LIBFT_H
# define LIBFT_H

/*
** =============================================================================
** SYSTEM INCLUDES
** =============================================================================
*/

/* limits.h: Provides INT_MAX, INT_MIN constants for overflow checking */
# include <limits.h>

/* stdlib.h: Provides malloc(), free(), NULL for memory management */
# include <stdlib.h>

/* unistd.h: Provides write() for output functions */
# include <unistd.h>

/* stdint.h: Provides SIZE_MAX for overflow protection in calloc */
# include <stdint.h>

/*
** =============================================================================
** CHARACTER CLASSIFICATION FUNCTIONS
** =============================================================================
** These functions test whether a character belongs to a certain class.
** They return non-zero (true) if the character matches, 0 (false) otherwise.
*/

/* ft_isalpha: Checks if character is alphabetic (a-z, A-Z) */
int		ft_isalpha(int c);

/* ft_isdigit: Checks if character is a decimal digit (0-9) */
int		ft_isdigit(int c);

/* ft_isalnum: Checks if character is alphanumeric (letter or digit) */
int		ft_isalnum(int c);

/* ft_isascii: Checks if character is valid ASCII (0-127) */
int		ft_isascii(int c);

/* ft_isprint: Checks if character is printable (32-126) */
int		ft_isprint(int c);

/*
** =============================================================================
** CHARACTER CONVERSION FUNCTIONS
** =============================================================================
*/

/* ft_toupper: Converts lowercase letter to uppercase */
int		ft_toupper(int c);

/* ft_tolower: Converts uppercase letter to lowercase */
int		ft_tolower(int c);

/*
** =============================================================================
** STRING FUNCTIONS
** =============================================================================
** Functions for string manipulation, searching, and modification.
*/

/* ft_strlen: Returns the length of a string (excluding null terminator) */
/* Used in pipex for write() calls to output command names */
size_t	ft_strlen(const char *s);

/* ft_strchr: Finds first occurrence of character in string */
/* Used in pipex to check if command contains '/' (is a path) */
char	*ft_strchr(const char *s, int c);

/* ft_strrchr: Finds last occurrence of character in string */
char	*ft_strrchr(const char *s, int c);

/* ft_strncmp: Compares first n characters of two strings */
/* Used in pipex to find "PATH=" in environment variables */
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/* ft_strnstr: Locates substring in string within n characters */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

/* ft_strlcpy: Copies string with size limit, ensures null termination */
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);

/* ft_strlcat: Concatenates strings with size limit */
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);

/* ft_strdup: Allocates and returns a copy of a string */
/* Used in pipex when command already contains '/' */
char	*ft_strdup(const char *s);

/* ft_substr: Extracts substring from string */
/* Used internally by ft_split() to extract each word */
char	*ft_substr(const char *s, unsigned int start, size_t len);

/* ft_strjoin: Concatenates two strings into new allocation */
/* Used in pipex to build paths: "/usr/bin" + "/" + "cmd" */
char	*ft_strjoin(const char *s1, const char *s2);

/* ft_strtrim: Removes leading/trailing characters from string */
char	*ft_strtrim(char const *s1, char const *set);

/* ft_split: Splits string by delimiter into array of strings */
/* Critical for pipex - splits "cmd arg1 arg2" into {"cmd", "arg1", "arg2"} */
/* Also splits PATH variable by ':' into directory array */
char	**ft_split(const char *s, char c);

/* ft_itoa: Converts integer to string */
char	*ft_itoa(int n);

/* ft_strmapi: Applies function to each character, returns new string */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

/* ft_striteri: Applies function to each character in place */
void	ft_striteri(char *s, void (*f)(unsigned int, char*));

/*
** =============================================================================
** MEMORY FUNCTIONS
** =============================================================================
** Functions for memory allocation, copying, and manipulation.
*/

/* ft_memset: Fills memory with a constant byte */
void	*ft_memset(void *b, int c, size_t len);

/* ft_bzero: Sets memory to zero */
void	ft_bzero(void *s, size_t n);

/* ft_memcpy: Copies memory area (non-overlapping) */
void	*ft_memcpy(void *dst, const void *src, size_t n);

/* ft_memmove: Copies memory area (handles overlapping) */
void	*ft_memmove(void *dst, const void *src, size_t len);

/* ft_memcmp: Compares memory areas */
int		ft_memcmp(const void *s1, const void *s2, size_t n);

/* ft_memchr: Finds first occurrence of byte in memory */
void	*ft_memchr(const void *s, int c, size_t n);

/* ft_calloc: Allocates memory and initializes to zero */
/* Used internally by ft_split() for safe allocation */
void	*ft_calloc(size_t count, size_t size);

/*
** =============================================================================
** CONVERSION FUNCTIONS
** =============================================================================
*/

/* ft_atoi: Converts string to integer */
int		ft_atoi(const char *str);

/*
** =============================================================================
** FILE DESCRIPTOR OUTPUT FUNCTIONS
** =============================================================================
** Functions that write to a specified file descriptor.
*/

/* ft_putchar_fd: Writes a character to file descriptor */
void	ft_putchar_fd(char c, int fd);

/* ft_putstr_fd: Writes a string to file descriptor */
void	ft_putstr_fd(char *s, int fd);

/* ft_putendl_fd: Writes a string followed by newline to file descriptor */
void	ft_putendl_fd(char *s, int fd);

/* ft_putnbr_fd: Writes an integer to file descriptor */
void	ft_putnbr_fd(int n, int fd);

/*
** =============================================================================
** LINKED LIST STRUCTURES AND FUNCTIONS (BONUS)
** =============================================================================
** A generic linked list implementation for bonus exercises.
*/

/*
** t_list - Linked list node structure
** -----------------------------------
** @content: Generic pointer to data (can point to any type)
** @next: Pointer to next node in the list
**
** Usage:
**   t_list *node = ft_lstnew("hello");
**   ft_lstadd_back(&head, node);
*/
typedef struct s_list
{
	void			*content;  /* Data stored in this node */
	struct s_list	*next;     /* Pointer to next node (NULL if last) */
}	t_list;

/* ft_lstnew: Creates a new list node with given content */
t_list	*ft_lstnew(void *content);

/* ft_lstadd_front: Adds node to beginning of list */
void	ft_lstadd_front(t_list **lst, t_list *new);

/* ft_lstsize: Counts number of nodes in list */
int		ft_lstsize(t_list *lst);

/* ft_lstlast: Returns the last node of list */
t_list	*ft_lstlast(t_list *lst);

/* ft_lstadd_back: Adds node to end of list */
void	ft_lstadd_back(t_list **lst, t_list *new);

/* ft_lstdelone: Deletes a single node using del function */
void	ft_lstdelone(t_list *lst, void (*del)(void*));

/* ft_lstclear: Deletes entire list using del function */
void	ft_lstclear(t_list **lst, void (*del)(void*));

/* ft_lstiter: Applies function to content of each node */
void	ft_lstiter(t_list *lst, void (*f)(void*));

/* ft_lstmap: Creates new list by applying function to each node */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*));

#endif
