/**
 * @file String.h
 * @brief String object management.
 *
 */
#ifndef STRING_H
#define STRING_H

#define DEFAULT_SIZE_ALLOC 64


/**
 * \struct String.
 * \brief String object in order to simplify operation on strings.
 */
typedef struct _String
{
	/**
	 * \brief The string (Pointer to char).
	 */
	char* str;	

	/** 
	 * \brief The length of the String.
	 */
	int length;

	/** 
	 * \brief The size of str buffer.
	 */
	int size;

}String;

/**
 * \fn int String_create(String* string, char *str)
 * \brief Allocates memory for a String, and copy str into the char* field
 * \return 0 if the allocation failed, 1 otherwise
 */
int String_create(String *string, char const *str);

/** 
 * \brief Initializes string to an new empty string buffer, able to contain at least size characters.
 * \return 0 if allocation failed, 1 otherwise.
 */
int String_initialize(String *string, int size);

/**
 * \brief Creates a String containing a copy of str, with a maximum of maxCharPerLine characters on each line.
 * \return 0 if allocation failed, 1 otherwise.
 */
int String_createFormattedText(String *dest, char const *str, int maxCharPerLine);

/**
 * \brief Empties the string, without altering the buffer.
 */
void String_empty(String *string);

/**
 * \brief Extends the size of the buffer to the largest between minSize and twice the previous size of the String.
 * \return 0 if allocation failed, 1 otherwise.
 */
int String_extend(String *string, int minSize);

/**
 * \brief Adds one character to the string (extends the buffer if needed).
 * \return 0 if allocation failed, 1 otherwise.
 */
int String_addCharacter(String *string, char c);

/**
 * \brief Inserts the character c at index given in argument. The characters placed after the index are moved.
 * \return 0 if allocation failed or index is invalid, 1 otherwise.
 */
int String_insertAtIndex(String* string, int index, char c);

/**
 * \fn void String_free(String* string)
 * \brief Frees a String
 */
void String_free(String *string);

/**
 * \fn int String_append(String* string, String const *str1, ...)
 * \brief Concatenate strings given in argument with the string result (result could be NULL or not NULL).
 * \return 0 if the allocation of result failed, 1 otherwise.
 */
int String_append(String* result, char const *str1, ...);

/**
 * \fn int String_compare(const String* str1, const String* str2)
 * \brief Compare 2 strings lexicographically
 * \return 0 if str1 equals str2 , a negative if str1 < str2 and a positive if str1 > str2
 */
int String_compare(String const* str1, String const* str2);

/**
 * \fn int String_parseInt(String const* string, int *value)
 * \brief Convert a digit string to an integer
 * \return 0 if the conversion failed, 1 otherwise
 */
int String_parseInt(String const* string, int *value);

/**
 * \fn int String_putInteger(String *result, int value)
 * \brief Convert an integer to a string
 * \return 0 if allocation failed, 1 otherwise
 */
int String_putInteger(String *result, int value);

#endif