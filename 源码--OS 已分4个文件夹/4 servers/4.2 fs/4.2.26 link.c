++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/link.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
27000 /* This file handles the LINK and UNLINK system calls. It also deals with
27001 * deallocating the storage used by a file when the last UNLINK is done to a
27002 * file and the blocks must be returned to the free block pool.
27003 *
27004 * The entry points into this file are
27005 * do_link: perform the LINK system call
27006 * do_unlink: perform the UNLINK and RMDIR system calls
27007 * do_rename: perform the RENAME system call
27008 * truncate: release all the blocks associated with an inode
27009 */
1004 File: servers/fs/link.c MINIX SOURCE CODE
27010
27011 #include "fs.h"
27012 #include <sys/stat.h>
27013 #include <string.h>
27014 #include <minix/com.h>
27015 #include <minix/callnr.h>
27016 #include "buf.h"
27017 #include "file.h"
27018 #include "fproc.h"
27019 #include "inode.h"
27020 #include "param.h"
27021 #include "super.h"
27022
27023 #define SAME 1000
27024
27025 FORWARD _PROTOTYPE( int remove_dir, (struct inode *rldirp, struct inode *rip,
27026 char dir_name[NAME_MAX]) );
27027
27028 FORWARD _PROTOTYPE( int unlink_file, (struct inode *dirp, struct inode *rip,
27029 char file_name[NAME_MAX]) );
27030
27031 /*===========================================================================*
27032 * do_link *
27033 *===========================================================================*/
27034 PUBLIC int do_link()
27035 {
27036 /* Perform the link(name1, name2) system call. */
27037
27038 register struct inode *ip, *rip;
27039 register int r;
27040 char string[NAME_MAX];
27041 struct inode *new_ip;
27042
27043 /* See if ’name’ (file to be linked) exists. */
27044 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27045 if ( (rip = eat_path(user_path)) == NIL_INODE) return(err_code);
27046
27047 /* Check to see if the file has maximum number of links already. */
27048 r = OK;
27049 if (rip->i_nlinks >= (rip->i_sp->s_version == V1 ? CHAR_MAX : SHRT_MAX))
27050 r = EMLINK;
27051
27052 /* Only super_user may link to directories. */
27053 if (r == OK)
27054 if ( (rip->i_mode & I_TYPE) == I_DIRECTORY && !super_user) r = EPERM;
27055
27056 /* If error with ’name’, return the inode. */
27057 if (r != OK) {
27058 put_inode(rip);
27059 return(r);
27060 }
27061
27062 /* Does the final directory of ’name2’ exist? */
27063 if (fetch_name(m_in.name2, m_in.name2_length, M1) != OK) {
27064 put_inode(rip);
27065 return(err_code);
27066 }
27067 if ( (ip = last_dir(user_path, string)) == NIL_INODE) r = err_code;
27068
27069 /* If ’name2’ exists in full (even if no space) set ’r’ to error. */
MINIX SOURCE CODE File: servers/fs/link.c 1005
27070 if (r == OK) {
27071 if ( (new_ip = advance(ip, string)) == NIL_INODE) {
27072 r = err_code;
27073 if (r == ENOENT) r = OK;
27074 } else {
27075 put_inode(new_ip);
27076 r = EEXIST;
27077 }
27078 }
27079
27080 /* Check for links across devices. */
27081 if (r == OK)
27082 if (rip->i_dev != ip->i_dev) r = EXDEV;
27083
27084 /* Try to link. */
27085 if (r == OK)
27086 r = search_dir(ip, string, &rip->i_num, ENTER);
27087
27088 /* If success, register the linking. */
27089 if (r == OK) {
27090 rip->i_nlinks++;
27091 rip->i_update |= CTIME;
27092 rip->i_dirt = DIRTY;
27093 }
27094
27095 /* Done. Release both inodes. */
27096 put_inode(rip);
27097 put_inode(ip);
27098 return(r);
27099 }
27101 /*===========================================================================*
27102 * do_unlink *
27103 *===========================================================================*/
27104 PUBLIC int do_unlink()
27105 {
27106 /* Perform the unlink(name) or rmdir(name) system call. The code for these two
27107 * is almost the same. They differ only in some condition testing. Unlink()
27108 * may be used by the superuser to do dangerous things; rmdir() may not.
27109 */
27110
27111 register struct inode *rip;
27112 struct inode *rldirp;
27113 int r;
27114 char string[NAME_MAX];
27115
27116 /* Get the last directory in the path. */
27117 if (fetch_name(m_in.name, m_in.name_length, M3) != OK) return(err_code);
27118 if ( (rldirp = last_dir(user_path, string)) == NIL_INODE)
27119 return(err_code);
27120
27121 /* The last directory exists. Does the file also exist? */
27122 r = OK;
27123 if ( (rip = advance(rldirp, string)) == NIL_INODE) r = err_code;
27124
27125 /* If error, return inode. */
27126 if (r != OK) {
27127 put_inode(rldirp);
27128 return(r);
27129 }
1006 File: servers/fs/link.c MINIX SOURCE CODE
27130
27131 /* Do not remove a mount point. */
27132 if (rip->i_num == ROOT_INODE) {
27133 put_inode(rldirp);
27134 put_inode(rip);
27135 return(EBUSY);
27136 }
27137
27138 /* Now test if the call is allowed, separately for unlink() and rmdir(). */
27139 if (call_nr == UNLINK) {
27140 /* Only the su may unlink directories, but the su can unlink any dir.*/
27141 if ( (rip->i_mode & I_TYPE) == I_DIRECTORY && !super_user) r = EPERM;
27142
27143 /* Don’t unlink a file if it is the root of a mounted file system. */
27144 if (rip->i_num == ROOT_INODE) r = EBUSY;
27145
27146 /* Actually try to unlink the file; fails if parent is mode 0 etc. */
27147 if (r == OK) r = unlink_file(rldirp, rip, string);
27148
27149 } else {
27150 r = remove_dir(rldirp, rip, string); /* call is RMDIR */
27151 }
27152
27153 /* If unlink was possible, it has been done, otherwise it has not. */
27154 put_inode(rip);
27155 put_inode(rldirp);
27156 return(r);
27157 }
27159 /*===========================================================================*
27160 * do_rename *
27161 *===========================================================================*/
27162 PUBLIC int do_rename()
27163 {
27164 /* Perform the rename(name1, name2) system call. */
27165
27166 struct inode *old_dirp, *old_ip; /* ptrs to old dir, file inodes */
27167 struct inode *new_dirp, *new_ip; /* ptrs to new dir, file inodes */
27168 struct inode *new_superdirp, *next_new_superdirp;
27169 int r = OK; /* error flag; initially no error */
27170 int odir, ndir; /* TRUE iff {old|new} file is dir */
27171 int same_pdir; /* TRUE iff parent dirs are the same */
27172 char old_name[NAME_MAX], new_name[NAME_MAX];
27173 ino_t numb;
27174 int r1;
27175
27176 /* See if ’name1’ (existing file) exists. Get dir and file inodes. */
27177 if (fetch_name(m_in.name1, m_in.name1_length, M1) != OK) return(err_code);
27178 if ( (old_dirp = last_dir(user_path, old_name))==NIL_INODE) return(err_code);
27179
27180 if ( (old_ip = advance(old_dirp, old_name)) == NIL_INODE) r = err_code;
27181
27182 /* See if ’name2’ (new name) exists. Get dir and file inodes. */
27183 if (fetch_name(m_in.name2, m_in.name2_length, M1) != OK) r = err_code;
27184 if ( (new_dirp = last_dir(user_path, new_name)) == NIL_INODE) r = err_code;
27185 new_ip = advance(new_dirp, new_name); /* not required to exist */
27186
27187 if (old_ip != NIL_INODE)
27188 odir = ((old_ip->i_mode & I_TYPE) == I_DIRECTORY); /* TRUE iff dir */
27189
MINIX SOURCE CODE File: servers/fs/link.c 1007
27190 /* If it is ok, check for a variety of possible errors. */
27191 if (r == OK) {
27192 same_pdir = (old_dirp == new_dirp);
27193
27194 /* The old inode must not be a superdirectory of the new last dir. */
27195 if (odir && !same_pdir) {
27196 dup_inode(new_superdirp = new_dirp);
27197 while (TRUE) { /* may hang in a file system loop */
27198 if (new_superdirp == old_ip) {
27199 r = EINVAL;
27200 break;
27201 }
27202 next_new_superdirp = advance(new_superdirp, dot2);
27203 put_inode(new_superdirp);
27204 if (next_new_superdirp == new_superdirp)
27205 break; /* back at system root directory */
27206 new_superdirp = next_new_superdirp;
27207 if (new_superdirp == NIL_INODE) {
27208 /* Missing ".." entry. Assume the worst. */
27209 r = EINVAL;
27210 break;
27211 }
27212 }
27213 put_inode(new_superdirp);
27214 }
27215
27216 /* The old or new name must not be . or .. */
27217 if (strcmp(old_name, ".")==0 || strcmp(old_name, "..")==0 ||
27218 strcmp(new_name, ".")==0 || strcmp(new_name, "..")==0) r = EINVAL;
27219
27220 /* Both parent directories must be on the same device. */
27221 if (old_dirp->i_dev != new_dirp->i_dev) r = EXDEV;
27222
27223 /* Parent dirs must be writable, searchable and on a writable device */
27224 if ((r1 = forbidden(old_dirp, W_BIT | X_BIT)) != OK ||
27225 (r1 = forbidden(new_dirp, W_BIT | X_BIT)) != OK) r = r1;
27226
27227 /* Some tests apply only if the new path exists. */
27228 if (new_ip == NIL_INODE) {
27229 /* don’t rename a file with a file system mounted on it. */
27230 if (old_ip->i_dev != old_dirp->i_dev) r = EXDEV;
27231 if (odir && new_dirp->i_nlinks >=
27232 (new_dirp->i_sp->s_version == V1 ? CHAR_MAX : SHRT_MAX) &&
27233 !same_pdir && r == OK) r = EMLINK;
27234 } else {
27235 if (old_ip == new_ip) r = SAME; /* old=new */
27236
27237 /* has the old file or new file a file system mounted on it? */
27238 if (old_ip->i_dev != new_ip->i_dev) r = EXDEV;
27239
27240 ndir = ((new_ip->i_mode & I_TYPE) == I_DIRECTORY); /* dir ? */
27241 if (odir == TRUE && ndir == FALSE) r = ENOTDIR;
27242 if (odir == FALSE && ndir == TRUE) r = EISDIR;
27243 }
27244 }
27245
27246 /* If a process has another root directory than the system root, we might
27247 * "accidently" be moving it’s working directory to a place where it’s
27248 * root directory isn’t a super directory of it anymore. This can make
27249 * the function chroot useless. If chroot will be used often we should
1008 File: servers/fs/link.c MINIX SOURCE CODE
27250 * probably check for it here.
27251 */
27252
27253 /* The rename will probably work. Only two things can go wrong now:
27254 * 1. being unable to remove the new file. (when new file already exists)
27255 * 2. being unable to make the new directory entry. (new file doesn’t exists)
27256 * [directory has to grow by one block and cannot because the disk
27257 * is completely full].
27258 */
27259 if (r == OK) {
27260 if (new_ip != NIL_INODE) {
27261 /* There is already an entry for ’new’. Try to remove it. */
27262 if (odir)
27263 r = remove_dir(new_dirp, new_ip, new_name);
27264 else
27265 r = unlink_file(new_dirp, new_ip, new_name);
27266 }
27267 /* if r is OK, the rename will succeed, while there is now an
27268 * unused entry in the new parent directory.
27269 */
27270 }
27271
27272 if (r == OK) {
27273 /* If the new name will be in the same parent directory as the old one,
27274 * first remove the old name to free an entry for the new name,
27275 * otherwise first try to create the new name entry to make sure
27276 * the rename will succeed.
27277 */
27278 numb = old_ip->i_num; /* inode number of old file */
27279
27280 if (same_pdir) {
27281 r = search_dir(old_dirp, old_name, (ino_t *) 0, DELETE);
27282 /* shouldn’t go wrong. */
27283 if (r==OK) (void) search_dir(old_dirp, new_name, &numb, ENTER);
27284 } else {
27285 r = search_dir(new_dirp, new_name, &numb, ENTER);
27286 if (r == OK)
27287 (void) search_dir(old_dirp, old_name, (ino_t *) 0, DELETE);
27288 }
27289 }
27290 /* If r is OK, the ctime and mtime of old_dirp and new_dirp have been marked
27291 * for update in search_dir.
27292 */
27293
27294 if (r == OK && odir && !same_pdir) {
27295 /* Update the .. entry in the directory (still points to old_dirp). */
27296 numb = new_dirp->i_num;
27297 (void) unlink_file(old_ip, NIL_INODE, dot2);
27298 if (search_dir(old_ip, dot2, &numb, ENTER) == OK) {
27299 /* New link created. */
27300 new_dirp->i_nlinks++;
27301 new_dirp->i_dirt = DIRTY;
27302 }
27303 }
27304
27305 /* Release the inodes. */
27306 put_inode(old_dirp);
27307 put_inode(old_ip);
27308 put_inode(new_dirp);
27309 put_inode(new_ip);
MINIX SOURCE CODE File: servers/fs/link.c 1009
27310 return(r == SAME ? OK : r);
27311 }
27313 /*===========================================================================*
27314 * truncate *
27315 *===========================================================================*/
27316 PUBLIC void truncate(rip)
27317 register struct inode *rip; /* pointer to inode to be truncated */
27318 {
27319 /* Remove all the zones from the inode ’rip’ and mark it dirty. */
27320
27321 register block_t b;
27322 zone_t z, zone_size, z1;
27323 off_t position;
27324 int i, scale, file_type, waspipe, single, nr_indirects;
27325 struct buf *bp;
27326 dev_t dev;
27327
27328 file_type = rip->i_mode & I_TYPE; /* check to see if file is special */
27329 if (file_type == I_CHAR_SPECIAL || file_type == I_BLOCK_SPECIAL) return;
27330 dev = rip->i_dev; /* device on which inode resides */
27331 scale = rip->i_sp->s_log_zone_size;
27332 zone_size = (zone_t) rip->i_sp->s_block_size << scale;
27333 nr_indirects = rip->i_nindirs;
27334
27335 /* Pipes can shrink, so adjust size to make sure all zones are removed. */
27336 waspipe = rip->i_pipe == I_PIPE; /* TRUE is this was a pipe */
27337 if (waspipe) rip->i_size = PIPE_SIZE(rip->i_sp->s_block_size);
27338
27339 /* Step through the file a zone at a time, finding and freeing the zones. */
27340 for (position = 0; position < rip->i_size; position += zone_size) {
27341 if ( (b = read_map(rip, position)) != NO_BLOCK) {
27342 z = (zone_t) b >> scale;
27343 free_zone(dev, z);
27344 }
27345 }
27346
27347 /* All the data zones have been freed. Now free the indirect zones. */
27348 rip->i_dirt = DIRTY;
27349 if (waspipe) {
27350 wipe_inode(rip); /* clear out inode for pipes */
27351 return; /* indirect slots contain file positions */
27352 }
27353 single = rip->i_ndzones;
27354 free_zone(dev, rip->i_zone[single]); /* single indirect zone */
27355 if ( (z = rip->i_zone[single+1]) != NO_ZONE) {
27356 /* Free all the single indirect zones pointed to by the double. */
27357 b = (block_t) z << scale;
27358 bp = get_block(dev, b, NORMAL); /* get double indirect zone */
27359 for (i = 0; i < nr_indirects; i++) {
27360 z1 = rd_indir(bp, i);
27361 free_zone(dev, z1);
27362 }
27363
27364 /* Now free the double indirect zone itself. */
27365 put_block(bp, INDIRECT_BLOCK);
27366 free_zone(dev, z);
27367 }
27368
27369 /* Leave zone numbers for de(1) to recover file after an unlink(2). */
1010 File: servers/fs/link.c MINIX SOURCE CODE
27370 }
27372 /*===========================================================================*
27373 * remove_dir *
27374 *===========================================================================*/
27375 PRIVATE int remove_dir(rldirp, rip, dir_name)
27376 struct inode *rldirp; /* parent directory */
27377 struct inode *rip; /* directory to be removed */
27378 char dir_name[NAME_MAX]; /* name of directory to be removed */
27379 {
27380 /* A directory file has to be removed. Five conditions have to met:
27381 * - The file must be a directory
27382 * - The directory must be empty (except for . and ..)
27383 * - The final component of the path must not be . or ..
27384 * - The directory must not be the root of a mounted file system
27385 * - The directory must not be anybody’s root/working directory
27386 */
27387
27388 int r;
27389 register struct fproc *rfp;
27390
27391 /* search_dir checks that rip is a directory too. */
27392 if ((r = search_dir(rip, "", (ino_t *) 0, IS_EMPTY)) != OK) return r;
27393
27394 if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0)return(EINVAL);
27395 if (rip->i_num == ROOT_INODE) return(EBUSY); /* can’t remove ’root’ */
27396
27397 for (rfp = &fproc[INIT_PROC_NR + 1]; rfp < &fproc[NR_PROCS]; rfp++)
27398 if (rfp->fp_workdir == rip || rfp->fp_rootdir == rip) return(EBUSY);
27399 /* can’t remove anybody’s working dir */
27400
27401 /* Actually try to unlink the file; fails if parent is mode 0 etc. */
27402 if ((r = unlink_file(rldirp, rip, dir_name)) != OK) return r;
27403
27404 /* Unlink . and .. from the dir. The super user can link and unlink any dir,
27405 * so don’t make too many assumptions about them.
27406 */
27407 (void) unlink_file(rip, NIL_INODE, dot1);
27408 (void) unlink_file(rip, NIL_INODE, dot2);
27409 return(OK);
27410 }
27412 /*===========================================================================*
27413 * unlink_file *
27414 *===========================================================================*/
27415 PRIVATE int unlink_file(dirp, rip, file_name)
27416 struct inode *dirp; /* parent directory of file */
27417 struct inode *rip; /* inode of file, may be NIL_INODE too. */
27418 char file_name[NAME_MAX]; /* name of file to be removed */
27419 {
27420 /* Unlink ’file_name’; rip must be the inode of ’file_name’ or NIL_INODE. */
27421
27422 ino_t numb; /* inode number */
27423 int r;
27424
27425 /* If rip is not NIL_INODE, it is used to get faster access to the inode. */
27426 if (rip == NIL_INODE) {
27427 /* Search for file in directory and try to get its inode. */
27428 err_code = search_dir(dirp, file_name, &numb, LOOK_UP);
27429 if (err_code == OK) rip = get_inode(dirp->i_dev, (int) numb);
MINIX SOURCE CODE File: servers/fs/link.c 1011
27430 if (err_code != OK || rip == NIL_INODE) return(err_code);
27431 } else {
27432 dup_inode(rip); /* inode will be returned with put_inode */
27433 }
27434
27435 r = search_dir(dirp, file_name, (ino_t *) 0, DELETE);
27436
27437 if (r == OK) {
27438 rip->i_nlinks--; /* entry deleted from parent’s dir */
27439 rip->i_update |= CTIME;
27440 rip->i_dirt = DIRTY;
27441 }
27442
27443 put_inode(rip);
27444 return(r);
27445 }