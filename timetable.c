#include <stdio.h>
#define BUFF 4097

// checks string equality
int is_equals(const char *str1, const char *str2) { 
    while(*str1 != '\0' || *str2 != '\0') {
        if (*str1 != *str2) {
            return -1;
        }
        str1++;
        str2++;
    }
    return 0;
}
// changes time in (hh, mm, ss) format to seconds
int to_sec(int *time) { 
    return time[0]*60*60 + time[1]*60 + time[2];
}
// if entry is best time in comparison to previous best, sets it as best time
int set_best_time(int *og, int *best, int *entry) {
    if (entry[0] > 23 || entry[1] > 59 || entry[2] > 59) {
        return 0;
    } else if (to_sec(og) <= to_sec(entry) && to_sec (entry) <= to_sec(best)) {
        for (int i = 0; i < 3; i++) {
            best[i] = entry[i];
        }
        return 1;
    } 
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Please provide <source> <destination> <time> as command line arguments\n");
        return 0;
    }

    char *og_src = argv[1];
    char *og_dest = argv[2];
    int og_time[3] = {0, 0, 0};

    char empty[BUFF];
    char entry[BUFF] = {"::"};

    int best[3] = {23, 59, 59};
    int early[3] = {23, 59, 59};
    int epoch[3] = {0, 0, 0};

    int counter = 0;
    int early_count = 0;

    // if argument given is not in form %d:%d:%d or if time is not valid, return
    int cmd = sscanf(argv[3], "%d:%d:%d%s", &og_time[0], &og_time[1], &og_time[2], empty);
    if (cmd != 3 || og_time[0] > 23 || og_time[1] > 59 || og_time[2] > 59) {
        printf("No suitable trains can be found\n");
        return 0;
    }

    while (fgets(entry, BUFF, stdin) != NULL) {
        if (entry[sizeof(entry) - 1] != '\0') { // if line too long, skips
            continue;
        }
        char src[BUFF];
        char dest[BUFF];
        int time[3];
        // takes entry in from fgets, if not in form:
        //<source>::<destination>::hh:mm:ss, skips value
        if (sscanf(entry, "%[^:]::%[^:]::%d:%d:%d%s", src, dest, &time[0],
                            &time[1], &time[2], empty) != 5) {
            continue;
        }

        // if sources and destination match, set if entry is the best time
        if (is_equals(src, og_src) == 0 && is_equals(dest, og_dest) == 0) {
            // if sets best time, increment counter
            early_count += set_best_time(epoch, early, time);
            counter += set_best_time(og_time, best, time);
        }
    }
    // if there are no times for today, gives earliest time for the next day
    if (counter == 0 && early_count != 0) {
        printf("The next train to %s from %s departs at %02d:%02d:%02d\n",
            og_dest, og_src, early[0], early[1], early[2]);

    } else if (counter != 0) { 
        // if best time has been set, give best time
        printf("The next train to %s from %s departs at %02d:%02d:%02d\n",
            og_dest, og_src, best[0], best[1], best[2]);

    } else if (is_equals(entry, "::") == 0) {
        // if default value unchanged
        fprintf(stderr, "No timetable to process\n");

    } else {
        printf("No suitable trains can be found\n");
    } 
    return 0;
}