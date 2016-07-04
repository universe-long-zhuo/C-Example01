++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             include/ansi.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* The <ansi.h> header attempts to decide whether the compiler has enough
* conformance to Standard C for Minix to take advantage of. If so, the
* symbol _ANSI is defined (as ). Otherwise _ANSI is not defined
* here, but it may be defined by applications that want to bend the rules.
* The magic number in the definition is to inhibit unnecessary bending
* of the rules. (For consistency with the new ’#ifdef _ANSI" tests in
* the headers, _ANSI should really be defined as nothing, but that would
* break many library routines that use "#if _ANSI".)
* If _ANSI ends up being defined, a macro
*
* _PROTOTYPE(function, params)
*
* is defined. This macro expands in different不同的 ways方式, generating生成 either二者之一
* ANSI Standard C prototypes or old-style K&R (Kernighan & Ritchie)
* prototypes, as needed. Finally终于, some programs use _CONST, _VOIDSTAR etc
* in such a way that they are portable over both ANSI and K&R compilers.
* The appropriate macros are defined here.
*/

/* 美国国家标准学会（American National Standards Institute，ANSI）
* 词根词缀记忆法 at·tempt 试图，tempt 诱惑，temp的缩写是温度，所以不要容易记错。
* decide       v. 决定
* whether   conj. 是否
* enough    conj. 足够
* conformance  n. 一致性；符合性 词根con·form·ance
* advantage    n. 优点
* symbol       n. 符号 icon n. 图标
* define       v. 定义
* definition   n. 定义
* otherwise conj. 否则
* want         v. 要
* bend         v. 弯曲
* rule         n. 规则
* magic        n.
* inhibit      v. 抑制；阻止
* unnecessary　adj. 不需要的     词根un·neces·sary
* should really
* nothing      n. 不存在(的东西)
* would     auxv. 会
* macro        n. 宏指令
*
* _原型（函数，参数）
* 
* expand，相近extend
* different 
* appropriate
*/ 

#ifndef _ANSI_H
#define _ANSI_H

#if __STDC__ == 1
#define _ANSI /* compiler claims要求 full ANSI conformance */
#endif

/* 通用公共许可证（GNU General Public License，GPL） */
#ifdef __GNUC__  
#define _ANSI /* gcc conforms enough even in non-ANSI mode */
#endif

#ifdef _ANSI

/* Keep everything一切 for ANSI prototypes. */
#define _PROTOTYPE(function, params) function params
#define _ARGS(params) params

#define _VOIDSTAR void *
#define _VOID void
#define _CONST const
#define _VOLATILE volatile
#define _SIZET size_t

#else

/* Throw away the parameters for K&R prototypes. */
#define _PROTOTYPE(function, params) function()
#define _ARGS(params) ()

#define _VOIDSTAR void *
#define _VOID void
#define _CONST
#define _VOLATILE
#define _SIZET int

#endif /* _ANSI */
                        /* 相近limits限制 */
/* This should be defined as restrict限制 when a C99 compiler is used. */
#define _RESTRICT

/* 可移植操作系统接口(Portable Operating System Interface X) */

/* Setting any of _MINIX, _POSIX_C_SOURCE or _POSIX2_SOURCE implies
* _POSIX_SOURCE. (Seems wrong to put this here in ANSI space.)
*/
#if defined(_MINIX) || _POSIX_C_SOURCE > 0 || defined(_POSIX2_SOURCE)
#undef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif

#endif /* ANSI_H */


/* #if、#else、#undef、#ifdef、#ifndef、#endif 都是什么？*/