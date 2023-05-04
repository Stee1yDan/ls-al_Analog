#include <iostream>
#include <sys/stat.h>
#include <sys/dir.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

void listFiles(const char* dirname) 
{
    DIR* dir = opendir(dirname);

    if (dir == NULL) 
    {
        return;
    }

    struct dirent* entity;

    struct group* gr_p;
    struct passwd* pw_d;

    entity = readdir(dir);

    while (entity != NULL) 
    {

        if (strcmp(entity -> d_name, "."))
        {
            if (strcmp(entity -> d_name, ".."))
            {
                    struct stat status;

                    int mode = status.st_mode % 512;

                    int p = 256;

                    char accessRule[10] = { 0 };

                    for (int i = 0; i < 3; i++)
                    {
                        p >>= 1;
                        strcat(accessRule,(mode&p) ? "r" : "-");

                        p >>= 1;
                        strcat(accessRule,(mode&p) ? "w" : "-");

                        p >>= 1;
                        strcat(accessRule,(mode&p) ? "x" : "-");
                    }


                    if (stat(entity -> d_name, &status))
                    {
                        exit(2);
                    }

                    stat(entity -> d_name,&status);

                    gr_p = getgrgid(status.st_gid);
                    pw_d = getpwuid(status.st_uid);

                    cout
                        << accessRule << "\t" 
                        << gr_p -> gr_name << "\t" 
                        << pw_d -> pw_name << "\t"  
                        << status.st_size << "\t" 
                        << ctime(&status.st_mtime) << "\t" 
                        << entity->d_name << endl;
            }       
        }

        entity = readdir(dir);
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {

    cout << "Reading files" << endl;

    if (argc == 1)
        listFiles(".");
    else
        listFiles(argv[1]);

    return 0;
}