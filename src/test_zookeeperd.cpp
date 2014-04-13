#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <zookeeper.h>
#include <zookeeper_log.h>

zhandle_t *zkhandle;

void QueryServerd_watcher_global(zhandle_t * zh, int type, int state,
        const char *path, void *watcherCtx);
static void QueryServerd_dump_stat(const struct Stat *stat);
void QueryServerd_stat_completion(int rc, const struct Stat *stat,
        const void *data);
void QueryServerd_watcher_awexists(zhandle_t *zh, int type, int state,
        const char *path, void *watcherCtx);
static void QueryServerd_awexists(zhandle_t *zh);

void QueryServerd_watcher_global(zhandle_t * zh, int type, int state,
        const char *path, void *watcherCtx) {
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            printf("Connected to zookeeper service successfully!\n");
        } else if (state == ZOO_EXPIRED_SESSION_STATE) { 
            printf("Zookeeper session expired!\n");
        }
    }
}

void zkConf_watcher_awget(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) {
    printf("type: %d, path: %s\n", type, path);
}

void data_completion(int rc, const char *value, int value_len, const struct Stat *stat, const void *data) {
    printf("rc: %d, value: %s, value_len: %d, data: %s\n", rc, value, value_len, (char *)data);
    zoo_awget(zkhandle, "/test", zkConf_watcher_awget, NULL, data_completion, NULL);
    sleep(1);
}

    static void
QueryServerd_dump_stat(const struct Stat *stat)
{
    char tctimes[40];
    char tmtimes[40];
    time_t tctime;
    time_t tmtime;

    if (!stat) {
        fprintf(stderr, "null\n");
        return;
    }
    tctime = stat->ctime / 1000;
    tmtime = stat->mtime / 1000;

    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);

    fprintf(stderr, "\tctime = %s\tczxid=%llx\n"
            "\tmtime=%s\tmzxid=%llx\n"
            "\tversion=%x\taversion=%x\n"
            "\tephemeralOwner = %llx\n",
            tctimes, stat->czxid,
            tmtimes, stat->mzxid,
            (unsigned int) stat->version, (unsigned int) stat->aversion,
            stat->ephemeralOwner);
}

    void
QueryServerd_stat_completion(int rc, const struct Stat *stat,
        const void *data)
{
    fprintf(stderr, "%s: rc = %d Stat:\n", (char *) data, rc);
    // QueryServerd_dump_stat(stat);
}

void QueryServerd_watcher_awexists(zhandle_t *zh, int type, int state,
        const char *path, void *watcherCtx)
{
    if (state == ZOO_CONNECTED_STATE) {
        if (type == ZOO_DELETED_EVENT) {
            printf("QueryServer gone away ...\n"); // re-exists and set watch on /QueryServer again.
            QueryServerd_awexists(zh);
            /*
            pid_t pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Error when doing fork.\n");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) { // child process
                // .. QueryServer ...
                execl("/tmp/QueryServer/QueryServer", "QueryServer", NULL);
                exit(EXIT_SUCCESS);
            }
            sleep(1); // sleep 1 second for purpose.
            */
            sleep(5);
        } else if (type == ZOO_CREATED_EVENT) {
            printf("QueryServer started...\n");
            QueryServerd_awexists(zh);
        }
    } else {
        // re-exists and set watch on /QueryServer again.
        QueryServerd_awexists(zh);
    }
}

    static void
QueryServerd_awexists(zhandle_t *zh)
{
    int ret =
        zoo_awexists(zh, "/QueryServer",
                QueryServerd_watcher_awexists,
                (void *)"QueryServerd_awexists.",
                QueryServerd_stat_completion,
                (void *)"zoo_awexists");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "aexists");
        exit(EXIT_FAILURE);
    } else {
        printf("QueryServerd_awexists...successful\n");
    }
}

    int
main(int argc, const char *argv[])
{
    const char *host = "192.168.69.111:2181";
    int timeout = 30000;

    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
    zkhandle = zookeeper_init(host,
            QueryServerd_watcher_global,
            timeout,
            0, (void *)"QueryServerd", 0);
    if (zkhandle == NULL) {
        fprintf(stderr, "Error when connecting to zookeeper servers...\n");
        exit(EXIT_FAILURE);
    }

    //QueryServerd_awexists(zkhandle);
    zoo_awget(zkhandle, "/test", zkConf_watcher_awget, NULL, data_completion, NULL);
    // Wait for asynchronous zookeeper call done.
    getchar();

    zookeeper_close(zkhandle);

    return 0;
}
