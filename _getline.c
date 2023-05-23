/*get_line() gets a line from a file descriptor.*/
/*The line is returned as a pointer to a newly allocated string.*/
char *__get_line(const int fd) {
	/* Check if the file descriptor is valid.*/
	if (fd == -1) {
		/* If the file descriptor is not valid, return NULL.*/
		return NULL;
	}

	/* Get the linked list node for the file descriptor.*/
	struct FdBuf *fb = get_fdbuf(&head, fd);

	/* Check if the linked list node is not null.*/
	if (!fb) {
		/* If the linked list node is null, return NULL.*/
		return NULL;
	}

	/* Allocate a buffer to store the line.*/
	char *line = malloc(READ_SIZE + 1);

	/* Check if the buffer allocation was successful.*/
	if (!line) {
		/* If the buffer allocation was not successful, return NULL.*/
		return NULL;
	}

	/* Read data from the file descriptor into the buffer until a newline character is found.*/
	ssize_t r;
	while ((r = read(fb->fd, line, READ_SIZE)) > 0) {
		/* Find the first occurrence of a newline character in the buffer.*/
		char *p = strchr(line, '\n');

		/* If a newline character is found, break out of the loop.*/
		if (p) {
			break;
		}
	}

	/* If the end of the file is reached, check if the buffer contains a newline character.*/
	if (r == 0) {
		/* If the buffer does not contain a newline character, return NULL.*/
		if (line[0] != '\n') {
			free(line);
			return NULL;
		}
	}

	/* Replace the newline character in the buffer with a null character.*/
	if (p) {
		*p = '\0';
	} else {
		/* If no newline character was found, set the length of the line to 0.*/
		line[0] = '\0';
	}

	/* Return the line.*/
	return line;
}

/*
 * read_buf() reads data from a file descriptor into a buffer.
 * The number of bytes read is returned.
 */
ssize_t __read_buf(struct FdBuf *fb, char *buf) {
	/* Check if the linked list node is not null.*/
	if (!fb) {
		/* If the linked list node is null, return -1.*/
		return -1;
	}

	/* Read data from the file descriptor into the buffer.*/
	ssize_t r = read(fb->fd, buf, READ_SIZE);
	return r;
}

/*
 * get_fdbuf() gets a linked list node for a file descriptor.
 * The linked list node is returned as a pointer to a struct FdBuf.
 */
struct FdBuf *get_fdbuf(struct FdBuf *head, const int fd) {
	for (; head && head->fd <= fd; head = head->next)
		;
	return head;
}

/* strchr() finds the first occurrence of a character in a string.
 * The pointer to the first occurrence of the character is returned.
 */
char *__strchr(char *s, char c) {
	if (!s) {
		return NULL;
	}

	/* Iterate over the string until the character is found.*/
	while (*s != c) {
		if (*s == '\0') {
			return NULL;
		}
		s++;
	}
	return s;
}
