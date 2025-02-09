#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define KEY_PARSE_FAILURE 10
#define SHM_GET_FAILURE 11
#define SHM_ATTACH_FAILURE 12
#define SHM_DETACH_FAILURE 13
#define SHM_FREE_FAILURE 14

#define SHM_SIZE 1000
#define ACCESS_PERMISSIONS 0666

key_t parse_key(const char *arg) {
	char *endptr;
	key_t key = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || key == 0) {
		fprintf(stderr, "Invalid key: %s\n", arg);
		exit(KEY_PARSE_FAILURE);
	}
	return key;
}

int get_shared_memory(key_t key) {
	int shmid = shmget(key, SHM_SIZE, ACCESS_PERMISSIONS);
	if (shmid == -1) {
		perror("shmget failed");
		exit(SHM_GET_FAILURE);
	}
	return shmid;
}

char* attach_shared_memory(int shmid) {
	char *shm_ptr = shmat(shmid, NULL, 0);
	if (shm_ptr == (char *)-1) {
		perror("shmat failed");
		exit(SHM_ATTACH_FAILURE);
	}
	return shm_ptr;
}

void detach_shared_memory(char *shm_ptr) {
	if (shmdt(shm_ptr) == -1) {
		perror("shmdt failed");
		exit(SHM_DETACH_FAILURE);
	}
}

void free_shared_memory(int shmid) {
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl failed");
		exit(SHM_FREE_FAILURE);
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <key>\n", argv[0]);
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