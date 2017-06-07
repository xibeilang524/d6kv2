drop table modbusdaqo_special_config;
create table modbusdaqo_special_config
	(
	channel_no	 number(5)	not null,
	yx_func_code number(5)	default 1 not null,
	yx_start_no  number(5)	default 3000 not null,
	yx_call_time number(5)	default 300 not null,
	yc_func_code number(5)	default 3 not null,
	yc_start_no  number(5)	default 0 not null,
	yc_call_time number(5)	default 1000 not null,
	yk_func_code number(5)	default 6 not null,
	yk_start_no  number(5)	default 9 not null,
	t0	number(5)	default 15 not null
	);
alter table modbusdaqo_special_config
add constraint modbusdaqo_special_config
 primary key  (channel_no);