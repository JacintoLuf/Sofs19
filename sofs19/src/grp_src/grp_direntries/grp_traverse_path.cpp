#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "direntries.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>



namespace sofs19
{
    bool checkTraverse(SOInode ip, uint32_t u, uint32_t g){
        if ((ip->owner == u) && ((ip->mode & 0100) != 0100)){
            return false;
        }
        if ((ip->group == g) && ((ip->mode & 0010) != 0010)){
            return false;
        }
        if ((ip->group != g) && (ip->owner != u) && ((ip->mode & 0001) != 0001)){
            return false;
        }
        return true;
    }


    uint32_t grpTraversePath(char *path)
    {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);
        uint32_t u = getuid();
        uint32_t g = getgid();
        if (strcmp(&path,"/") == 0){
            throw SOException(EINVAL, __FUNCTION__);
        }
        if (strcmp(path,"/") == 0) {
            return 0;
        }
        char* dname = dirname(strdupa(path));
        char* bname = basename(strdupa(path));
        
        uint32_t bnode = grpTraversePath(dname);
        
        uint32_t ih = soOpenInode(bnode);
        SOInode* ip = soGetInodePointer(ih);

        if((ip->mode & S_IFDIR) != S_IFDIR){
            throw SOException(ENOTDIR, __FUNCTION__);
        }
        if (checkTraverse(bnode, u, g) == false){
            throw SOException(EACCES, __FUNCTION__);
        }

        uint32_t in = soGetDirEntry(ih,bname);
        soCloseInode(ih);
        return in;

        /* change the following line by your code */
        //return binTraversePath(path);
    }
};

