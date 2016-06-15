++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/param.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17700 /* The following names are synonyms for the variables in the input message. */
17701 #define addr m1_p1
17702 #define exec_name m1_p1
17703 #define exec_len m1_i1
17704 #define func m6_f1
872 File: servers/pm/param.h MINIX SOURCE CODE
17705 #define grp_id m1_i1
17706 #define namelen m1_i2
17707 #define pid m1_i1
17708 #define procnr m1_i1
17709 #define seconds m1_i1
17710 #define sig m6_i1
17711 #define stack_bytes m1_i2
17712 #define stack_ptr m1_p2
17713 #define status m1_i1
17714 #define usr_id m1_i1
17715 #define request m2_i2
17716 #define taddr m2_l1
17717 #define data m2_l2
17718 #define sig_nr m1_i2
17719 #define sig_nsa m1_p1
17720 #define sig_osa m1_p2
17721 #define sig_ret m1_p3
17722 #define sig_set m2_l1
17723 #define sig_how m2_i1
17724 #define sig_flags m2_i2
17725 #define sig_context m2_p1
17726 #define info_what m1_i1
17727 #define info_where m1_p1
17728 #define reboot_flag m1_i1
17729 #define reboot_code m1_p1
17730 #define reboot_strlen m1_i2
17731 #define svrctl_req m2_i1
17732 #define svrctl_argp m2_p1
17733 #define stime m2_l1
17734 #define memsize m4_l1
17735 #define membase m4_l2
17736
17737 /* The following names are synonyms for the variables in a reply message. */
17738 #define reply_res m_type
17739 #define reply_res2 m2_i1
17740 #define reply_ptr m2_p1
17741 #define reply_mask m2_l1
17742 #define reply_trace m2_l2
17743 #define reply_time m2_l1
17744 #define reply_utime m2_l2
17745 #define reply_t1 m4_l1
17746 #define reply_t2 m4_l2
17747 #define reply_t3 m4_l3
17748 #define reply_t4 m4_l4
17749 #define reply_t5 m4_l5
17750
17751 /* The following names are used to inform the FS about certain events. */
17752 #define tell_fs_arg1 m1_i1
17753 #define tell_fs_arg2 m1_i2
17754 #define tell_fs_arg3 m1_i3
17755