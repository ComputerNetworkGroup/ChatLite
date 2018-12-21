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
msg_time DATETIME ,
msg varchar(140),
key msg_time(msg_time)
);

CREATE TABLE test(
id VARCHAR(32)
);
insert into test values("1");
insert into test values("2");
insert into test values("3");
insert into test values("4");
insert into test values("5");
