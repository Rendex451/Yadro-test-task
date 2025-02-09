#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define KEY_CREATE_FAILURE 2
#define SHM_ALLOCATE_FAILURE 3
#define SHM_ATTACH_FAILURE 4

#define SHM_SIZE 1000
#define INPUT_STRING "Hello world!"
#define ACCESS_PERMISSIONS 0666
#define KEY_PATH "/tmp"
#define KEY_PROJ_ID 'A'

key_t create_key(const char *path, char proj_id) {
	key_t key = ftok(path, proj_id);
	if (key == -1){
		perror("ftok failed");
		fprintf(stderr, "(exit code %d)\n", KEY_CREATE_FAILURE);
		exit(KEY_CREATE_FAILURE);
	}
	return key;
}

int allocate_shared_memory(key_t key, size_t size) {
	int shmid = shmget(key, size, IPC_CREAT | ACCESS_PERMISSIONS);
	if (shmid == -1) {
		perror("shmget failed");
		fprintf(stderr, "(exit code %d)\n", SHM_ALLOCATE_FAILURE);
		exit(SHM_ALLOCATE_FAILURE);
	}
	return shmid;
}

char *attach_shared_memory(int shmid) {
	char *shm_ptr = shmat(shmid, NULL, 0);
	if (shm_ptr == (char *)-1) {
		perror("shmat failed");
		fprintf(stderr, "(exit code %d)\n", SHM_ATTACH_FAILURE);
		exit(SHM_ATTACH_FAILURE);
	}
	return shm_ptr;
}

int main() {
	key_t key = create_key(KEY_PATH, KEY_PROJ_ID);
	int shmid = allocate_shared_memory(key, SHM_SIZE);
	char *shm_ptr = attach_shared_memory(shmid);

	strcpy(shm_ptr, INPUT_STRING);
	printf("Shared Memory Key: %d\n", key);

	return EXIT_SUCCESS;
}