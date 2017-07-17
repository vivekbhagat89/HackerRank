#include <stdio.h>
#include <stdlib.h>

#define MAX_CHILD 500
#define MAX_NODES 100000
#define D_DIR 1
#define D_FILE 0


#define DEBUG 
typedef enum
{
	CMD_ADD = 1,
	CMD_MOVE,
	CMD_INFECT,
	CMD_RECOVER,
	CMD_REMOVE
} COMMAND;


typedef struct node
{
	int id;
	int pid;
	int cid[MAX_CHILD];
	int cid_count;

	int file_or_dir;
	int is_infected;
	int size;
	int orig_size;
}node;

int count;
int filecount;
int size;

node list[MAX_NODES];

void init(void)
{
	count = 0;
	size = 0;
	filecount = 0;

	list[count].id = 10000;
	list[count].cid_count = 0;
	list[count].file_or_dir = D_DIR;
	list[count].is_infected = 0;
	list[count].pid = -1;

	count++;
}
	
int find (int id)
{
	int i=0;

	while (list[i].id != id && i < count)
		i++;

	return i;
}

int calculate (int id)
{
	node n = list[find(id)];
	int nodesize = 0;

	if (n.file_or_dir == D_FILE)
		nodesize = n.size;

	for (int i=0; i < n.cid_count; i++)
	{
		node child = list[find (n.cid[i])];

		if (child.file_or_dir == D_DIR && child.cid_count)
			nodesize += calculate (child.id);
		else
			nodesize += child.size;
	}
	
	return nodesize;
}

void dump(int id, int depth)
{
	return ;//INTENTIONAL
	int i;
	node n = list[find (id)];
	printf ("pid %d, count %d = \t ", n.id, n.cid_count);
	for (i=0; i < n.cid_count; i++)
	{
		node child = list[find (n.cid[i])];
		printf ("(%d, %d)\t", child.id, child.size);
	}
	printf ("\n");
	for (i=0; i < n.cid_count; i++)
	{
		node child = list[find (n.cid[i])];
		if (child.file_or_dir == D_DIR && child.cid_count)
			dump (child.id, depth+1);
	}

	printf ("\n");
}



int add(int id, int pid, int fileSize)
{

//	printf (" ADD pid %d id %d size %d\n", pid, id, fileSize);
	node* parent = &list[find(pid)];
	node* child = &list[count++];
	size += fileSize;

	parent->cid[parent->cid_count++] = id;

	child->id = id;
	child->pid = pid;
	child->size = fileSize;
	child->orig_size = fileSize;
	child->cid_count = 0;

	child->file_or_dir = !fileSize;
	if (child->file_or_dir == D_FILE)
		filecount++;
	child->is_infected = 0;

	dump(10000, 0);

	return calculate (pid);
}

int move(int id, int pid)
{
	//printf (" Move new pid %d id %d \n", pid, id);
	int i;

	node* child = &list[find(id)];
	node* new_parent = &list[find(pid)];
	node* old_parent = &list[find(child->pid)];

	//remove from old parent list
	for (i=0; i < old_parent->cid_count; i++)
	{
		if (old_parent->cid[i] == id)
			break;
	}

	if (i+1 != old_parent->cid_count) //not last child
		old_parent->cid[i] = old_parent->cid[old_parent->cid_count - 1];
	old_parent->cid_count--;

	//Add in new parent list
	new_parent->cid[new_parent->cid_count++] = id;
	new_parent = &list[find(pid)];


	//update parent
	child->pid = pid;

	dump(10000, 0);
	return calculate (pid);
}
int infected;
void rec_infect(int id)
{
	//printf (" Rec Infect %d \n", id);
	node* n = &list[find(id)];

	if (n->file_or_dir == D_FILE)
	{
		n->is_infected = 1;
		n->size += size/filecount;
		infected += size/filecount;
		return;
	}
	else
	{
		for (int i=0; i < n->cid_count; i++)
		{
			node* child = &list[find (n->cid[i])];

			child->is_infected = 1;

			if (child->file_or_dir == D_DIR)
				rec_infect (child->id);
			else
			{
				child->size += size/filecount;
				infected += size/filecount;
			}
		}
		return; 
	}
}

int infect(int id)
{
	if (filecount)
	{
	//	printf (" Infect %d size %d\n", id, size/filecount);
		infected = 0;
		rec_infect(id);
		size += infected;
		dump(10000, 0);
	}
	return calculate (id); 
}

void rec_recover(int id)
{
	//printf ("Rec Recover %d \n", id);
	node* n = &list[find(id)];

	if (n->file_or_dir == D_FILE)
	{
		n->is_infected = 0;
		infected += n->size - n->orig_size;
		n->size = n->orig_size;
		return;
	}
	else
	{
		for (int i=0; i < n->cid_count; i++)
		{
			node* child = &list[find (n->cid[i])];

			child->is_infected = 0;

			if (child->file_or_dir == D_DIR)
				rec_recover (child->id);
			else
			{
				infected += child->size - child->orig_size;
				child->size = child->orig_size;
			}
		}
		return; 
	}
}

int recover(int id)
{
//	printf (" Recover %d \n", id);
	infected = 0;
	rec_recover (id);
	dump (10000, 0);
	size -= infected;
	return calculate (id);
}

void rec_remove(int id)
{
//	printf ("Rec Remove %d \n", id);
	node* n = &list[find(id)];
	n->id = -1;

	if (n->file_or_dir == D_FILE)
	{
		infected += n->size;
		filecount--;
		return;
	}
	else
	{
		for (int i=0; i < n->cid_count; i++)
		{
			node* child = &list[find (n->cid[i])];

			child->is_infected = 0;

			if (child->file_or_dir == D_DIR)
			{
				rec_remove (child->id);
			}
			else
			{
				infected += child->size;
				filecount--;
			}
			child->id = -1;
		}
		n->cid_count = 0;
		return; 
	}
}


int fremove(int id)
{
//	printf (" Remove %d \n", id);
	int i, ret;
	node* child = &list[find(id)];
	node* parent = &list[find(child->pid)];

	ret = calculate (id);

	//remove from old parent list
	for (i=0; i < parent->cid_count; i++)
	{
		if (parent->cid[i] == id)
			break;
	}

	if (i+1 != parent->cid_count) //not last child
		parent->cid[i] = parent->cid[parent->cid_count - 1];
	parent->cid_count--;
	if (child->file_or_dir == D_FILE)
	{
		filecount--;
		size -= child->size;
	}
	else
	{
		infected = 0;
		rec_remove (id);
		size -= infected;
	}

	child->id = -1;

	dump(10000, 0);
	return ret;
}

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
		int checksum; 
		scanf("%d", &checksum);
		if (ret == checksum) 
			++score;
		else
		{
			printf ("FAIL expected %d got %d\n", checksum, ret);
			exit (0);
			return 0;
		}
	}
	return score;
}

int main()
{
	//setbuf(stdout, NULL);
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