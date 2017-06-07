drop table 授权权限参数表;
create table 授权权限参数表
(
	组号 number(5) not null,
	授权周期 number(5) not null,
	文件生成时间 char(24) not null,
	文件导入时间 char(24) not null,
	MAC地址 char(24) not null,
	是否过期 number(5) not null,
	primary key(组号)
);
