drop DATABASE if EXISTS test; 
create DATABASE test;
use test;

CREATE TABLE userlist(
id varchar(32) PRIMARY KEY,
passwd varchar(32),
log_time DATETIME 
);

CREATE table settings(
id varchar(32) PRIMARY KEY,
set_color INTEGER

);

CREATE TABLE msgs(
from_id VARCHAR(32),
to_id VARCHAR(32),
msg_time DATETIME,
msg varchar(140)
);

CREATE TABLE test(
id VARCHAR(32)
);
insert into userlist values("1",MD5('1'),'1970-1-1 00:00:00');
insert into userlist values("2",MD5('2'),'1988-1-1 00:00:00');
insert into userlist values("3",MD5('3'),'1970-1-1 00:00:00');
insert into userlist values("4",MD5('4'),'1970-1-1 00:00:00');
insert into userlist values("5",MD5('5'),'1970-1-1 00:00:00');
