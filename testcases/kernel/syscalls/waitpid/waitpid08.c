/*
 * Copyright (c) International Business Machines  Corp., 2001
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * History
 *	07/2001 John George
 *		-Ported
 *      04/2002 wjhuie sigset cleanups
 */

/*
 * DESCRIPTION
 *	Tests to see if pids returned from fork and waitpid are same.
 *
 * ALGORITHM
 *	Check proper functioning of waitpid with pid = -1 and arg =
 *	WUNTRACED
 */

#include "waitpid_common.h"

static void do_child_1(void)
{
	pid_t pid;
	int i;
	int status;

	for (i = 0; i < MAXKIDS; i++) {
		if (i == (MAXKIDS / 2))
			SAFE_SETPGID(0, 0);

		pid = SAFE_FORK();
		if (pid == 0)
			do_exit(1);

		fork_kid_pid[i] = pid;
	}

	/* Check that waitpid with WNOHANG|WUNTRACED returns
	 * zero
	 */
	if (TST_TRACE(waitpid_ret_test(-1, &status, WNOHANG | WUNTRACED, 0, 0)))
		return;

	TST_CHECKPOINT_WAKE2(0, MAXKIDS);

	if (TST_TRACE(reap_children(-1, WUNTRACED, fork_kid_pid, MAXKIDS)))
		return;

	tst_res(TPASS, "Test PASSED");
}

static struct tst_test test = {
	.tid = "waitpid08",
	.forks_child = 1,
	.needs_checkpoints = 1,
	.setup = waitpid_setup,
	.cleanup = waitpid_cleanup,
	.test_all = waitpid_test,
};
