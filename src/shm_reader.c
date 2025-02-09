#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define KEY_PARSE_FAILURE 2
#define SHM_GET_FAILURE 3
#define SHM_ATTACH_FAILURE 4
#define SHM_DETACH_FAILURE 5
#define SHM_FREE_FAILURE 6

#define SHM_SIZE 1000
#define ACCESS_PERMISSIONS 0666

key_t parse_key(const char *arg) {
	char *endptr;
	key_t key = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || key == 0) {
		fprintf(stderr, "Invalid key: %s\n", arg);
		printf("(exit code %d)\n", KEY_PARSE_FAILURE);
		exit(KEY_PARSE_FAILURE);
	}
	return key;
}

int get_shared_memory(key_t key) {
	int shmid = shmget(key, SHM_SIZE, ACCESS_PERMISSIONS);
	if (shmid == -1) {
		perror("shmget failed");
		printf("(exit code %d)\n", SHM_GET_FAILURE);
		exit(SHM_GET_FAILURE);
	}
	return shmid;
}

char* attach_shared_memory(int shmid) {
	char *shm_ptr = shmat(shmid, NULL, 0);
	if (shm_ptr == (char *)-1) {
		perror("shmat failed");
		printf("(exit code %d)\n", SHM_ATTACH_FAILURE);
		exit(SHM_ATTACH_FAILURE);
	}
	return shm_ptr;
}

void detach_shared_memory(char *shm_ptr) {
	if (shmdt(shm_ptr) == -1) {
		perror("shmdt failed");
		printf("(exit code %d)\n", SHM_DETACH_FAILURE);
		exit(SHM_DETACH_FAILURE);
	}
}

void free_shared_memory(int shmid) {
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		printf("(exit code %d)\n", SHM_FREE_FAILURE);
		exit(SHM_FREE_FAILURE);
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <key>\n", argv[0]);
		printf("(exit code %d)\n", EXIT_FAILURE);
		return EXIT_FAILURE;
	}

	key_t key = parse_key(argv[1]);
	int shmid = get_shared_memory(key);
	char *shm_ptr = attach_shared_memory(shmid);
	
	printf("Shared Memory Content: %s\n", shm_ptr);
	
	detach_shared_memory(shm_ptr);
	free_shared_memory(shmid);

	return EXIT_SUCCESS;
}