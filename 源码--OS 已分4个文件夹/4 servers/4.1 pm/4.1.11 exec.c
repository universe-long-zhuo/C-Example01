++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/exec.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
18700 /* This file handles the EXEC system call. It performs the work as follows:
18701 * - see if the permissions allow the file to be executed
18702 * - read the header and extract the sizes
18703 * - fetch the initial args and environment from the user space
18704 * - allocate the memory for the new process
886 File: servers/pm/exec.c MINIX SOURCE CODE
18705 * - copy the initial stack from PM to the process
18706 * - read in the text and data segments and copy to the process
18707 * - take care of setuid and setgid bits
18708 * - fix up ’mproc’ table
18709 * - tell kernel about EXEC
18710 * - save offset to initial argc (for ps)
18711 *
18712 * The entry points into this file are:
18713 * do_exec: perform the EXEC system call
18714 * rw_seg: read or write a segment from or to a file
18715 * find_share: find a process whose text segment can be shared
18716 */
18717
18718 #include "pm.h"
18719 #include <sys/stat.h>
18720 #include <minix/callnr.h>
18721 #include <minix/com.h>
18722 #include <a.out.h>
18723 #include <signal.h>
18724 #include <string.h>
18725 #include "mproc.h"
18726 #include "param.h"
18727
18728 FORWARD _PROTOTYPE( int new_mem, (struct mproc *sh_mp, vir_bytes text_bytes,
18729 vir_bytes data_bytes, vir_bytes bss_bytes,
18730 vir_bytes stk_bytes, phys_bytes tot_bytes) );
18731 FORWARD _PROTOTYPE( void patch_ptr, (char stack[ARG_MAX], vir_bytes base) );
18732 FORWARD _PROTOTYPE( int insert_arg, (char stack[ARG_MAX],
18733 vir_bytes *stk_bytes, char *arg, int replace) );
18734 FORWARD _PROTOTYPE( char *patch_stack, (int fd, char stack[ARG_MAX],
18735 vir_bytes *stk_bytes, char *script) );
18736 FORWARD _PROTOTYPE( int read_header, (int fd, int *ft, vir_bytes *text_bytes,
18737 vir_bytes *data_bytes, vir_bytes *bss_bytes,
18738 phys_bytes *tot_bytes, long *sym_bytes, vir_clicks sc,
18739 vir_bytes *pc) );
18740
18741 #define ESCRIPT (-2000) /* Returned by read_header for a #! script. */
18742 #define PTRSIZE sizeof(char *) /* Size of pointers in argv[] and envp[]. */
18743
18744 /*===========================================================================*
18745 * do_exec *
18746 *===========================================================================*/
18747 PUBLIC int do_exec()
18748 {
18749 /* Perform the execve(name, argv, envp) call. The user library builds a
18750 * complete stack image, including pointers, args, environ, etc. The stack
18751 * is copied to a buffer inside PM, and then to the new core image.
18752 */
18753 register struct mproc *rmp;
18754 struct mproc *sh_mp;
18755 int m, r, fd, ft, sn;
18756 static char mbuf[ARG_MAX]; /* buffer for stack and zeroes */
18757 static char name_buf[PATH_MAX]; /* the name of the file to exec */
18758 char *new_sp, *name, *basename;
18759 vir_bytes src, dst, text_bytes, data_bytes, bss_bytes, stk_bytes, vsp;
18760 phys_bytes tot_bytes; /* total space for program, including gap */
18761 long sym_bytes;
18762 vir_clicks sc;
18763 struct stat s_buf[2], *s_p;
18764 vir_bytes pc;
MINIX SOURCE CODE File: servers/pm/exec.c 887
18765
18766 /* Do some validity checks. */
18767 rmp = mp;
18768 stk_bytes = (vir_bytes) m_in.stack_bytes;
18769 if (stk_bytes > ARG_MAX) return(ENOMEM); /* stack too big */
18770 if (m_in.exec_len <= 0 || m_in.exec_len > PATH_MAX) return(EINVAL);
18771
18772 /* Get the exec file name and see if the file is executable. */
18773 src = (vir_bytes) m_in.exec_name;
18774 dst = (vir_bytes) name_buf;
18775 r = sys_datacopy(who, (vir_bytes) src,
18776 PM_PROC_NR, (vir_bytes) dst, (phys_bytes) m_in.exec_len);
18777 if (r != OK) return(r); /* file name not in user data segment */
18778
18779 /* Fetch the stack from the user before destroying the old core image. */
18780 src = (vir_bytes) m_in.stack_ptr;
18781 dst = (vir_bytes) mbuf;
18782 r = sys_datacopy(who, (vir_bytes) src,
18783 PM_PROC_NR, (vir_bytes) dst, (phys_bytes)stk_bytes);
18784 /* can’t fetch stack (e.g. bad virtual addr) */
18785 if (r != OK) return(EACCES);
18786
18787 r = 0; /* r = 0 (first attempt), or 1 (interpreted script) */
18788 name = name_buf; /* name of file to exec. */
18789 do {
18790 s_p = &s_buf[r];
18791 tell_fs(CHDIR, who, FALSE, 0); /* switch to the user’s FS environ */
18792 fd = allowed(name, s_p, X_BIT); /* is file executable? */
18793 if (fd < 0) return(fd); /* file was not executable */
18794
18795 /* Read the file header and extract the segment sizes. */
18796 sc = (stk_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18797
18798 m = read_header(fd, &ft, &text_bytes, &data_bytes, &bss_bytes,
18799 &tot_bytes, &sym_bytes, sc, &pc);
18800 if (m != ESCRIPT || ++r > 1) break;
18801 } while ((name = patch_stack(fd, mbuf, &stk_bytes, name_buf)) != NULL);
18802
18803 if (m < 0) {
18804 close(fd); /* something wrong with header */
18805 return(stk_bytes > ARG_MAX ? ENOMEM : ENOEXEC);
18806 }
18807
18808 /* Can the process’ text be shared with that of one already running? */
18809 sh_mp = find_share(rmp, s_p->st_ino, s_p->st_dev, s_p->st_ctime);
18810
18811 /* Allocate new memory and release old memory. Fix map and tell kernel. */
18812 r = new_mem(sh_mp, text_bytes, data_bytes, bss_bytes, stk_bytes, tot_bytes);
18813 if (r != OK) {
18814 close(fd); /* insufficient core or program too big */
18815 return(r);
18816 }
18817
18818 /* Save file identification to allow it to be shared. */
18819 rmp->mp_ino = s_p->st_ino;
18820 rmp->mp_dev = s_p->st_dev;
18821 rmp->mp_ctime = s_p->st_ctime;
18822
18823 /* Patch up stack and copy it from PM to new core image. */
18824 vsp = (vir_bytes) rmp->mp_seg[S].mem_vir << CLICK_SHIFT;
888 File: servers/pm/exec.c MINIX SOURCE CODE
18825 vsp += (vir_bytes) rmp->mp_seg[S].mem_len << CLICK_SHIFT;
18826 vsp -= stk_bytes;
18827 patch_ptr(mbuf, vsp);
18828 src = (vir_bytes) mbuf;
18829 r = sys_datacopy(PM_PROC_NR, (vir_bytes) src,
18830 who, (vir_bytes) vsp, (phys_bytes)stk_bytes);
18831 if (r != OK) panic(__FILE__,"do_exec stack copy err on", who);
18832
18833 /* Read in text and data segments. */
18834 if (sh_mp != NULL) {
18835 lseek(fd, (off_t) text_bytes, SEEK_CUR); /* shared: skip text */
18836 } else {
18837 rw_seg(0, fd, who, T, text_bytes);
18838 }
18839 rw_seg(0, fd, who, D, data_bytes);
18840
18841 close(fd); /* don’t need exec file any more */
18842
18843 /* Take care of setuid/setgid bits. */
18844 if ((rmp->mp_flags & TRACED) == 0) { /* suppress if tracing */
18845 if (s_buf[0].st_mode & I_SET_UID_BIT) {
18846 rmp->mp_effuid = s_buf[0].st_uid;
18847 tell_fs(SETUID,who, (int)rmp->mp_realuid, (int)rmp->mp_effuid);
18848 }
18849 if (s_buf[0].st_mode & I_SET_GID_BIT) {
18850 rmp->mp_effgid = s_buf[0].st_gid;
18851 tell_fs(SETGID,who, (int)rmp->mp_realgid, (int)rmp->mp_effgid);
18852 }
18853 }
18854
18855 /* Save offset to initial argc (for ps) */
18856 rmp->mp_procargs = vsp;
18857
18858 /* Fix ’mproc’ fields, tell kernel that exec is done, reset caught sigs. */
18859 for (sn = 1; sn <= _NSIG; sn++) {
18860 if (sigismember(&rmp->mp_catch, sn)) {
18861 sigdelset(&rmp->mp_catch, sn);
18862 rmp->mp_sigact[sn].sa_handler = SIG_DFL;
18863 sigemptyset(&rmp->mp_sigact[sn].sa_mask);
18864 }
18865 }
18866
18867 rmp->mp_flags &= ˜SEPARATE; /* turn off SEPARATE bit */
18868 rmp->mp_flags |= ft; /* turn it on for separate I & D files */
18869 new_sp = (char *) vsp;
18870
18871 tell_fs(EXEC, who, 0, 0); /* allow FS to handle FD_CLOEXEC files */
18872
18873 /* System will save command line for debugging, ps(1) output, etc. */
18874 basename = strrchr(name, ’/’);
18875 if (basename == NULL) basename = name; else basename++;
18876 strncpy(rmp->mp_name, basename, PROC_NAME_LEN-1);
18877 rmp->mp_name[PROC_NAME_LEN] = ’\0’;
18878 sys_exec(who, new_sp, basename, pc);
18879
18880 /* Cause a signal if this process is traced. */
18881 if (rmp->mp_flags & TRACED) check_sig(rmp->mp_pid, SIGTRAP);
18882
18883 return(SUSPEND); /* no reply, new program just runs */
18884 }
MINIX SOURCE CODE File: servers/pm/exec.c 889
18886 /*===========================================================================*
18887 * read_header *
18888 *===========================================================================*/
18889 PRIVATE int read_header(fd, ft, text_bytes, data_bytes, bss_bytes,
18890 tot_bytes, sym_bytes, sc, pc)
18891 int fd; /* file descriptor for reading exec file */
18892 int *ft; /* place to return ft number */
18893 vir_bytes *text_bytes; /* place to return text size */
18894 vir_bytes *data_bytes; /* place to return initialized data size */
18895 vir_bytes *bss_bytes; /* place to return bss size */
18896 phys_bytes *tot_bytes; /* place to return total size */
18897 long *sym_bytes; /* place to return symbol table size */
18898 vir_clicks sc; /* stack size in clicks */
18899 vir_bytes *pc; /* program entry point (initial PC) */
18900 {
18901 /* Read the header and extract the text, data, bss and total sizes from it. */
18902
18903 int m, ct;
18904 vir_clicks tc, dc, s_vir, dvir;
18905 phys_clicks totc;
18906 struct exec hdr; /* a.out header is read in here */
18907
18908 /* Read the header and check the magic number. The standard MINIX header
18909 * is defined in <a.out.h>. It consists of 8 chars followed by 6 longs.
18910 * Then come 4 more longs that are not used here.
18911 * Byte 0: magic number 0x01
18912 * Byte 1: magic number 0x03
18913 * Byte 2: normal = 0x10 (not checked, 0 is OK), separate I/D = 0x20
18914 * Byte 3: CPU type, Intel 16 bit = 0x04, Intel 32 bit = 0x10,
18915 * Motorola = 0x0B, Sun SPARC = 0x17
18916 * Byte 4: Header length = 0x20
18917 * Bytes 5-7 are not used.
18918 *
18919 * Now come the 6 longs
18920 * Bytes 8-11: size of text segments in bytes
18921 * Bytes 12-15: size of initialized data segment in bytes
18922 * Bytes 16-19: size of bss in bytes
18923 * Bytes 20-23: program entry point
18924 * Bytes 24-27: total memory allocated to program (text, data + stack)
18925 * Bytes 28-31: size of symbol table in bytes
18926 * The longs are represented in a machine dependent order,
18927 * little-endian on the 8088, big-endian on the 68000.
18928 * The header is followed directly by the text and data segments, and the
18929 * symbol table (if any). The sizes are given in the header. Only the
18930 * text and data segments are copied into memory by exec. The header is
18931 * used here only. The symbol table is for the benefit of a debugger and
18932 * is ignored here.
18933 */
18934
18935 if ((m= read(fd, &hdr, A_MINHDR)) < 2) return(ENOEXEC);
18936
18937 /* Interpreted script? */
18938 if (((char *) &hdr)[0] == ’#’ && ((char *) &hdr)[1] == ’!’) return(ESCRIPT);
18939
18940 if (m != A_MINHDR) return(ENOEXEC);
18941
18942 /* Check magic number, cpu type, and flags. */
18943 if (BADMAG(hdr)) return(ENOEXEC);
18944 if (hdr.a_cpu != A_I80386) return(ENOEXEC);
890 File: servers/pm/exec.c MINIX SOURCE CODE
18945 if ((hdr.a_flags & ˜(A_NSYM | A_EXEC | A_SEP)) != 0) return(ENOEXEC);
18946
18947 *ft = ( (hdr.a_flags & A_SEP) ? SEPARATE : 0); /* separate I & D or not */
18948
18949 /* Get text and data sizes. */
18950 *text_bytes = (vir_bytes) hdr.a_text; /* text size in bytes */
18951 *data_bytes = (vir_bytes) hdr.a_data; /* data size in bytes */
18952 *bss_bytes = (vir_bytes) hdr.a_bss; /* bss size in bytes */
18953 *tot_bytes = hdr.a_total; /* total bytes to allocate for prog */
18954 *sym_bytes = hdr.a_syms; /* symbol table size in bytes */
18955 if (*tot_bytes == 0) return(ENOEXEC);
18956
18957 if (*ft != SEPARATE) {
18958 /* If I & D space is not separated, it is all considered data. Text=0*/
18959 *data_bytes += *text_bytes;
18960 *text_bytes = 0;
18961 }
18962 *pc = hdr.a_entry; /* initial address to start execution */
18963
18964 /* Check to see if segment sizes are feasible. */
18965 tc = ((unsigned long) *text_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18966 dc = (*data_bytes + *bss_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18967 totc = (*tot_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
18968 if (dc >= totc) return(ENOEXEC); /* stack must be at least 1 click */
18969 dvir = (*ft == SEPARATE ? 0 : tc);
18970 s_vir = dvir + (totc - sc);
18971 m = (dvir + dc > s_vir) ? ENOMEM : OK;
18972 ct = hdr.a_hdrlen & BYTE; /* header length */
18973 if (ct > A_MINHDR) lseek(fd, (off_t) ct, SEEK_SET); /* skip unused hdr */
18974 return(m);
18975 }
18977 /*===========================================================================*
18978 * new_mem *
18979 *===========================================================================*/
18980 PRIVATE int new_mem(sh_mp, text_bytes, data_bytes,
18981 bss_bytes,stk_bytes,tot_bytes)
18982 struct mproc *sh_mp; /* text can be shared with this process */
18983 vir_bytes text_bytes; /* text segment size in bytes */
18984 vir_bytes data_bytes; /* size of initialized data in bytes */
18985 vir_bytes bss_bytes; /* size of bss in bytes */
18986 vir_bytes stk_bytes; /* size of initial stack segment in bytes */
18987 phys_bytes tot_bytes; /* total memory to allocate, including gap */
18988 {
18989 /* Allocate new memory and release the old memory. Change the map and report
18990 * the new map to the kernel. Zero the new core image’s bss, gap and stack.
18991 */
18992
18993 register struct mproc *rmp = mp;
18994 vir_clicks text_clicks, data_clicks, gap_clicks, stack_clicks, tot_clicks;
18995 phys_clicks new_base;
18996 phys_bytes bytes, base, bss_offset;
18997 int s;
18998
18999 /* No need to allocate text if it can be shared. */
19000 if (sh_mp != NULL) text_bytes = 0;
19001
19002 /* Allow the old data to be swapped out to make room. (Which is really a
19003 * waste of time, because we are going to throw it away anyway.)
19004 */
MINIX SOURCE CODE File: servers/pm/exec.c 891
19005 rmp->mp_flags |= WAITING;
19006
19007 /* Acquire the new memory. Each of the 4 parts: text, (data+bss), gap,
19008 * and stack occupies an integral number of clicks, starting at click
19009 * boundary. The data and bss parts are run together with no space.
19010 */
19011 text_clicks = ((unsigned long) text_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19012 data_clicks = (data_bytes + bss_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19013 stack_clicks = (stk_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19014 tot_clicks = (tot_bytes + CLICK_SIZE - 1) >> CLICK_SHIFT;
19015 gap_clicks = tot_clicks - data_clicks - stack_clicks;
19016 if ( (int) gap_clicks < 0) return(ENOMEM);
19017
19018 /* Try to allocate memory for the new process. */
19019 new_base = alloc_mem(text_clicks + tot_clicks);
19020 if (new_base == NO_MEM) return(ENOMEM);
19021
19022 /* We’ve got memory for the new core image. Release the old one. */
19023 rmp = mp;
19024
19025 if (find_share(rmp, rmp->mp_ino, rmp->mp_dev, rmp->mp_ctime) == NULL) {
19026 /* No other process shares the text segment, so free it. */
19027 free_mem(rmp->mp_seg[T].mem_phys, rmp->mp_seg[T].mem_len);
19028 }
19029 /* Free the data and stack segments. */
19030 free_mem(rmp->mp_seg[D].mem_phys,
19031 rmp->mp_seg[S].mem_vir + rmp->mp_seg[S].mem_len - rmp->mp_seg[D].mem_vir);
19032
19033 /* We have now passed the point of no return. The old core image has been
19034 * forever lost, memory for a new core image has been allocated. Set up
19035 * and report new map.
19036 */
19037 if (sh_mp != NULL) {
19038 /* Share the text segment. */
19039 rmp->mp_seg[T] = sh_mp->mp_seg[T];
19040 } else {
19041 rmp->mp_seg[T].mem_phys = new_base;
19042 rmp->mp_seg[T].mem_vir = 0;
19043 rmp->mp_seg[T].mem_len = text_clicks;
19044 }
19045 rmp->mp_seg[D].mem_phys = new_base + text_clicks;
19046 rmp->mp_seg[D].mem_vir = 0;
19047 rmp->mp_seg[D].mem_len = data_clicks;
19048 rmp->mp_seg[S].mem_phys = rmp->mp_seg[D].mem_phys + data_clicks + gap_clicks;
19049 rmp->mp_seg[S].mem_vir = rmp->mp_seg[D].mem_vir + data_clicks + gap_clicks;
19050 rmp->mp_seg[S].mem_len = stack_clicks;
19051
19052 sys_newmap(who, rmp->mp_seg); /* report new map to the kernel */
19053
19054 /* The old memory may have been swapped out, but the new memory is real. */
19055 rmp->mp_flags &= ˜(WAITING|ONSWAP|SWAPIN);
19056
19057 /* Zero the bss, gap, and stack segment. */
19058 bytes = (phys_bytes)(data_clicks + gap_clicks + stack_clicks) << CLICK_SHIFT;
19059 base = (phys_bytes) rmp->mp_seg[D].mem_phys << CLICK_SHIFT;
19060 bss_offset = (data_bytes >> CLICK_SHIFT) << CLICK_SHIFT;
19061 base += bss_offset;
19062 bytes -= bss_offset;
19063
19064 if ((s=sys_memset(0, base, bytes)) != OK) {
892 File: servers/pm/exec.c MINIX SOURCE CODE
19065 panic(__FILE__,"new_mem can’t zero", s);
19066 }
19067
19068 return(OK);
19069 }
19071 /*===========================================================================*
19072 * patch_ptr *
19073 *===========================================================================*/
19074 PRIVATE void patch_ptr(stack, base)
19075 char stack[ARG_MAX]; /* pointer to stack image within PM */
19076 vir_bytes base; /* virtual address of stack base inside user */
19077 {
19078 /* When doing an exec(name, argv, envp) call, the user builds up a stack
19079 * image with arg and env pointers relative to the start of the stack. Now
19080 * these pointers must be relocated, since the stack is not positioned at
19081 * address 0 in the user’s address space.
19082 */
19083
19084 char **ap, flag;
19085 vir_bytes v;
19086
19087 flag = 0; /* counts number of 0-pointers seen */
19088 ap = (char **) stack; /* points initially to ’nargs’ */
19089 ap++; /* now points to argv[0] */
19090 while (flag < 2) {
19091 if (ap >= (char **) &stack[ARG_MAX]) return; /* too bad */
19092 if (*ap != NULL) {
19093 v = (vir_bytes) *ap; /* v is relative pointer */
19094 v += base; /* relocate it */
19095 *ap = (char *) v; /* put it back */
19096 } else {
19097 flag++;
19098 }
19099 ap++;
19100 }
19101 }
19103 /*===========================================================================*
19104 * insert_arg *
19105 *===========================================================================*/
19106 PRIVATE int insert_arg(stack, stk_bytes, arg, replace)
19107 char stack[ARG_MAX]; /* pointer to stack image within PM */
19108 vir_bytes *stk_bytes; /* size of initial stack */
19109 char *arg; /* argument to prepend/replace as new argv[0] */
19110 int replace;
19111 {
19112 /* Patch the stack so that arg will become argv[0]. Be careful, the stack may
19113 * be filled with garbage, although it normally looks like this:
19114 * nargs argv[0] ... argv[nargs-1] NULL envp[0] ... NULL
19115 * followed by the strings "pointed" to by the argv[i] and the envp[i]. The
19116 * pointers are really offsets from the start of stack.
19117 * Return true iff the operation succeeded.
19118 */
19119 int offset, a0, a1, old_bytes = *stk_bytes;
19120
19121 /* Prepending arg adds at least one string and a zero byte. */
19122 offset = strlen(arg) + 1;
19123
19124 a0 = (int) ((char **) stack)[1]; /* argv[0] */
MINIX SOURCE CODE File: servers/pm/exec.c 893
19125 if (a0 < 4 * PTRSIZE || a0 >= old_bytes) return(FALSE);
19126
19127 a1 = a0; /* a1 will point to the strings to be moved */
19128 if (replace) {
19129 /* Move a1 to the end of argv[0][] (argv[1] if nargs > 1). */
19130 do {
19131 if (a1 == old_bytes) return(FALSE);
19132 --offset;
19133 } while (stack[a1++] != 0);
19134 } else {
19135 offset += PTRSIZE; /* new argv[0] needs new pointer in argv[] */
19136 a0 += PTRSIZE; /* location of new argv[0][]. */
19137 }
19138
19139 /* stack will grow by offset bytes (or shrink by -offset bytes) */
19140 if ((*stk_bytes += offset) > ARG_MAX) return(FALSE);
19141
19142 /* Reposition the strings by offset bytes */
19143 memmove(stack + a1 + offset, stack + a1, old_bytes - a1);
19144
19145 strcpy(stack + a0, arg); /* Put arg in the new space. */
19146
19147 if (!replace) {
19148 /* Make space for a new argv[0]. */
19149 memmove(stack + 2 * PTRSIZE, stack + 1 * PTRSIZE, a0 - 2 * PTRSIZE);
19150
19151 ((char **) stack)[0]++; /* nargs++; */
19152 }
19153 /* Now patch up argv[] and envp[] by offset. */
19154 patch_ptr(stack, (vir_bytes) offset);
19155 ((char **) stack)[1] = (char *) a0; /* set argv[0] correctly */
19156 return(TRUE);
19157 }
19159 /*===========================================================================*
19160 * patch_stack *
19161 *===========================================================================*/
19162 PRIVATE char *patch_stack(fd, stack, stk_bytes, script)
19163 int fd; /* file descriptor to open script file */
19164 char stack[ARG_MAX]; /* pointer to stack image within PM */
19165 vir_bytes *stk_bytes; /* size of initial stack */
19166 char *script; /* name of script to interpret */
19167 {
19168 /* Patch the argument vector to include the path name of the script to be
19169 * interpreted, and all strings on the #! line. Returns the path name of
19170 * the interpreter.
19171 */
19172 char *sp, *interp = NULL;
19173 int n;
19174 enum { INSERT=FALSE, REPLACE=TRUE };
19175
19176 /* Make script[] the new argv[0]. */
19177 if (!insert_arg(stack, stk_bytes, script, REPLACE)) return(NULL);
19178
19179 if (lseek(fd, 2L, 0) == -1 /* just behind the #! */
19180 || (n= read(fd, script, PATH_MAX)) < 0 /* read line one */
19181 || (sp= memchr(script, ’\n’, n)) == NULL) /* must be a proper line */
19182 return(NULL);
19183
19184 /* Move sp backwards through script[], prepending each string to stack. */
894 File: servers/pm/exec.c MINIX SOURCE CODE
19185 for (;;) {
19186 /* skip spaces behind argument. */
19187 while (sp > script && (*--sp == ’ ’ || *sp == ’\t’)) {}
19188 if (sp == script) break;
19189
19190 sp[1] = 0;
19191 /* Move to the start of the argument. */
19192 while (sp > script && sp[-1] != ’ ’ && sp[-1] != ’\t’) --sp;
19193
19194 interp = sp;
19195 if (!insert_arg(stack, stk_bytes, sp, INSERT)) return(NULL);
19196 }
19197
19198 /* Round *stk_bytes up to the size of a pointer for alignment contraints. */
19199 *stk_bytes= ((*stk_bytes + PTRSIZE - 1) / PTRSIZE) * PTRSIZE;
19200
19201 close(fd);
19202 return(interp);
19203 }
19205 /*===========================================================================*
19206 * rw_seg *
19207 *===========================================================================*/
19208 PUBLIC void rw_seg(rw, fd, proc, seg, seg_bytes0)
19209 int rw; /* 0 = read, 1 = write */
19210 int fd; /* file descriptor to read from / write to */
19211 int proc; /* process number */
19212 int seg; /* T, D, or S */
19213 phys_bytes seg_bytes0; /* how much is to be transferred? */
19214 {
19215 /* Transfer text or data from/to a file and copy to/from a process segment.
19216 * This procedure is a little bit tricky. The logical way to transfer a
19217 * segment would be block by block and copying each block to/from the user
19218 * space one at a time. This is too slow, so we do something dirty here,
19219 * namely send the user space and virtual address to the file system in the
19220 * upper 10 bits of the file descriptor, and pass it the user virtual address
19221 * instead of a PM address. The file system extracts these parameters when
19222 * gets a read or write call from the process manager, which is the only
19223 * process that is permitted to use this trick. The file system then copies
19224 * the whole segment directly to/from user space, bypassing PM completely.
19225 *
19226 * The byte count on read is usually smaller than the segment count, because
19227 * a segment is padded out to a click multiple, and the data segment is only
19228 * partially initialized.
19229 */
19230
19231 int new_fd, bytes, r;
19232 char *ubuf_ptr;
19233 struct mem_map *sp = &mproc[proc].mp_seg[seg];
19234 phys_bytes seg_bytes = seg_bytes0;
19235
19236 new_fd = (proc << 7) | (seg << 5) | fd;
19237 ubuf_ptr = (char *) ((vir_bytes) sp->mem_vir << CLICK_SHIFT);
19238
19239 while (seg_bytes != 0) {
19240 #define PM_CHUNK_SIZE 8192
19241 bytes = MIN((INT_MAX / PM_CHUNK_SIZE) * PM_CHUNK_SIZE, seg_bytes);
19242 if (rw == 0) {
19243 r = read(new_fd, ubuf_ptr, bytes);
19244 } else {
MINIX SOURCE CODE File: servers/pm/exec.c 895
19245 r = write(new_fd, ubuf_ptr, bytes);
19246 }
19247 if (r != bytes) break;
19248 ubuf_ptr += bytes;
19249 seg_bytes -= bytes;
19250 }
19251 }
19253 /*===========================================================================*
19254 * find_share *
19255 *===========================================================================*/
19256 PUBLIC struct mproc *find_share(mp_ign, ino, dev, ctime)
19257 struct mproc *mp_ign; /* process that should not be looked at */
19258 ino_t ino; /* parameters that uniquely identify a file */
19259 dev_t dev;
19260 time_t ctime;
19261 {
19262 /* Look for a process that is the file <ino, dev, ctime> in execution. Don’t
19263 * accidentally "find" mp_ign, because it is the process on whose behalf this
19264 * call is made.
19265 */
19266 struct mproc *sh_mp;
19267 for (sh_mp = &mproc[0]; sh_mp < &mproc[NR_PROCS]; sh_mp++) {
19268
19269 if (!(sh_mp->mp_flags & SEPARATE)) continue;
19270 if (sh_mp == mp_ign) continue;
19271 if (sh_mp->mp_ino != ino) continue;
19272 if (sh_mp->mp_dev != dev) continue;
19273 if (sh_mp->mp_ctime != ctime) continue;
19274 return sh_mp;
19275 }
19276 return(NULL);
19277 }