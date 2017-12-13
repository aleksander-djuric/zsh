/*
 * zsh.c
 *
 * Description: Simple shell in C
 * Copyright (c) 2017 Aleksander Djuric. All rights reserved.
 * Distributed under the GNU Lesser General Public License (LGPL).
 * The complete text of the license can be found in the LICENSE
 * file included in the distribution.
 *
 */

#include "zsh.h"

int run(char *cmd, int in, int out);
void splitcmd(char *cmd, char **args);
char *trimspaces(char *str);
void help();

int main() {
	char hostname[NAME_MAX];
	char cwd[NAME_MAX];
	char prompt[PATH_MAX];
	char *user = getlogin();
	char *home = DEF_HOME;
	char *mail = DEF_MAIL;
	char *path = DEF_PATH;
	int in;

	if (gethostname(hostname, NAME_MAX) < 0)
		strcpy(hostname, "localhost");
	printf(CLEAR); // clear screen
	if ((setenv("HOME", home, 1) != 0) ||
		(setenv("MAIL", mail, 1) != 0) ||
		(setenv("PATH", path, 1) != 0)) {
		fprintf(stderr, PACKAGE_NAME": %s\n", strerror(errno));
	}

	while (1) {
		char *cmd, *cur, *nxt;

		snprintf(prompt, PATH_MAX, UCOLOR"%s@%s"RESET":"HCOLOR"%s$ "RESET,
			user, hostname, getcwd(cwd, NAME_MAX));

		cmd = readline(prompt);
		if (!cmd) break;
		if (*cmd == '\0') continue;

		add_history(cmd);
		fflush(NULL);

		cur = cmd;
		in = 0;
		while ((nxt = strchr(cur, '|'))) {
			*nxt = '\0';
			if ((in = run(trimspaces(cur), in, 1)) < 0)
				break;
			cur = nxt + 1;
		}
		if ((in = run(trimspaces(cur), in, 0)) < 0)
			break;
	}

	fprintf(stdout, "\n");

	return 0;
}

int run(char *cmd, int in, int out) {
	char *args[MAX_ARGS];
	int fd[2];
	int rc;
	pid_t pid;

	if (!cmd || *cmd == '\0') return 0;
	splitcmd(cmd, args);
	if (args[0] == NULL) return 0;

	// built in commands
	if (strncmp(args[0], "exit", 4) == 0)
		return -1;
	if (strncmp(args[0], "cd", 2) == 0) {
		if (chdir(args[1]) < 0) strerror(errno);
		return 0;
	} else if (strncmp(args[0], "help", 4) == 0) {
		help();
		return 0;
	}

	if (pipe(fd) < 0) {
		fprintf(stderr, PACKAGE_NAME": %s", strerror(errno));
		return -1;
	}

	if ((pid = fork()) == 0) {
		if (!in && out) { // first
			dup2(fd[OUT], STDOUT_FILENO);
		} else if (in && out) { // middle
			dup2(in, STDIN_FILENO);
			dup2(fd[OUT], STDOUT_FILENO);
		} else if (in && !out) { // last
			dup2(in, STDIN_FILENO);
		}

		if ((rc = execvp(args[0], args)) < 0)
			fprintf(stderr, PACKAGE_NAME": %s %s", args[0], strerror(errno));
		return rc;
	}

	waitpid(pid, &rc, 0);
	if (in) close(in);

	close(fd[OUT]);
	if (!out || rc < 0) { // if last command
		close(fd[IN]);
		return 0;
	}

	return fd[0];
}

char *trimspaces(char *str) {
	char *end;

	if (!str || *str == '\0') return NULL;

	end = str + strlen(str) - 1;
	while (isspace(*str)) str++;
	if (*str == '\0') return NULL;

	while (end > str && isspace(*end)) end--;
	*(end + 1) = '\0';

	return str;
}

void splitcmd(char *cmd, char **args) {
	char *cur, *nxt;
	int i = 0;

	cur = trimspaces(cmd);
	while ((nxt = strchr(cur, ' '))) {
		*nxt = '\0';
		args[i] = trimspaces(cur);
		cur = nxt + 1;
		i++;
	}

	args[i] = trimspaces(cur);
	args[i + 1] = NULL;
}

void help() {
	fprintf(stdout, "Sample shell "PACKAGE_NAME" version "VERSION"\n");
	fprintf(stdout, "Type program names and arguments, and hit enter.\n");
	fprintf(stdout, "Built in commands: cd, exit, help.\n");
}
