#include <cstring>
#include <iostream>

#define PRETEST_PRINT(test_name) std::cout << "Begin test: " << test_name << std::endl
#define POSTTEST_PRINT(test_name) std::cout << "Succeeded: " << test_name << std::endl
#define TEST_FAILED(test_name, reason)                                           \
    std::cout << "Failed: " << test_name << ", reason: " << reason << std::endl; \
    return -1
#define START_ACTION_PRINT(action) std::cout << "Begin action: " << action << std::endl
#define STOP_ACTION_PRINT(action) std::cout << "Stop action: " << action << std::endl
#define CREATE_FILE_PATH(mnt_path, file_name) (mnt_path + file_name)
#define PRINT_ERRNO_ERROR(er) std::cerr << std::strerror(er) << std::endl

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cout << "Usage: driver <path_to_mnt>" << std::endl;
        return -1;
    }

    std::cout << "Hello driver!" << std::endl << std::endl;

    std::string mnt_path = std::string(argv[1]).append("/");
    std::cout << "mnt_path: " << mnt_path << std::endl;

    {
        std::string test_name("file open/close");
        PRETEST_PRINT(test_name);

        std::string fp_name = CREATE_FILE_PATH(mnt_path, std::string("test"));
        std::cout << "abs file path: " << fp_name << std::endl;
        START_ACTION_PRINT("fopen(test, w+)");
        FILE* fp = fopen(CREATE_FILE_PATH(mnt_path, std::string("test")).c_str(), "w+");

        if(!fp) {
            PRINT_ERRNO_ERROR(errno);
            TEST_FAILED(test_name, "fp found null after fopen");
        }

        STOP_ACTION_PRINT("fopen(test, w+");

        START_ACTION_PRINT("fclose(fp)");
        fclose(fp);
        STOP_ACTION_PRINT("fclose(fp)");

        POSTTEST_PRINT(test_name);
    }
}
