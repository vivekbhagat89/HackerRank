#define _CRT_SECURE_NO_WARNINGS


#if 0

The Nefarious File System
A system that controls the files and directories is going to be generated.
Refer to the following API descriptions and implement each function.

Void init()
All files and directories excluding the root directory are deleted and initialized.
Called once at the beginning of each test case.


Int add( int id, int pid, int fileSize)
File or directory corresponding to id is newly added to the pid directory.
The id value is not overlapping with another id.
Parameters
Id: id of file or directory that is newly added.
Pid: id of parent directory.
fileSize: size of the file that is newly added.
‘0’ refers to the directory. If greater than ‘0’, it refers to a file.
Returns
After file or directory is added, return size of corresponding pid.
(the total size of subordinate directories and files)



Init move(init id, init pid)
File or directory corresponding to id is moved below the pid directory.
In the move command, a pid that is the subordinate directory of the id will not be given.
Parameters
Id: id of file or directory that is to be moved.
Pid: id of becoming parent directory.
Returns
After file or directory is moved, return size of corresponding pid.
(the total size of subordinate directories and files)


Int infect (int id)
File or directory corresponding to id is infected.
In case of directory, the virus does not damage the directory itself,
But the subordinated files are infected only.
An infected file increases its size as much as the following formula.
Increasing file size:
Total file size within the system/ total number of files (however, the rest in thrown away)
An infected file can be infected several times.
Whenever it gets re-infected, file size increases additionally.
However, if the total number of files is 0, no infection takes place and return 0.
Parameters
Id: id of file or directory that is to be infected
Returns
After file or directory is infected, return size of corresponding id.
In case file, return the size of file itself.
In case of directory, return the total size of subordinate directories and files.



Int recover(int id)
File or directory corresponding to id that is infected gets recovered.
The recovered files are converted to their original size.
Files that had been infected several times get recovered at once and get their original size back.
Parameters
Id: id of file or directory that is to be recovered.
Returns
After file or directory is recovered, return size of corresponding id.
In case of file, return the size of file itself.
In case of directory, return the total size of subordinate directories and files.


Int remove(int id)
File or directory corresponding to id is removed.
If the id is the root directory, root directory does not get removed itself.
But, all subordinate files and directories of the root get removed.
Parameters
Id: id of file or directory that is to be removed.
Returns
Return size of corresponding id that is to be removed.
In case of file, return the size of file itself.
In case of directory, return the total size of subordinate directories and files.

The following shows an example of each API operation:

[init] After first init() execution, only the root directory exists in the system and the id of the root directory is always 10000.

[add]
cmd id pid fileSize Return
1 30001 10003 0 0
1 40001 30001 240 240
The picture above demonstrates the generation of directory (30001) and file (40001) which size is 240.
The return value of the first command is 0, the size of pid directory (10003).
The return value of the second command is 240, the size of pid directory (30001).

[MOVE]
cmd id pid Return
2 20002 30001 340
2 10001 10003 540
The picture above demonstrates the movement of file (20002) and directory (10001).
The return value of the first command is 340, the size of pid directory (30001).
The return value of the second command is 540, the size of pid directory (10003).


[Infect]
cmd id Return
3 10002 510
3 10003 1326
The pictures above show the virus infection of file (10002) and directory (10003).
In case of the first command, 200(=840/4) must be added to the size of infected files.
The return value is 510, the size of id file (10002).
In case of the second command, 262(=1050/4) must be added to the size of infected files.
The return value is 1326, the size of id directory (10003).

[Recover]
cmd id Return
4 10001 200
4 20002 100
The pictures above show the recovery of directory (10001) and file (20001).
The return value of the first command is 200, the size of id directory (10001).
The return value of the second command is 100, the size of id file (20002).

[Remove]
cmd id Return
5 10002 510
5 10001 200
The pictures above show the deletion of file (10002) and directory (10001).
The return value of the first command is 510, the size of id file (10002).
The return value of the second command is 200, the size of id directory (10001).



[Constraints]
1. 5<=N (the number of commands) <=10,000 is guaranteed.
2. 0<=id, pid<=1,000,000,000 is guaranteed.
3. The id of the root directory is always 10,000.
4. The file size is given as an integer that is greater than or equal to 1 and less than or equal to 1,000.
5. The directory has no size and although the directory gets infected, the capacity does not increase.
6. There will never be a case given where the total file size exceeds the int scope.
7. The maximum depth of tree is less than or equal to 50, including the root directory.
[Input/Output]
Input/ output are processed in the provided main part code.
Thus, in the user code part, input/output are not separately processed.



#endif


#include <stdio.h>
#include <stdlib.h>
typedef enum
{
	CMD_ADD = 1,
	CMD_MOVE,
	CMD_INFECT,
	CMD_RECOVER,
	CMD_REMOVE
} COMMAND;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern void init(void);
extern int add(int id, int pid, int fileSize);
extern int move(int id, int pid);
extern int infect(int id);
extern int recover(int id);
extern int fremove(int id);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int run() {
	int N, score = 0; scanf("%d", &N);
	for (register int i = 0; i < N; ++i) 
	{
		int cmd, ret, id, pid, size; 
		scanf("%d", &cmd);
		switch (cmd) 
		{
		case CMD_ADD:
			scanf("%d%d%d", &id, &pid, &size);
			ret = add(id, pid, size);
			break;
		case CMD_MOVE:
			scanf("%d%d", &id, &pid);
			ret = move(id, pid);
			break;
		case CMD_INFECT:
			scanf("%d", &id);
			ret = infect(id);
			break;
		case CMD_RECOVER:
			scanf("%d", &id);
			ret = recover(id);
			break;
		case CMD_REMOVE:
			scanf("%d", &id);
			ret = fremove(id);
			break;
		}
		int checksum; scanf("%d", &checksum);
		if (ret == checksum) 
			++score;
		else
		{
			printf ("checksum %d ret %d\n", checksum, ret);
			exit (0);
		}
	}
	return score;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);
	//freopen("sample_output.txt", "w", stdout);

	int T;
	scanf("%d", &T);
	int TotalScore = 0;
	for (int tc = 1; tc <= T; tc++)
	{
		init();
		int score = run();
		printf("#%d %d\n", tc, score);
		TotalScore += score;
	}
	printf("TotalScore = %d\n", TotalScore);
	return 0;
}
