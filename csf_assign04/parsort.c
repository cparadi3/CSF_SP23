#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int compare_i64(const void *left, const void *right) {
  if(*(int64_t*)left > *(int64_t*)right) {
    return 1;
  }
  else if(*(int64_t*)left < *(int64_t*)right) {
    return -1;
  }
  return 0;
}


// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
  size_t numElements = end - begin;
  unsigned mid = begin + (numElements / 2);
  int wstatus;
  if (numElements <= threshold) {
    //TODO: sequential sort algorithm
    qsort(arr + begin, numElements, sizeof(int64_t), compare_i64);

  } else {
    pid_t pid1 = fork();
    //If fork fails
    if(pid1 < 0) {
      fprintf(stderr, "Child process failed to execute\n");
       exit(1);
    } else if (pid1 == 0) {
      merge_sort(arr, begin, mid, threshold);
      exit(0);
    }
    else {
      pid_t actual_pid = waitpid(pid1, &wstatus,0);
      if(actual_pid == -1) {
      fprintf(stderr, "Child process failed to execute\n");
      exit(1);
    }
    }
  
  pid_t pid2 = fork();
  if(pid2 < 0) {
    fprintf(stderr, "Child process failed to execute\n");
    exit(1);
  } else if (pid2 == 0) {
        merge_sort(arr, mid, end, threshold);
  } else {
    pid_t actual_pid = waitpid(pid2, &wstatus,0);
    if(actual_pid == -1) {
      fprintf(stderr, "Child process failed to execute\n");
      exit(1);
    }
  }

  if (pid1 > 0 && pid2 > 0) {
    int64_t *temp;
    temp = malloc(numElements * sizeof(int64_t));
    merge(arr, begin, mid, end, temp);
    memcpy(arr + begin, temp, numElements * sizeof(int64_t));
    free(temp);
  }
}
}
int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) {
    fprintf(stderr, "Threshold value is invalid\n");
    return 1;
  }
  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    fprintf(stderr, "Could not open file: %s\n", filename);
    return 1;
  }
  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // handle fstat error and exit
    fprintf(stderr, "fstat failed\n");
    return 1;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  
  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  if (data == MAP_FAILED) {
    // handle mmap error and exit
    fprintf(stderr, "Failed to mmap the file data\n");
    return 1;
  }
  // TODO: sort the data!
  size_t numItems = file_size_in_bytes / 8;
  merge_sort(data, 0, numItems, threshold);
  //memcpy(array, data, file_size_in_bytes);
  //qsort(data, numItems, 8, compare_i64);
  munmap(data, file_size_in_bytes);
  if (data == MAP_FAILED) {
    // handle mmap error and exit
    fprintf(stderr, "Failed to munmap the file data\n");
    return 1;
  }
  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}
