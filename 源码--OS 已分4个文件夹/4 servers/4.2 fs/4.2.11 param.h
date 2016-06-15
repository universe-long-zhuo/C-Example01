++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/fs/param.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
22000 /* The following names are synonyms for the variables in the input message. */
22001 #define acc_time m2_l1
22002 #define addr m1_i3
22003 #define buffer m1_p1
22004 #define child m1_i2
22005 #define co_mode m1_i1
22006 #define eff_grp_id m1_i3
22007 #define eff_user_id m1_i3
22008 #define erki m1_p1
22009 #define fd m1_i1
22010 #define fd2 m1_i2
22011 #define ioflags m1_i3
22012 #define group m1_i3
22013 #define real_grp_id m1_i2
22014 #define ls_fd m2_i1
22015 #define mk_mode m1_i2
22016 #define mk_z0 m1_i3
22017 #define mode m3_i2
22018 #define c_mode m1_i3
22019 #define c_name m1_p1
22020 #define name m3_p1
22021 #define name1 m1_p1
22022 #define name2 m1_p2
22023 #define name_length m3_i1
22024 #define name1_length m1_i1
22025 #define name2_length m1_i2
22026 #define nbytes m1_i2
22027 #define owner m1_i2
22028 #define parent m1_i1
22029 #define pathname m3_ca1
22030 #define pid m1_i3
22031 #define pro m1_i1
22032 #define ctl_req m4_l1
22033 #define driver_nr m4_l2
22034 #define dev_nr m4_l3
930 File: servers/fs/param.h MINIX SOURCE CODE
22035 #define dev_style m4_l4
22036 #define rd_only m1_i3
22037 #define real_user_id m1_i2
22038 #define request m1_i2
22039 #define sig m1_i2
22040 #define slot1 m1_i1
22041 #define tp m2_l1
22042 #define utime_actime m2_l1
22043 #define utime_modtime m2_l2
22044 #define utime_file m2_p1
22045 #define utime_length m2_i1
22046 #define utime_strlen m2_i2
22047 #define whence m2_i2
22048 #define svrctl_req m2_i1
22049 #define svrctl_argp m2_p1
22050 #define pm_stime m1_i1
22051 #define info_what m1_i1
22052 #define info_where m1_p1
22053
22054 /* The following names are synonyms for the variables in the output message. */
22055 #define reply_type m_type
22056 #define reply_l1 m2_l1
22057 #define reply_i1 m1_i1
22058 #define reply_i2 m1_i2
22059 #define reply_t1 m4_l1
22060 #define reply_t2 m4_l2
22061 #define reply_t3 m4_l3
22062 #define reply_t4 m4_l4
22063 #define reply_t5 m4_l5