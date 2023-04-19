#pragma once

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "user.h"

enum {
    WORK, SAVE, GET
};

static volatile sig_atomic_t mod = WORK;

static void handler_save(int n) {
    mod = SAVE;
}

static void handler_get(int n) {
    mod = GET;
}


static void prepare_for_work() {
    FILE *file = fopen("server_metadata.txt", "w");
    fprintf(file, "%d\n", getpid());
    fclose(file);
    struct sigaction sa_save = {};
    sa_save.sa_handler = handler_save;
    sa_save.sa_flags = 0;
    sigaction(SIGUSR1, &sa_save, NULL);

    struct sigaction sa_get = {};
    sa_get.sa_handler = handler_get;
    sa_get.sa_flags = 0;
    sigaction(SIGUSR2, &sa_get, NULL);
}

void work_in_save_mod(struct User *);

void work_in_get_mode(struct User *);

void recovery(struct User *);
