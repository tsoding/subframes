#define NOB_IMPLEMENTATION
#include "nob.h"

void cflags(Nob_Cmd *cmd)
{
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
    nob_cmd_append(cmd, "-I./raylib/raylib-5.0_linux_amd64/include");
    nob_cmd_append(cmd, "-I./src/");
}

void cc(Nob_Cmd *cmd)
{
    nob_cmd_append(cmd, "cc");
    cflags(cmd);
}

void libs(Nob_Cmd *cmd)
{
    nob_cmd_append(cmd, "-L./raylib/raylib-5.0_linux_amd64/lib");
    nob_cmd_append(cmd, "-l:libraylib.a", "-lm", "-ldl", "-lpthread");
}

bool build_exe(bool force, Nob_Cmd *cmd, const char **input_paths, size_t input_paths_len, const char *output_path)
{
    int rebuild_is_needed = nob_needs_rebuild(output_path, input_paths, input_paths_len);
    if (rebuild_is_needed < 0) return false;

    if (force || rebuild_is_needed) {
        cmd->count = 0;
        cc(cmd);
        nob_cmd_append(cmd, "-o", output_path);
        nob_da_append_many(cmd, input_paths, input_paths_len);
        libs(cmd);
        return nob_cmd_run_sync(*cmd);
    }

    nob_log(NOB_INFO, "%s is up-to-date", output_path);
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program_name = nob_shift_args(&argc, &argv);

    bool force = false;

    while (argc > 0) {
        const char *flag = nob_shift_args(&argc, &argv);
        if (strcmp(flag, "-f") == 0) {
            force = true;
        } else {
            nob_log(NOB_ERROR, "Unknown flag `%s`", flag);
            return 1;
        }
    }

    const char *input_paths[] = {
        "main.c"
    };

    Nob_Cmd cmd = {0};
    if (!build_exe(force, &cmd, input_paths, NOB_ARRAY_LEN(input_paths), "subframes")) return 1;

    return 0;
}
